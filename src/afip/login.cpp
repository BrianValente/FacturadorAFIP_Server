#include <string>
#include <fstream>
#include <ctime>

#include "login.hpp"

#include "soap/soapLoginCmsSoapBindingProxy.h"
#include "soap/LoginCmsSoapBinding.nsmap"

#include "wsfe.hpp"

#include "libraries/tinyxml2/tinyxml2.h"

#include <openssl/pem.h>
#include <openssl/cms.h>
#include <openssl/err.h>

using namespace std;
using namespace tinyxml2;

namespace Login {
	const char loginEndpoint[] = "https://wsaahomo.afip.gov.ar/ws/services/LoginCms";
    //const char loginEndpoint[] = "https://wsaa.afip.gov.ar/ws/services/LoginCms";

	string getEpoch() {
		std::time_t result = std::time(0);
    	string str = to_string(result);
    	return str;
	}

	string getCurrentDateTime(int sum = 0) {
		// https://stackoverflow.com/questions/16357999/current-date-and-time-as-string

		time_t rawtime;
		struct tm * timeinfo;
		char buffer[80];

		time (&rawtime);
		rawtime += sum;
        rawtime -= 10800; // UTC-3
		timeinfo = gmtime(&rawtime);

		//strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);
		strftime(buffer,sizeof(buffer),"%Y-%m-%dT%H:%M:%S",timeinfo);
		std::string str(buffer);

		return str;
	}

	string signTicketRequest(string request) {
		// https://github.com/openssl/openssl/blob/master/demos/cms/cms_sign.c

		ofstream requestFile;

		requestFile.open("LoginTicketRequest.xml");
		requestFile << request;
		requestFile.close();

	    BIO *in = NULL, *out = NULL, *pem = NULL, *key = NULL;
	    X509 *scert = NULL;
	    EVP_PKEY *skey = NULL;
	    CMS_ContentInfo *cms = NULL;
	    int ret = 1;
	    string result;

	    int flags = 0;

	    OpenSSL_add_all_algorithms();
	    ERR_load_crypto_strings();

	    pem = BIO_new_file("afip-cert-x509v2.pem", "r");
	    key = BIO_new_file("private.key", "r");

	    if (!pem || !key)
	        goto err;

	    scert = PEM_read_bio_X509(pem, NULL, 0, NULL);

	    BIO_reset(pem);

	    skey = PEM_read_bio_PrivateKey(key, NULL, 0, NULL);

	    if (!scert || !skey)
	        goto err;

	    in = BIO_new_file("LoginTicketRequest.xml", "r");

	    if (!in)
	        goto err;

	    /* Sign content */
	    cms = CMS_sign(scert, skey, NULL, in, flags);

	    if (!cms)
	        goto err;

	    out = BIO_new_file("smout.txt", "w");
	    if (!out)
	        goto err;

	    if (!(flags & CMS_STREAM))
	        BIO_reset(in);

	    if (!PEM_write_bio_CMS_stream(out, cms, in, flags))
	        goto err;

	    ret = 0;

	 err:

	    if (ret) {
	        fprintf(stderr, "Error Signing Data\n");
	        ERR_print_errors_fp(stderr);
	    }

	    CMS_ContentInfo_free(cms);
	    X509_free(scert);
	    EVP_PKEY_free(skey);
	    BIO_free(in);
	    BIO_free(out);
	    BIO_free(pem);
	    BIO_free(key);

	    ifstream signedRequestFile;

	    signedRequestFile.open("smout.txt");

	    result = string((std::istreambuf_iterator<char>(signedRequestFile)),std::istreambuf_iterator<char>());

	    // https://stackoverflow.com/questions/8247102/how-to-remove-a-line-from-a-string-with-large-content-in-c
	    result.erase(0, ((string)"-----BEGIN CMS-----").length() + 1);
	    string endText = "-----END CMS-----";
	    unsigned long endPos = result.find(endText);
	    result.erase(endPos, endText.length() + 1);

	    return result;
	}

	string buildTicketRequest() {
		string xml = "<loginTicketRequest><header><uniqueId>";
		xml.append(getEpoch());
		xml.append("</uniqueId><generationTime>");
		xml.append(getCurrentDateTime(-60));
		xml.append("</generationTime><expirationTime>");
		xml.append(getCurrentDateTime(60));
		xml.append("</expirationTime></header>");
		xml.append("<service>wsfe</service></loginTicketRequest>");

		return xml;
	}

	string generateSignedTicketRequest() {
		return signTicketRequest(buildTicketRequest());
	}

	bool credentialsAreValid(AccessCredentials *credentials) {
		return credentials != NULL && Wsfe::testConnection(credentials);
	}

	AccessCredentials* getCredentialsFromDisk() {
		ifstream credentialsFile;
		string token, sign;

		credentialsFile.open("credentials.txt");

		if (!credentialsFile.is_open())
			return NULL;

		if (!credentialsFile.eof())
			getline(credentialsFile, token);
		if (!credentialsFile.eof())
			getline(credentialsFile, sign);

		if (token.empty() || sign.empty())
			return NULL;

		return new AccessCredentials(token, sign);
	}

	void storeCredentialsOnDisk(AccessCredentials *credentials) {
		ofstream tokenFile;
		tokenFile.open("credentials.txt");
		tokenFile << credentials->token << endl << credentials->sign;
		tokenFile.close();
	}

	AccessCredentials* getCredentialsFromXmlString(const char *xml) {
		XMLDocument doc;
		doc.Parse(xml);

		XMLElement* credentialsXml = doc.FirstChildElement( "loginTicketResponse" )
			->FirstChildElement( "credentials" );

		const char* token = credentialsXml->FirstChildElement( "token" )->GetText();
		const char* sign = credentialsXml->FirstChildElement( "sign" )->GetText();

		return new AccessCredentials(string(token), string(sign));
	}

	AccessCredentials* getCredentialsFromServer() {		
		AccessCredentials *accessCredentials;

		LoginCmsSoapBindingProxy login(loginEndpoint);
		_afip1__loginCms loginCms;
		_afip1__loginCmsResponse response;

		login.soap->connect_timeout = 5;
		login.soap->connect_retry = 0;

		cout << "Retrieving access credentials from server" << endl;

		loginCms.in0 = generateSignedTicketRequest();

		login.loginCms(&loginCms, response);

		if (login.soap->error) {
			cerr << "There was an error retrieving credentials from server:" << endl << endl;
			login.soap_stream_fault(std::cerr);
			cerr << endl << endl;
			return NULL;
		}

		accessCredentials = getCredentialsFromXmlString(response.loginCmsReturn.c_str());
		cout << "Credentials received from server successfully!" << endl;
		return accessCredentials;
	}

	AccessCredentials* getAccessCredentials() {
		AccessCredentials* credentials = getCredentialsFromDisk();

		if (credentialsAreValid(credentials))
			return credentials;

		credentials = getCredentialsFromServer();

		if (!credentialsAreValid(credentials))
			return NULL;

		storeCredentialsOnDisk(credentials);
		return credentials;
	}
}

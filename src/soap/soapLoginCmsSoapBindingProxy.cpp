/* soapLoginCmsSoapBindingProxy.cpp
   Generated by gSOAP 2.8.89 for soap/afip_soap.h

gSOAP XML Web services tools
Copyright (C) 2000-2018, Robert van Engelen, Genivia Inc. All Rights Reserved.
The soapcpp2 tool and its generated software are released under the GPL.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#include "soapLoginCmsSoapBindingProxy.h"

LoginCmsSoapBindingProxy::LoginCmsSoapBindingProxy()
{	this->soap = soap_new();
	this->soap_own = true;
	LoginCmsSoapBindingProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

LoginCmsSoapBindingProxy::LoginCmsSoapBindingProxy(const LoginCmsSoapBindingProxy& rhs)
{	this->soap = rhs.soap;
	this->soap_own = false;
	this->soap_endpoint = rhs.soap_endpoint;
}

LoginCmsSoapBindingProxy::LoginCmsSoapBindingProxy(struct soap *_soap)
{	this->soap = _soap;
	this->soap_own = false;
	LoginCmsSoapBindingProxy_init(_soap->imode, _soap->omode);
}

LoginCmsSoapBindingProxy::LoginCmsSoapBindingProxy(struct soap *_soap, const char *soap_endpoint_url)
{	this->soap = _soap;
	this->soap_own = false;
	LoginCmsSoapBindingProxy_init(_soap->imode, _soap->omode);
	soap_endpoint = soap_endpoint_url;
}

LoginCmsSoapBindingProxy::LoginCmsSoapBindingProxy(const char *soap_endpoint_url)
{	this->soap = soap_new();
	this->soap_own = true;
	LoginCmsSoapBindingProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
	soap_endpoint = soap_endpoint_url;
}

LoginCmsSoapBindingProxy::LoginCmsSoapBindingProxy(soap_mode iomode)
{	this->soap = soap_new();
	this->soap_own = true;
	LoginCmsSoapBindingProxy_init(iomode, iomode);
}

LoginCmsSoapBindingProxy::LoginCmsSoapBindingProxy(const char *soap_endpoint_url, soap_mode iomode)
{	this->soap = soap_new();
	this->soap_own = true;
	LoginCmsSoapBindingProxy_init(iomode, iomode);
	soap_endpoint = soap_endpoint_url;
}

LoginCmsSoapBindingProxy::LoginCmsSoapBindingProxy(soap_mode imode, soap_mode omode)
{	this->soap = soap_new();
	this->soap_own = true;
	LoginCmsSoapBindingProxy_init(imode, omode);
}

LoginCmsSoapBindingProxy::~LoginCmsSoapBindingProxy()
{	if (this->soap_own)
	{	this->destroy();
		soap_free(this->soap);
	}
}

void LoginCmsSoapBindingProxy::LoginCmsSoapBindingProxy_init(soap_mode imode, soap_mode omode)
{	soap_imode(this->soap, imode);
	soap_omode(this->soap, omode);
	soap_endpoint = NULL;
	static const struct Namespace namespaces[] = {
        { "SOAP-ENV", "http://www.w3.org/2003/05/soap-envelope", "http://schemas.xmlsoap.org/soap/envelope/", NULL },
        { "SOAP-ENC", "http://www.w3.org/2003/05/soap-encoding", "http://schemas.xmlsoap.org/soap/encoding/", NULL },
        { "xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL },
        { "xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL },
        { "afip2", "https://wsaahomo.afip.gov.ar/ws/services/LoginCms", NULL, NULL },
        { "afip1", "http://wsaa.view.sua.dvadac.desein.afip.gov", NULL, NULL },
        { "afip4", "http://ar.gov.afip.dif.FEV1/ServiceSoap", NULL, NULL },
        { "afip3", "http://ar.gov.afip.dif.FEV1/", NULL, NULL },
        { "afip5", "http://ar.gov.afip.dif.FEV1/ServiceSoap12", NULL, NULL },
        { NULL, NULL, NULL, NULL}
    };
	soap_set_namespaces(this->soap, namespaces);
}

LoginCmsSoapBindingProxy *LoginCmsSoapBindingProxy::copy()
{	LoginCmsSoapBindingProxy *dup = SOAP_NEW_UNMANAGED(LoginCmsSoapBindingProxy);
	if (dup)
	{	soap_done(dup->soap);
		soap_copy_context(dup->soap, this->soap);
	}
	return dup;
}

LoginCmsSoapBindingProxy& LoginCmsSoapBindingProxy::operator=(const LoginCmsSoapBindingProxy& rhs)
{	if (this->soap != rhs.soap)
	{	if (this->soap_own)
			soap_free(this->soap);
		this->soap = rhs.soap;
		this->soap_own = false;
		this->soap_endpoint = rhs.soap_endpoint;
	}
	return *this;
}

void LoginCmsSoapBindingProxy::destroy()
{	soap_destroy(this->soap);
	soap_end(this->soap);
}

void LoginCmsSoapBindingProxy::reset()
{	this->destroy();
	soap_done(this->soap);
	soap_initialize(this->soap);
	LoginCmsSoapBindingProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

void LoginCmsSoapBindingProxy::soap_noheader()
{	this->soap->header = NULL;
}

::SOAP_ENV__Header *LoginCmsSoapBindingProxy::soap_header()
{	return this->soap->header;
}

::SOAP_ENV__Fault *LoginCmsSoapBindingProxy::soap_fault()
{	return this->soap->fault;
}

const char *LoginCmsSoapBindingProxy::soap_fault_subcode()
{	return ::soap_fault_subcode(this->soap);
}

const char *LoginCmsSoapBindingProxy::soap_fault_string()
{	return ::soap_fault_string(this->soap);
}

const char *LoginCmsSoapBindingProxy::soap_fault_detail()
{	return ::soap_fault_detail(this->soap);
}

int LoginCmsSoapBindingProxy::soap_close_socket()
{	return ::soap_closesock(this->soap);
}

int LoginCmsSoapBindingProxy::soap_force_close_socket()
{	return ::soap_force_closesock(this->soap);
}

void LoginCmsSoapBindingProxy::soap_print_fault(FILE *fd)
{	::soap_print_fault(this->soap, fd);
}

#ifndef WITH_LEAN
#ifndef WITH_COMPAT
void LoginCmsSoapBindingProxy::soap_stream_fault(std::ostream& os)
{	::soap_stream_fault(this->soap, os);
}
#endif

char *LoginCmsSoapBindingProxy::soap_sprint_fault(char *buf, size_t len)
{	return ::soap_sprint_fault(this->soap, buf, len);
}
#endif

int LoginCmsSoapBindingProxy::send_loginCms(const char *soap_endpoint_url, const char *soap_action, _afip1__loginCms *afip1__loginCms)
{
	struct __afip2__loginCms soap_tmp___afip2__loginCms;
	if (soap_endpoint_url != NULL)
		soap_endpoint = soap_endpoint_url;
	if (soap_endpoint == NULL)
		soap_endpoint = "https://wsaahomo.afip.gov.ar/ws/services/LoginCms";
	if (soap_action == NULL)
		soap_action = "";
	soap_tmp___afip2__loginCms.afip1__loginCms = afip1__loginCms;
	soap_begin(soap);
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	soap_serialize___afip2__loginCms(soap, &soap_tmp___afip2__loginCms);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___afip2__loginCms(soap, &soap_tmp___afip2__loginCms, "-afip2:loginCms", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___afip2__loginCms(soap, &soap_tmp___afip2__loginCms, "-afip2:loginCms", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	return SOAP_OK;
}

int LoginCmsSoapBindingProxy::recv_loginCms(_afip1__loginCmsResponse &afip1__loginCmsResponse)
{
	afip1__loginCmsResponse.soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	afip1__loginCmsResponse.soap_get(soap, "afip1:loginCmsResponse", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}
/* End of client proxy code */

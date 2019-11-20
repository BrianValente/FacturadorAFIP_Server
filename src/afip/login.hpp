#ifndef LOGIN_H
#define LOGIN_H

class AccessCredentials {
	public:
		std::string token, sign;
		AccessCredentials(std::string t, std::string s) {
			token = t;
			sign = s;
		}
};

namespace Login {
	AccessCredentials* getAccessCredentials();
	AccessCredentials* getCredentialsFromServer();
	std::string generateSignedTicketRequest();
}

#endif

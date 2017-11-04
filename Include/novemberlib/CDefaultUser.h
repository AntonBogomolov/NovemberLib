#ifndef CDEFAULTUSER_H
#define CDEFAULTUSER_H

#include "interfaces/IValidTester.h"

#include <vector>
#include <string>

class CBan;

class CDefaultUser : public IValidTester
{
	public:
		static const int UT_NONE  = 0;
		static const int UT_USER  = 1;
		static const int UT_MODER = 2;
		static const int UT_ADMIN = 3;

		const std::string& getLogin() const;
		const std::string& getPass() const;
		const std::string& getSalt() const;
		const std::string& getCookie() const;
		int getUserId() const;
		int getUserType() const;
		long getLastChange() const;
		long getLastAction() const;

		const std::string& getRemouteAddress() const;
		const std::string& getUserAgent() const;

		bool getIsAccessClosed() const;
		bool getIsUserGuest() const;
		long getReadOnlyBanTime() const;
		const std::vector<CBan*>* getUserBans() const;

		const std::string& getUserKey() const;
		static std::string genUserKey();

		template<class T> static CDefaultUser* createUser()
		{
			return new T();
		}

		static CDefaultUser* createNewUser(const std::string& login, const std::string& pass, const std::string& cookieData, const std::string& remouteAddress, const std::string& userAgent);
		virtual void afterFillUserData();
		virtual void fillUserData(const std::string& cookieData);
		void fillUserDataById(const int userId);
		static CDefaultUser* loginUser(const std::string& login, const std::string& pass);
<<<<<<< HEAD
		static bool isUserExist(const std::string& userKey);
=======
		static bool isUserExist(const std::string userKey);
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311

		CDefaultUser();
		CDefaultUser(const int userId);
		virtual ~CDefaultUser();
	protected:
		static int authorization(const std::string& login, const std::string& pass);

		std::string cookie;
		std::string key;
		std::string pass;
		std::string login;
		std::string salt;
		std::string remouteAddress;
		std::string userAgent;
		int userId;
		int userType;
		long lastChange;
		long lastAction;

		std::vector<CBan*> bans;
	private:
};

#endif // CDEFAULTUSER_H

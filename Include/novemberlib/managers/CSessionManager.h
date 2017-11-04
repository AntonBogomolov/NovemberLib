<<<<<<< HEAD
#ifndef CSESSIONMANAGER_H
#define CSESSIONMANAGER_H

#include "../interfaces/IUncopyable.h"
#include "../CDefaultUser.h"

#include <string>

class CFCGIRequest;

class CSessionManager : public IUncopyable
{
	public:
		typedef CDefaultUser* (*factoryFuncPtr)();
		factoryFuncPtr userCreationFunc;

		virtual bool checkSession(CFCGIRequest* currRequest);

        virtual void logoutUser(CFCGIRequest* currRequest);
		virtual void loginUser(CFCGIRequest* currRequest, const std::string& login, const std::string& pass);
		virtual void registerUser(CFCGIRequest* currRequest, const std::string& login, const std::string& pass);

		bool getIsNeedSessionCheck() const;
		void setIsNeedSessionCheck(const bool value);

		template<typename Type>	void setUserType()
		{
			userCreationFunc = &CDefaultUser::createUser<Type>;
		}

		CSessionManager();
		virtual ~CSessionManager();
	protected:
		bool isNeedSessionCheck;
	private:
};

#endif // CSESSIONMANAGER_H
=======
#ifndef CSESSIONMANAGER_H
#define CSESSIONMANAGER_H

#include "../interfaces/IUncopyable.h"
#include "../CDefaultUser.h"

#include <string>

class CFCGIRequest;

class CSessionManager : public IUncopyable
{
	public:
		typedef CDefaultUser* (*factoryFuncPtr)();
		factoryFuncPtr userCreationFunc;

		virtual bool checkSession(CFCGIRequest* currRequest);

		virtual void loginUser(CFCGIRequest* currRequest, const std::string& login, const std::string& pass);
		virtual void registerUser(CFCGIRequest* currRequest, const std::string& login, const std::string& pass);

		bool getIsNeedSessionCheck() const;
		void setIsNeedSessionCheck(const bool value);

		template<typename Type>	void setUserType()
		{
			userCreationFunc = &CDefaultUser::createUser<Type>;			
		}		

		CSessionManager();
		virtual ~CSessionManager();
	protected:
		bool isNeedSessionCheck;
	private:
};

#endif // CSESSIONMANAGER_H
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311

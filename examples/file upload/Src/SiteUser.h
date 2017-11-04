<<<<<<< HEAD
#pragma once
#include "novemberlib/novemberlib.h"

#include <vector>

class CAttachment;

class CSiteUser : public CDefaultUser
{
public:
	const std::vector<CAttachment*>* getAttachments() const;
	virtual void afterFillUserData();
	void attachFile(const std::string& attachStr);

	CSiteUser();
	virtual ~CSiteUser();
protected:
	std::vector<CAttachment*> attachments;
};

=======
#pragma once
#include "novemberlib/novemberlib.h"

#include <vector>

class CAttachment;

class CSiteUser : public CDefaultUser
{
public:
	const std::vector<CAttachment*>* getAttachments() const;
	virtual void afterFillUserData();
	void attachFile(const std::string& attachStr);

	CSiteUser();
	virtual ~CSiteUser();
protected:
	std::vector<CAttachment*> attachments;
};

>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311

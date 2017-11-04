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


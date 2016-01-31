/*
 * MIME types manager
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */

#ifndef H_MIMETYPE
#define H_MIMETYPE

#include "../interfaces/ITemplateSingleton.h"

#include <map>

class MIMEType : public ITemplateSingleton<MIMEType>
{
  public:
	std::string get(const std::string& filname) const;
	void set(const std::string& ext, const std::string& type);

	const std::string getFileExt(const std::string& fileMimeType);
	const std::string getFileType(const std::string& fileMimeType);

	virtual ~MIMEType();
  private:
    std::map<std::string, std::string> exts;
    std::string default_type;

	friend MIMEType* ITemplateSingleton::getInstance();
    MIMEType();
};

#endif

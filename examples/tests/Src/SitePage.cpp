#include "SitePage.h"

#include "Attach.h"

CSitePage::CSitePage(const std::string name, const CFCGIRequest* currRequest) : CPage(name, currRequest)
{

}

CSitePage::~CSitePage()
{

}

std::string CSitePage::buildNavBar(const int currCat)
{
	CTemplateHelper* templateManager = CTemplateHelper::getInstance();
	std::map<std::string, std::string> params;

	const CHTMLTemplate* navBarTemplate = templateManager->findTemplate("navBar");
	if (navBarTemplate == NULL) return "Missing menu template";

	bool isCat0Active = currCat == 0;
	bool isCat1Active = !isCat0Active;

	params["{isCat0Active}"] = isCat0Active ? "true" : "false";
	params["{isCat1Active}"] = isCat1Active ? "true" : "false";

	return  navBarTemplate->getHTMLData(&params);
}

std::string CSitePage::buildAttachments(const std::vector<CAttachment*>* attachments)
{
	CTemplateHelper* templateManager = CTemplateHelper::getInstance();
	std::map<std::string, std::string> params;
	std::string tmpStr = "";

	const CHTMLTemplate* attachTemplate = NULL;
	attachTemplate = templateManager->findTemplate("attach");
	if (attachTemplate == NULL) return "Missing attach template";

	for (unsigned int i = 0; i < attachments->size(); i++)
	{
		CAttachment* currAttach = (*attachments)[i];
		bool isImagePrev = true;	
		bool isVidPrev = false;
		bool isAudPrev = false;
		std::string MIME = currAttach->getFileMIMEType();

		if (currAttach->getFileType() == "video")
		{
			if (MIME == "video/webm" || MIME == "video/mp4" || MIME == "video/mpeg")
			{
				params["{SOURCE}"] = replaceAll(currAttach->getFilePath(), "//", "/");
				params["{VIDTYPE}"] = MIME;
				isImagePrev = false;
				isAudPrev = false;
				isVidPrev = true;
			}
		}
		if (currAttach->getFileType() == "audio")
		{		
			params["{SOURCE}"] = replaceAll(currAttach->getFilePath(), "//", "/");
			params["{AUDTYPE}"] = MIME;
			isImagePrev = false;
			isVidPrev = false;
			isAudPrev = true;	
		}

		params["{FILEPATH}"] = replaceAll(currAttach->getFilePath(), "//", "/");
		params["{FILETYPE}"] = currAttach->getFileType();
		params["{FILESIZE}"] = valueToString(currAttach->getFileSize() / 1024);
		params["{FILENAME}"] = currAttach->getFileName();
		params["{FILEID}"]   = valueToString(currAttach->getFileId());
		params["{isIMAGE}"] = isImagePrev ? "true" : "false";
		params["{isVIDEO}"] = isVidPrev ? "true" : "false";
		params["{isAUDIO}"] = isAudPrev ? "true" : "false";
		params["{IMAGEPREVIEWPATH}"] = replaceAll(currAttach->getFilePreviewPath(), "//", "/");
		params["{IMAGEPREVIEWDESCRIPTION}"] = "img";
		params["{IMAGEPREVIEWWIDTH}"] = valueToString(currAttach->getFilePreviewWidth());
		params["{IMAGEPREVIEWHEIGHT}"] = valueToString(currAttach->getFilePreviewHeight());
		params["{IMAGEWIDTH}"] = valueToString(currAttach->getFileWidth());
		params["{IMAGEHEIGHT}"] = valueToString(currAttach->getFileHeight());

		tmpStr += attachTemplate->getHTMLData(&params);
		params.clear();
	}

	return tmpStr;
}
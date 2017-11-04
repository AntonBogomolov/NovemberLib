#include "Attach.h"

#include "novemberlib/novemberlib.h"

CAttachment::CAttachment(const std::string filePath, const long fileSize, const int width, const int height, const int fileId)
{
	this->filePath = filePath;
	this->width = width;
	this->height = height;
	this->fileSize = fileSize;
	this->fileId = fileId;

	previewWidth = 128;
	previewHeight = 128;
	if (this->width == 0) this->width = 128;
	if (this->height == 0) this->height = 128;
	
	fileType = "none";
	filePreviewPath = "img//previewicons//file.png";

	size_t fileNameStartPos = filePath.find_last_of("//");
	if (fileNameStartPos != std::string::npos)
	{
		fileName = filePath.substr(fileNameStartPos + 1, filePath.length() - 1);
	}
	
	size_t dotPos = filePath.find_last_of(".");
	if (dotPos == std::string::npos)
	{
		fileExtention = "none";
	}
	else
	{
		fileExtention = filePath.substr(dotPos + 1, filePath.length() - 1);
	}
	fileMIME = MIMEType::getInstance()->get(fileExtention);

	initPreviewImage(filePath);
}

CAttachment::~CAttachment()
{
	//dtor
}

std::vector<CAttachment*>* CAttachment::processAttachmentString(const std::string& attachmentString)
{
	std::vector<CAttachment*>* attachments = new std::vector<CAttachment*>();
	std::vector<std::string> strs;

	split(strs, attachmentString, ";");
	for (unsigned int i = 0; i < strs.size(); i += 5)
	{
		if (strs[i].length() > 0 && strs[i + 1].length() > 0 && strs[i + 2].length() > 0 && strs[i + 3].length() > 0 && strs[i + 4].length() > 0)
		{
			std::string filePath = strs[i];
			long fileSize = 0;
			int width = 0;
			int height = 0;
			int fileId = 0;
			try
			{
				fileSize = std::stoi(strs[i + 1]);
				width = std::stoi(strs[i + 2]);
				height = std::stoi(strs[i + 3]);
				fileId = std::stoi(strs[i + 4]);
			}
			catch (...)
			{
				continue;
			}

			attachments->push_back(new CAttachment(strs[i], fileSize, width, height, fileId));
		}
	}

	return attachments;
}

std::string CAttachment::createAttachmentString(const std::vector<CAttachment*>* attVector)
{
	std::string attachmentString = "";
	const unsigned int vectorSize = attVector->size();
	for (unsigned int i = 0; i < vectorSize; i++)
	{
		CAttachment* currAttach = (*attVector)[i];
		attachmentString += currAttach->getFilePath() + ";" + valueToString(currAttach->getFileSize()) + ";" + valueToString(currAttach->getFileWidth()) + ";" + valueToString(currAttach->getFileHeight()) + ";" + valueToString(currAttach->getFileId());
		if (i < vectorSize - 1) attachmentString += ";";
	}
	return attachmentString;
}

std::string CAttachment::getFilePath() const
{
	return filePath;
}

std::string CAttachment::getFilePreviewPath() const
{
	return filePreviewPath;
}

std::string CAttachment::getFileType() const
{
	return fileType;
}

std::string CAttachment::getFileName() const
{
	return fileName;
}

std::string CAttachment::getFileExtention() const
{
	return fileExtention;
}

std::string CAttachment::getFileMIMEType() const
{
	return fileMIME;
}

int CAttachment::getFilePreviewWidth() const
{
	return previewWidth;
}

int CAttachment::getFilePreviewHeight() const
{
	return previewHeight;
}

int CAttachment::getFileWidth() const
{
	return width;
}

int CAttachment::getFileHeight() const
{
	return height;
}

int CAttachment::getFileId() const
{
	return fileId;
}

long CAttachment::getFileSize() const
{
	return fileSize;
}

void CAttachment::calcPreviewDimension(const int imgWidth, const int imgHeight, int &prevWidth, int &prevHeight)
{
	CConfigHelper* settingsManager = CConfigHelper::getInstance();

	int maxImgDim = imgWidth;
	if (imgHeight > maxImgDim) maxImgDim = imgHeight;

	if (maxImgDim == 0) return;
	if (maxImgDim < settingsManager->getIntParamValue("maxPreviewDimension", 200))
	{
		prevWidth = imgWidth;
		prevHeight = imgHeight;
		return;
	}

	prevWidth = (int)((float)imgWidth / maxImgDim * settingsManager->getIntParamValue("maxPreviewDimension", 200));
	prevHeight = (int)((float)imgHeight / maxImgDim * settingsManager->getIntParamValue("maxPreviewDimension", 200));
}

void CAttachment::initPreviewImage(const std::string filePath)
{
	std::string fileExtention = "none";
	std::string fileName = "";

	size_t dotPos = filePath.find_last_of(".");
	if (dotPos != std::string::npos)
	{
		fileExtention = filePath.substr(dotPos + 1, filePath.length() - 1);
	}
	size_t fileNameStartPos = filePath.find_last_of("//");
	if (fileNameStartPos != std::string::npos)
	{
		fileName = filePath.substr(fileNameStartPos + 1, filePath.length() - 1);
	}
	
	if (fileExtention == "jpg" || fileExtention == "jpeg")
	{
		calcPreviewDimension(width, height, previewWidth, previewHeight);
		filePreviewPath = filePath;
		fileType = "image";
		return;
	}
	if (fileExtention == "png")
	{
		calcPreviewDimension(width, height, previewWidth, previewHeight);
		filePreviewPath = filePath;
		fileType = "image";
		return;
	}
	if (fileExtention == "bmp")
	{
		calcPreviewDimension(width, height, previewWidth, previewHeight);
		filePreviewPath = filePath;
		fileType = "image";
		return;
	}
	if (fileExtention == "gif")
	{
		calcPreviewDimension(width, height, previewWidth, previewHeight);
		filePreviewPath = filePath;
		fileType = "image";
		return;
	}	

	previewWidth = 128;
	previewHeight = 128;
	filePreviewPath = "img//previewicons//file.png";
}
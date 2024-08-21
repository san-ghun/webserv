#include "RequestParser.hpp"

////////////////////////////////////////////////////////////////////////////////
/// BodyCOnatiner
////////////////////////////////////////////////////////////////////////////////

BodyContainer::BodyContainer()
	: body(NULL), bodyType(Body::NONE) {}

BodyContainer::~BodyContainer()
{
	delete body;
}

void BodyContainer::initRaw()
{
	clear();
	body = new Raw();
	bodyType = Body::RAW;
}

void BodyContainer::initRaw(const std::string& rawData) {
	clear();
	body = new Raw(rawData);
	bodyType = Body::RAW;
}
void BodyContainer::initChunked()
{
	clear();
	body = new Chunked();
	bodyType = Body::CHUNKED;
}

void BodyContainer::initFormData()
{
	clear();
	body = new FormData();
	bodyType = Body::FORM_DATA;
}

void BodyContainer::initFormData(const std::string& boundary)
{
	clear();
	FormData* formData = new FormData();
	formData->setBoundary(boundary);
	body = formData;
	bodyType = Body::FORM_DATA;
}

void BodyContainer::clear()
{
	delete body;
	body = NULL;
	bodyType = Body::NONE;
}

bool BodyContainer::empty() const
{
	return (body == NULL);
}

Body::Type BodyContainer::getType() const 
{
	return (bodyType);
}

Raw* BodyContainer::getRaw() const
{
	if (bodyType == Body::RAW)
		return (static_cast<Raw*>(body));
	return (NULL);
}

Chunked* BodyContainer::getChunked() const
{
	if (bodyType == Body::CHUNKED)
		return (static_cast<Chunked*>(body));
	return (NULL);
}

FormData* BodyContainer::getFormData() const
{
	if (bodyType == Body::FORM_DATA)
		return (static_cast<FormData*>(body));
	return (NULL);
}


////////////////////////////////////////////////////////////////////////////////
/// Raw
////////////////////////////////////////////////////////////////////////////////

Raw::Raw()
	: _data("")
{
}

Raw::Raw(const std::string& data)
	: _data(data)
{
}

void	Raw::setData(const std::string& data)
{
	_data = data;
}

std::string	Raw::getData() const
{
	return (_data);
}

std::string Raw::toString() const
{
	return (_data);
}

Body::Type	Raw::getType() const
{
	return( Body::RAW);
}

////////////////////////////////////////////////////////////////////////////////
/// Chunked
////////////////////////////////////////////////////////////////////////////////

ChunkSegment::ChunkSegment()
	:  _data(""), _size(0), _isEOF(false)
{
}

ChunkSegment::ChunkSegment(const std::string& chunkData, const size_t chunkSize)
	: _data(chunkData), _size(chunkSize), _isEOF(false)
{
}

size_t	ChunkSegment::parseSize(const std::string& line)
{
	size_t readedSize;
	std::stringstream ss;

	ss << std::hex << removeCRLF(line);
	ss >> readedSize;
	return (readedSize);
}

std::string ChunkSegment::parseData(const std::string& line)
{
	return (removeCRLF(line));
}

std::string	ChunkSegment::getData() const
{
	return (_data);
}

size_t	ChunkSegment::getSize() const
{
	return (_size);
}

bool	ChunkSegment::getIsEOF() const
{
	return (_isEOF);
}

void	ChunkSegment::setData(const std::string& chunkData)
{
	_data = chunkData;
}

void	ChunkSegment::setSize(size_t chunkSize)
{
	_size = chunkSize;
}

void	ChunkSegment::setEOF()
{
	_isEOF = true;
}

////////////////////////////////////////////////////////////////////////////////

Chunked::Chunked()
	: _isComplete(false)
{
}

Chunked::Chunked(const ChunkSegment& chunk)
	: _isComplete(false)
{
	addChunkSegment(chunk);
}

void	Chunked::addChunkSegment(const ChunkSegment& chunk)
{
	_datas.push_back(chunk);
}

void	Chunked::setComplete()
{
	_isComplete = true;
}

std::string	Chunked::assembleBody() const
{
	std::string	body;

	for (std::vector<ChunkSegment>::const_iterator it = _datas.begin(); it != _datas.end(); ++it)
		body += it->getData();
	return (body);
}

std::string Chunked::toString() const
{
	return (assembleBody());
}

Body::Type	Chunked::getType() const
{
	return (Body::CHUNKED);
}

////////////////////////////////////////////////////////////////////////////////
/// FormDataPart
////////////////////////////////////////////////////////////////////////////////

FormDataEntry::FormDataEntry()
	: _name(""), _filename(""), _contentType(""), _data(""), _isProcessed(false)
{
}

FormDataEntry::FormDataEntry(const std::string& partName, const std::string& partData,
								const std::string& partFilename, const std::string& partContentType)
	: _name(partName), _filename(partFilename), _contentType(partContentType), _data(partData), _isProcessed(false)
{
}

FormDataEntry::FormDataEntry(const FormDataEntry& formData)
	: _name(formData._name), _filename(formData._filename), _contentType(formData._contentType),
		_data(formData._data), _isProcessed(formData._isProcessed)
{
}

////////////////////////////////////////////////////////////////////////////////

void	FormDataEntry::handleContentDisposition(const std::string& line)
{
	std::string::size_type pos = line.find("Content-Disposition: ");
	if (pos != std::string::npos)
	{
		std::string				contentDisposition = line.substr(pos + 21);
		std::string::size_type	namePos = contentDisposition.find("name=\"");
		std::string::size_type	filenamePos = contentDisposition.find("filename=\"");
		std::string::size_type	endPos = contentDisposition.find("\"", namePos + 6);

		if (namePos != std::string::npos)
			_name = contentDisposition.substr(namePos + 6, endPos - namePos - 6);
		if (filenamePos != std::string::npos)
		{
			std::string::size_type endPos = contentDisposition.find("\"", filenamePos + 10);
			_filename = contentDisposition.substr(filenamePos + 10, endPos - filenamePos - 10);
		}
	}
}

void	FormDataEntry::handleContentType(const std::string& line)
{
	std::string::size_type pos = line.find("Content-Type: ");
	if (pos != std::string::npos)
	{
		std::string contentType = line.substr(pos + 14);
		setContentType(contentType);
	}
}

////////////////////////////////////////////////////////////////////////////////


std::string	FormDataEntry::getName() const
{
	return (_name);
}

std::string	FormDataEntry::getFilename() const
{
	return (_filename);
}

std::string	FormDataEntry::getContentType() const
{
	return (_contentType);
}

std::string	FormDataEntry::getData() const
{
	return (_data);
}

bool	FormDataEntry::getIsProcessed() const
{
	return (_isProcessed);
}

////////////////////////////////////////////////////////////////////////////////

void	FormDataEntry::setFilename(const std::string& partFilename)
{
	_filename = partFilename;
}

void	FormDataEntry::setContentType(const std::string& partContentType)
{
	_contentType = partContentType;
}

void	FormDataEntry::setName(const std::string& partName)
{
	_name = partName;
}

void	FormDataEntry::setData(const std::string& partData)
{
	_data = partData;
}

void	FormDataEntry::setProcessed()
{
	_isProcessed = true;
}

////////////////////////////////////////////////////////////////////////////////

FormData::FormData()
	: _datas()
{
}

FormData::FormData(const FormDataEntry& formData)
{
	_datas.push_back(formData);
}

void	FormData::addFormDataEntry(const FormDataEntry& formData)
{
	_datas.push_back(formData);
}

void	FormData::setBoundary(const std::string& partBoundary)
{
	_boundary = partBoundary;
}

std::string	FormData::getBoundary() const
{
	return (_boundary);
}

std::string FormData::toString() const // FIXME: sample code need to fix (auto, logic ... )
{ 
	std::stringstream ss;
	for (const auto& entry : _datas)
	{
		ss << "Name: " << entry.getName() << "\n";
		ss << "Filename: " << entry.getFilename() << "\n";
		ss << "ContentType: " << entry.getContentType() << "\n";
		ss << "Data: " << entry.getData() << "\n";
		ss << "------\n";
	}
	return (ss.str());
}

Body::Type FormData::getType() const
{
	return (Body::FORM_DATA);
}
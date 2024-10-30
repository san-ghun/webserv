
#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include <string>
#include <vector>
#include <sstream>
#include "Util.hpp"

////////////////////////////////////////////////////////////////////////////////
/// Raw
////////////////////////////////////////////////////////////////////////////////

struct Raw {
	std::string	_data;

	Raw();
	Raw(const std::string& rawData);

	void		setData(const std::string& rawData);
	std::string	getData() const;
};

////////////////////////////////////////////////////////////////////////////////
/// Chunked
////////////////////////////////////////////////////////////////////////////////

struct ChunkSegment {
	std::string	_data;
	size_t		_size;
	bool		_isEOF;

	ChunkSegment();
	ChunkSegment(const std::string& chunkData, const size_t chunkSize);

	std::string	getData() const;
	size_t		getSize() const;
	void		setSize(size_t chunkSize);
	void		setData(const std::string& chunkData);
	size_t		parseSize(const std::string& line);
	std::string parseData(const std::string& line);
};

struct Chunked {
	std::vector<ChunkSegment>	_datas;
	bool						_isComplete;

	Chunked();
	Chunked(const ChunkSegment& segment);

	void			addChunkSegment(const ChunkSegment& segment);
	void			setComplete();
	std::string		assembleBody() const;
};

////////////////////////////////////////////////////////////////////////////////
/// FormData
////////////////////////////////////////////////////////////////////////////////

struct FormDataEntry {
	std::string	_name;
	std::string	_filename;
	std::string	_contentType;
	std::string	_data;
	bool		_isProcessed;

	FormDataEntry();
	FormDataEntry(const std::string& partName, const std::string& partData,
					const std::string& partFilename, const std::string& partContentType);
	FormDataEntry(const FormDataEntry& entry);

	std::string	getName() const;
	std::string	getFilename() const;
	std::string	getContentType() const;
	std::string	getData() const;
	bool		getIsProcessed() const;

	void		setFilename(const std::string& partFilename);
	void		setContentType(const std::string& partContentType);
	void		setName(const std::string& partName);
	void		setData(const std::string& partData);
	void		setProcessed();

	void		handleContentDisposition(const std::string& line);
	void		handleContentType(const std::string& line);
	// void		handleContentTransferEncoding(const std::string& line);
};


struct FormData {

	std::vector<FormDataEntry>	_datas;
	std::string					_boundary;

	FormData();
	FormData(const FormDataEntry& entry);

	std::string getBoundary() const;
	void		setBoundary(const std::string& partBoundary);
	void		addFormDataEntry(const FormDataEntry& entry);
};


////////////////////////////////////////////////////////////////////////////////
/// Body
////////////////////////////////////////////////////////////////////////////////

enum Type {
	RAW,
	CHUNKED,
	FORM_DATA,
	NONE
};

struct Body
{
	Type	bodyType;

	union {
		Raw*		raw;
		Chunked*	chunked;
		FormData*	formData;
	};

	Body();
	~Body();

	void			clear();
	bool			empty() const;

	void			initRaw();
	void			initRaw(const std::string& rawData);
	void			initChunked();
	void			initFormData();
	void			initFormData(const std::string& boundary);

	Raw				getRaw() const;
	Chunked			getChunked() const;
	FormData		getFormData() const;

};

#endif // REQUESTPARSER_HPP
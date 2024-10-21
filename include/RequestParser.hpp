
#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include <string>
#include <vector>
#include <sstream>
#include "Util.hpp"


////////////////////////////////////////////////////////////////////////////////
/// Body (Base Class)
////////////////////////////////////////////////////////////////////////////////

class Body {
public:
	enum Type {
		RAW,
		CHUNKED,
		FORM_DATA,
		NONE
	};

	virtual ~Body() {}
	virtual std::string toString() const = 0;
	virtual Type getType() const = 0;

	// @brief Initialize the body container with the appropriate body type
	virtual void initialize(BodyContainer& container) const = 0;
};


////////////////////////////////////////////////////////////////////////////////
/// Raw (Derived Class)
////////////////////////////////////////////////////////////////////////////////

class Raw : public Body {
private:
	std::string	_data;

public:
	Raw() {}
	Raw(const std::string& data);

	void setData(const std::string& data);
	std::string getData() const;

	std::string toString() const;	// override	
	Type getType() const;			// override
};


////////////////////////////////////////////////////////////////////////////////
/// Chunked (Derived Class)
////////////////////////////////////////////////////////////////////////////////

class ChunkSegment 
{
private:
	std::string	_data;
	size_t		_size;
	bool		_isEOF;

public:
	ChunkSegment();
	ChunkSegment(const std::string& data, const size_t size);

	std::string	getData() const;
	size_t		getSize() const;
	bool		getIsEOF() const;

	void		setSize(size_t size);
	void		setData(const std::string& data);
	void		setEOF();

	size_t parseSize(const std::string& line);
	std::string parseData(const std::string& line);
};

class Chunked : public Body
{
private:
	std::vector<ChunkSegment>	_datas;
	bool						_isComplete;

public:
	Chunked();
	Chunked(const ChunkSegment& segment);

	void addChunkSegment(const ChunkSegment& segment);
	void setComplete();
	std::string assembleBody() const;

	std::string toString() const;	// override
	Type getType() const;			// override
	Chunked getChunked() const;
};

////////////////////////////////////////////////////////////////////////////////
/// FormData (Derived Class)
////////////////////////////////////////////////////////////////////////////////

class FormDataEntry {
private:
	std::string	_name;
	std::string	_filename;
	std::string	_contentType;
	std::string	_data;
	bool		_isProcessed;

public:
	FormDataEntry();
	FormDataEntry(const std::string& name, const std::string& data,
				  const std::string& filename, const std::string& contentType);
	FormDataEntry(const FormDataEntry& entry);

	std::string	getName() const;
	std::string	getFilename() const;
	std::string	getContentType() const;
	std::string	getData() const;
	bool		getIsProcessed() const;

	void		setFilename(const std::string& filename);
	void		setContentType(const std::string& contentType);
	void		setName(const std::string& name);
	void		setData(const std::string& data);
	void		setProcessed();

	void		handleContentDisposition(const std::string& line);
	void		handleContentType(const std::string& line);
	// void handleContentTransferEncoding(const std::string& line);
};

class FormData : public Body {
private:
	std::vector<FormDataEntry>	_datas;
	std::string					_boundary;

public:
	FormData();
	FormData(const FormDataEntry& entry);

	std::string getBoundary() const;
	void setBoundary(const std::string& boundary);
	void addFormDataEntry(const FormDataEntry& entry);

	std::string toString() const; 	// override
	Body::Type	getType() const;	// override
};

////////////////////////////////////////////////////////////////////////////////
/// Body Container (Formerly Union in Body Struct)
////////////////////////////////////////////////////////////////////////////////

/// FIXME: delete :)... Body container is not necessary, only Body class is needed
class BodyContainer {
public:
	BodyContainer();
	~BodyContainer();

	/// FIXME: initialize(); cast to appropriate body type
	void		initRaw();
	void		initChunked();
	void		initFormData();

	void		initRaw(const std::string& rawData);
	void		initFormData(const std::string& boundary);

	void		clear();
	bool		empty() const;

	Body::Type	getType() const;

	/// FIXME: getBody(); cast to appropriate body type
	Raw*		getRaw() const;
	Chunked*	getChunked() const;
	FormData*	getFormData() const;

	private:
		Body*		body;
		Body::Type	bodyType;
};

#endif // REQUESTPARSER_HPP

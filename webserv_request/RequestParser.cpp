#ifndef REQUESTPARSER_HPP
# define REQUESTPARSER_HPP

# include <stdio.h>
# include <unistd.h>
# include <map>
# include "utils.hpp"

# define GET_LINE_BUF 1024
# define ERROR -1
# define SUCCESS 0

class RequestParser
{
public:
	int _socket_fd;
	FILE* _socket_read;
	FILE* _socket_write;

	std::string _method;
	std::string _path;
	std::string _protocol;
	std::map<std::string, std::string> _request_header;
	std::string _request_body;

public:
	RequestParser(int socket_fd);
	~RequestParser();
	int getRequest();

public:
	int getRequestLine();
	int getRequestHeader();
	int getRequestBody();
};


RequestParser::RequestParser(int socket_fd)
	: _socket_fd(socket_fd), _socket_read(fdopen(socket_fd, "r")), _socket_write(fdopen(dup(socket_fd), "w"))
{
	//
}

RequestParser::~RequestParser()
{
	// fclose(_socket_read);
	// fclose(_socket_write);
	// close(_socket_fd);
}

int RequestParser::getRequest()
{
	if (getRequestLine() == ERROR)
		return ERROR;
	if (getRequestHeader() == ERROR)
		return ERROR;
	if (this->_method == "POST")
		getRequestBody();
	return SUCCESS;
}

int RequestParser::getRequestLine()
{
	char line[GET_LINE_BUF];
	std::string str_line;
	std::vector<std::string> request_line;
	size_t pos;

	fgets(line, GET_LINE_BUF, this->_socket_read);
	str_line = std::string(line);
	if (str_line == "" || str_line == "\r\n")
		return ERROR;

	request_line = split(str_line, " ");
	if (request_line.size() != 3)
		return ERROR;

	this->_method = request_line[0];
	this->_path = request_line[1];
	this->_protocol = request_line[2];
	
	if (_method != "GET" && _method != "POST" && _method != "DELETE")
		return ERROR;
	if ((pos = _protocol.find("HTTP/")) == std::string::npos)
		return ERROR;

	return SUCCESS;
}

int RequestParser::getRequestHeader()
{
	char line[GET_LINE_BUF];
	std::string str_line, key, value;
	std::vector<std::string> key_value;

	fgets(line, GET_LINE_BUF, this->_socket_read);
	str_line = std::string(line);
	while (str_line != "" && str_line != "\r\n")
	{
		key_value = split(std::string(line), ": ");

		if (key_value.size() != 2) {
			_request_header.clear();
			return ERROR;
		}

		key = key_value[0];
		value = key_value[1].replace(key_value[1].find("\r\n"), 2, "\0");
		_request_header.insert(std::make_pair<std::string, std::string>(key, value));

		fgets(line, GET_LINE_BUF, this->_socket_read);
		str_line = std::string(line);
	}
	if (_request_header.find("Host") == _request_header.end())
		return ERROR;
	
	return SUCCESS;
}

int RequestParser::getRequestBody()
{
	char line[GET_LINE_BUF];

	while (fgets(line, GET_LINE_BUF, this->_socket_read) != NULL)
	{
		_request_body += (std::string(line) + "\n");
	}
	return SUCCESS;
}

#endif
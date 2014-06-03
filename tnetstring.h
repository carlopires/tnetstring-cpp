/*
 * tnetstring.h
 *
 *  Created on: 16/05/2014
 *      Author: Carlo Pires <carlopires@gmail.com>
 *     Licence: MIT
 *         URL: https://github.com/carlopires/tnetstring-cpp
 */

#ifndef TNETSTRING_H_
#define TNETSTRING_H_

#include <list>
#include <map>

namespace tns {

enum tnetstring_type {
  UNDEFINED = '\0',
  STRING    = ',',
  INTEGER   = '#',
  FLOAT     = '^',
  BOOLEAN   = '!',
  LIST      = ']',
  DICT      = '}',
  NONE      = '~'
};

class tnetstring;

typedef std::list<tnetstring> tnetstring_list;
typedef std::map<std::string, tnetstring> tnetstring_dict;

class tnetstring {
public:
	tnetstring();
	tnetstring(const char* val);
	tnetstring(const std::string& val);
	tnetstring(const int val);
	tnetstring(const long val);
	tnetstring(const float val);
	tnetstring(const double val);
	tnetstring(const bool val);
	tnetstring(const tnetstring& val);
	tnetstring(const tnetstring_type val);
	virtual ~tnetstring();

	std::string dump() const;
	bool parse(char* str, size_t str_size);
	bool parse(std::istream& istr);

	tnetstring& operator=(const tnetstring& val);
	tnetstring& operator<<(const tnetstring& val);
	tnetstring& operator[](const std::string key);
	tnetstring& operator[](const int index);
    friend std::ostream& operator<< (std::ostream& stream, const tnetstring& val);

    const std::string* str_val();
    const long* lint_val();
    const double* double_val();
    const bool* bool_val();
    const tnetstring_list* list_val();
    const tnetstring_dict* dict_val();
    const size_t size();
    void precision(std::streamsize prec);
private:
	tnetstring_type type = UNDEFINED;
	void* value = NULL;
	std::streamsize prec;
	void free_value();
	void assign(const tnetstring& val);
	bool parse_payload(std::string& payload, char& payload_type, std::istream& str);
};

class TnetstringError : public std::exception {
private:
	std::string error_msg;
public:
	TnetstringError(const std::string &error) throw();
	~TnetstringError() throw();
	const char *what() const throw();
};

template <typename char_type>
struct istreambuf : public std::basic_streambuf<char_type, std::char_traits<char_type> > {
    istreambuf(char_type* buffer, std::streamsize buffer_size) {
        this->setg(0, buffer, buffer + buffer_size);
    }
};

template <typename char_type>
struct ostreambuf : public std::basic_streambuf<char_type, std::char_traits<char_type> > {
    ostreambuf(char_type* buffer, std::streamsize buffer_size) {
        this->setp(buffer, buffer + buffer_size);
    }
};

extern const tnetstring EMPTY;

} /* namespace tns */

#endif /* TNETSTRING_H_ */

/*
 * tnetstring.cpp
 *
 *  Created on: 16/05/2014
 *      Author: Carlo Pires <carlopires@gmail.com>
 *     Licence: MIT
 *         URL: https://github.com/carlopires/tnetstring-cpp
 */

#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <streambuf>
#include <string>
#include <limits>
#include "tnetstring.h"

using namespace std;

namespace tns {

// CONSTRUCTORS

tnetstring::tnetstring() :
	prec(numeric_limits<double>::digits10 + 1) { }

tnetstring::tnetstring(const tnetstring_type type) :
	prec(numeric_limits<double>::digits10 + 1) {
	this->type = type;
	switch (type) {
		case STRING:
			value = new string;
			break;
		case INTEGER:
			value = new long(0);
			break;
		case FLOAT:
			value = new double(0);
			break;
		case BOOLEAN:
			value = new bool(0);
			break;
		case LIST:
			value = new tnetstring_list;
			break;
		case DICT:
			value = new tnetstring_dict;
			break;
		case NONE:
		case UNDEFINED:
			value = NULL;
	}
}

tnetstring::tnetstring(const char* val) :
	type(STRING),
	prec(numeric_limits<double>::digits10 + 1) {
	value = new string(val);
}

tnetstring::tnetstring(const string& val) :
	type(STRING),
	prec(numeric_limits<double>::digits10 + 1) {
	value = new string(val);
}

tnetstring::tnetstring(const int val) :
	type(INTEGER),
	prec(numeric_limits<double>::digits10 + 1) {
	value = new long(val);
}

tnetstring::tnetstring(const long val) :
	type(INTEGER),
	prec(numeric_limits<double>::digits10 + 1) {
	value = new long(val);
}

tnetstring::tnetstring(const float val) :
	type(FLOAT),
	prec(numeric_limits<double>::digits10 + 1) {
	value = new double(val);
}

tnetstring::tnetstring(const double val) :
	type(FLOAT),
	prec(numeric_limits<double>::digits10 + 1) {
	value = new double(val);
}

tnetstring::tnetstring(const bool val) :
	type(BOOLEAN),
	prec(numeric_limits<double>::digits10 + 1) {
	value = new bool(val);
}

tnetstring::tnetstring(const tnetstring& val) :
	prec(numeric_limits<double>::digits10 + 1) {
	assign(val);
}

// METHODS

void tnetstring::precision(streamsize prec) {
	this->prec = prec;
}

void tnetstring::assign(const tnetstring& val) {
	type = val.type;
	switch (type) {
		case STRING:
			value = new string(*(string*) val.value);
			break;
		case INTEGER:
			value = new long(*(long*) val.value);
			break;
		case FLOAT:
			value = new double(*(double*) val.value);
			break;
		case BOOLEAN:
			value = new bool(*(bool*) val.value);
			break;
		case LIST: {
				tnetstring_list* newl = new tnetstring_list;
				tnetstring_list* l = (tnetstring_list*)val.value;
				for (auto& it: *l) {
					newl->push_back(tnetstring(it));
				}
				value = newl;
			}
			break;
		case DICT: {
				tnetstring_dict* newd = new tnetstring_dict;
				tnetstring_dict* d = (tnetstring_dict*)val.value;
				for (auto& it: *d) {
					(*newd)[it.first] = tnetstring(it.second);
				}
				value = newd;
			}
			break;
		case NONE:
		case UNDEFINED:
			value = NULL;
	}
}

string tnetstring::dump() const {
	stringstream res;
	switch (type) {
		case STRING: {
				string* str = (string*)value;
				res << str->length() << ':' << *str << ',';
			}
			break;
		case INTEGER: {
				stringstream str;
				str << *(long*)value;
				res << str.tellp() << ':' << str.str() << '#';
			}
			break;
		case FLOAT: {
				stringstream str;
				str.precision(prec);
				str << fixed << *(double*)value;
				res << str.tellp() << ':' << str.str() << '^';
			}
			break;
		case BOOLEAN: {
				string str((bool*)value ? "true" : "false");
				res << str.length() << ':' << str << '!';
			}
			break;
		case LIST: {
				tnetstring_list* l = (tnetstring_list*)value;
				stringstream str;
				for (auto& it: *l) {
					str << it.dump();
				}
				res << str.tellp() << ':' << str.str() << ']';
			}
			break;
		case DICT: {
				tnetstring_dict* d = (tnetstring_dict*)value;
				stringstream str;
				for (auto& it: *d) {
					str << it.first.length() << ':' << it.first << ',';
					str << it.second.dump();
				}
				res << str.tellp() << ':' << str.str() << '}';
			}
			break;
		case NONE:
			res << "0:~";
			break;
		default:
			throw TnetstringError("Cannot dump UNDEFINED tnetstring");
	}
	return res.str();
}

void tnetstring::free_value() {
	switch (type) {
		case STRING:
			delete (string*) value;
			break;
		case INTEGER:
			delete (long*) value;
			break;
		case FLOAT:
			delete (double*) value;
			break;
		case BOOLEAN:
			delete (bool*) value;
			break;
		case LIST:
			delete (tnetstring_list*)value;
			break;
		case DICT:
			delete (tnetstring_dict*) value;
			break;
		case NONE:
		case UNDEFINED:
			break;
	}
	this->type = UNDEFINED;
	this->value = NULL;
}

tnetstring::~tnetstring() {
	this->free_value();
}

tnetstring& tnetstring::operator<<(const tnetstring& val) {
	if (type == LIST) {
		((tnetstring_list*) value)->push_back(val);
		return *this;
	} else
		throw TnetstringError("Invalid list operation: << is valid only for LISTs");
}

tnetstring& tnetstring::operator=(const tnetstring& val) {
	this->free_value();
	this->assign(val);
	return *this;
}

tnetstring& tnetstring::operator[](const string key) {
	if (type == DICT) {
		string new_key(key);
		tnetstring new_val;
		tnetstring_dict* dict_el = ((tnetstring_dict*) value);
		auto it = dict_el->insert(std::pair<string, tnetstring>(new_key, new_val));
		return it.first->second;
	} else
		throw TnetstringError("Invalid list operation: [string] is valid only for DICTs");
}

tnetstring& tnetstring::operator[](const int index) {
	if (type == LIST) {
		int n = 0;
		for (auto& it: *((tnetstring_list*)value)) {
			if (n++ == index)
				return it;
		}
		throw TnetstringError("Invalid index");
	} else
		throw TnetstringError("Invalid list operation: [int] is valid only for LISTs");
}

std::ostream& operator<<(std::ostream& stream, const tnetstring& val) {
	stream << val.dump();
	return stream;
}

const size_t tnetstring::size() {
	switch (type) {
		case STRING:
			return ((string*) value)->length();
		case LIST:
			return ((tnetstring_list*) value)->size();
		case DICT:
			return ((tnetstring_dict*) value)->size();
		case UNDEFINED:
		case INTEGER:
		case FLOAT:
		case BOOLEAN:
		case NONE:
			break;
	}
	throw TnetstringError("Invalid list operation: size is for STRINGs, LISTs or DICTs");
}

// Exposes internal values as pointers

const string* tnetstring::str_val() {
	return (type == STRING) ? (string*) value : NULL;
}

const long* tnetstring::lint_val() {
	return (type == INTEGER) ? (long*) value : NULL;
}

const double* tnetstring::double_val() {
	return (type == FLOAT) ? (double*) value : NULL;
}

const bool* tnetstring::bool_val() {
	return (type == BOOLEAN) ? (bool*) value : NULL;
}

const tnetstring_list* tnetstring::list_val() {
	return (type == LIST) ? (tnetstring_list*) value : NULL;
}

const tnetstring_dict* tnetstring::dict_val() {
	return (type == DICT) ? (tnetstring_dict*) value : NULL;
}

// Parse

bool tnetstring::parse(char* str, size_t str_size) {
	istreambuf<char> str_buf(str, str_size);
	istream istr(&str_buf);
	return this->parse(istr);
}

bool tnetstring::parse(istream& istr) {
	if (type != UNDEFINED)
		throw TnetstringError("Invalid operation: parse is allowed for UNDEFINED tnetstrings only");

	string payload;
	char payload_type;

	if (parse_payload(payload, payload_type, istr)) {
		switch (payload_type) {
		case STRING:
			value = new string(payload);
			break;
		case INTEGER:
			value = new long(stol(payload));
			break;
		case FLOAT:
			value = new double(stod(payload));
			break;
		case BOOLEAN:
			value = new bool(payload == "true");
			break;
		case LIST:
			{
				tnetstring_list* newl = new tnetstring_list;
				istringstream items(payload);
				while (true) {
					tnetstring item;
					if (item.parse(items))
						newl->push_back(item);
					else {
						if (items.eof())
							break;
						else {
							delete newl;
							return false;
						}
					}
				};
				value = newl;
			}
			break;
		case DICT:
			{
				tnetstring_dict* newd = new tnetstring_dict;
				tnetstring key;
				istringstream items(payload);
				while (true) {
					key.free_value();
					if (key.parse(items) && key.type == STRING) {
						tnetstring val;
						if (val.parse(items)) {
							string str_key(*(key.str_val()));
							(*newd)[str_key] = val;
						} else {
							if (items.eof())
								break;
							else {
								delete newd;
								return false;
							}
						}
					} else {
						if (items.eof())
							break;
						else {
							delete newd;
							return false;
						}
					}
				}
				value = newd;
			}
			break;
		case NONE:
			break;
		default:
			return false;
		}
		type = (tnetstring_type)payload_type;
		return true;
	} else
		return false;
}

bool tnetstring::parse_payload(string& payload, char& payload_type, istream& str) {
	size_t len = 0;
	char colon = '\0';

	str >> len;
	if (!str)
		return false;

	str >> colon;
	if (!str || colon != ':')
		return false;

	payload.resize(len);
	str.read(&payload[0], len);

	str >> payload_type;
	if (!str)
		return false;
	else
		return true;
}

// Exceptions

TnetstringError::TnetstringError(const std::string &error) throw(): error_msg(error) {}
TnetstringError::~TnetstringError() throw() {}
const char *TnetstringError::what() const throw() { return error_msg.c_str(); }

} /* namespace tns */

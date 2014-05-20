/*
 * tnetstring_test.cpp
 *
 *  Created on: 17/05/2014
 *      Author: Carlo Pires <carlopires@gmail.com>
 *     Licence: MIT
 *         URL: https://github.com/carlopires/tnetstring-cpp
 */

#include <stdio.h>
#include <iostream>
#include "tnetstring.h"

using namespace std;
using namespace tns;

int main() {
	// basic types
	const char* name = "Carlo Pires";
	const int age = 39;
	const double salary = 55545888.25852;
	const bool is_married = true;

	tnetstring tn_name(name);
	tnetstring tn_age(age);
	tnetstring tn_salary(salary);
	tnetstring tn_is_married(is_married);
	tnetstring tn_none(NONE);

	cout << "string: " << tn_name << endl;
	cout << "string size: " << tn_name.size() << endl;
	cout << "integer: " << tn_age << endl;
	cout << "float: " << tn_salary << endl;
	cout << "boolean: " << tn_is_married << endl;
	cout << "null: " << tn_none << endl;

	// undefined
	tnetstring tn_undefined;
	try {
		cout << "undefined: " << tn_undefined << endl;
	} catch (TnetstringError&) {
		cout << "undefined: ok" << endl;
	}

	// accessing values
	cout << "string: " << *(tn_name.str_val()) << endl;
	cout << "integer: " << *(tn_age.lint_val()) << endl;
	cout << "float: " << fixed << *(tn_salary.double_val()) << endl;
	cout << "boolean: " << *(tn_is_married.bool_val()) << endl;

	// lists
	tnetstring tn_list(LIST);
	tn_list << tn_name << tn_age << tn_salary << tn_is_married << tn_none;
	cout << "list: " << tn_list << endl;

	// sub lists
	tnetstring tn_list1(LIST);
	tn_list1 << tn_list << tn_name << tn_age;
	cout << "sub list: " << tn_list1 << endl;

	// list by index
	cout << "index list: " << tn_list[2] << endl;

	// list size
	cout << "list size: " << tn_list.size() << endl;

	// maps
	tnetstring tn_dict(DICT);
	tn_dict["name"] = tn_name;
	cout << "dict key: " << tn_dict["name"] << endl;
	tnetstring* x = &(tn_dict["name"]);
	cout << "dict ref: " << *x << endl;
	tn_dict["age"] = tn_age;
	tn_dict["salary"] = tn_salary;
	cout << "dict: " << tn_dict << endl;

	// map size
	cout << "dict size: " << tn_dict.size() << endl;

	// maps with lists
	tn_dict["list"] = tn_list1;
	cout << "dict with list: " << tn_dict << endl;

	// clone
	tnetstring dup(tn_name);
	cout << "dup: " << dup << endl;

	// map with sub maps and list
	tnetstring tn_dict2(DICT);
	tn_dict2["submap"] = tn_dict;
	tn_dict2["sublist"] = tn_list;
	tn_dict2["name"] = tn_name;
	cout << "dict with list and maps: " << tn_dict2 << endl;

	// parse string
	{
		string tn_parse = tn_name.dump();
		tnetstring p;
		bool is_parsed = p.parse((char*)tn_parse.c_str(), tn_parse.length());
		cout << "parse string: " << (is_parsed ? *(p.str_val()) : "ERROR") << endl;
	}

	// parse int
	{
		string tn_parse = tn_age.dump();
		tnetstring p;
		bool is_parsed = p.parse((char*)tn_parse.c_str(), tn_parse.length());
		if (is_parsed)
			cout << "parsed int: " << *(p.lint_val()) << endl;
		else
			cout << "parse int: ERROR" << endl;
	}

	// parse float
	{
		string tn_parse = tn_salary.dump();
		tnetstring p;
		bool is_parsed = p.parse((char*)tn_parse.c_str(), tn_parse.length());
		if (is_parsed)
			cout << "parsed float: " << fixed << *(p.double_val()) << endl;
		else
			cout << "parse float: ERROR" << endl;
	}

	// parse bool
	{
		string tn_parse = tn_is_married.dump();
		tnetstring p;
		bool is_parsed = p.parse((char*)tn_parse.c_str(), tn_parse.length());
		if (is_parsed)
			cout << "parsed bool: " << *(p.bool_val()) << endl;
		else
			cout << "parse bool: ERROR" << endl;
	}

	// parse list
	{
		string tn_parse = tn_list1.dump();
		tnetstring p;
		bool is_parsed = p.parse((char*)tn_parse.c_str(), tn_parse.length());
		if (is_parsed) {
			cout << "parsed list: " << p.size() << endl;
			cout << "parsed sub list: " << *(p[0][0].str_val()) << endl;
		} else
			cout << "parse list: ERROR" << endl;
	}

	// parse dict
	{
		string tn_parse(tn_dict.dump());
		tnetstring p;
		bool is_parsed = p.parse((char*)tn_parse.c_str(), tn_parse.length());
		if (is_parsed) {
			cout << "parsed dict: " << p.size() << endl;
			cout << "parsed sub dict: " << *(p["name"].str_val()) << endl;
			cout << "parsed list in dict: " << p["list"][0] << endl;
		} else
			cout << "parse dict: ERROR" << endl;
	}

	return 0;
}

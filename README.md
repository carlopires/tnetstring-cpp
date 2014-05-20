tnetstring-cpp
==============

A tnetstring encoder/decoder with a pythonic interface for C++

---
[tnetstrings](http://tnetstrings.org) is an excelent format to encode/decode data flowing between applications. This project is an implementation for C++ using an interface clean and intuitive.

---

### Encoding

    const char* name = "Carlo Pires";
    const int age = 39;
	const double salary = 55545888.25852;
	const bool is_married = true;

	tnetstring tn_name(name);
	tnetstring tn_age(age);
	tnetstring tn_salary(salary);
	tnetstring tn_is_married(is_married);
	tnetstring tn_none(NONE);

### Using the encoded tnetstring:

#### Serializing
    
    cout << "string: " << tn_name << endl;
	cout << "string size: " << tn_name.size() << endl;
	cout << "integer: " << tn_age << endl;
	cout << "float: " << tn_salary << endl;
	cout << "boolean: " << tn_is_married << endl;
	cout << "null: " << tn_none << endl;

Trying to serialize an undefined tnetstring (with no value) throws an exception:

	tnetstring tn_undefined; // tnetstring has no value
	try {
		cout << "undefined: " << tn_undefined << endl;
	} catch (TnetstringError&) {
		cout << "undefined: ok" << endl;
	}

#### Access tnetstring value

    cout << "string: " << *(tn_name.str_val()) << endl;
	cout << "integer: " << *(tn_age.lint_val()) << endl;
	cout << "float: " << *(tn_salary.double_val()) << endl;
	cout << "boolean: " << *(tn_is_married.bool_val()) << endl;

### Lists

A list is a collection of tnetstrings

	tnetstring tn_list(LIST);
	tn_list << tn_name << tn_age << tn_salary << tn_is_married << tn_none;

Serializing lists:

	cout << "list: " << tn_list << endl;

A list can have sub lists:

	tnetstring tn_list1(LIST);
	tn_list1 << tn_list << tn_name << tn_age;
	cout << "list with sub list: " << tn_list1 << endl;

Lists elements can be accessed using the operator []:

	cout << "element by index in list: " << tn_list[2] << endl; // returns tn_age

List size can be viewed through size method:

	cout << "list size: " << tn_list.size() << endl; // returns 5

### Maps (dictionaries)

A map is a collection of pairs (key, value) where key is a **std::string** and value is a **tnetstring**.

    tnetstring tn_dict(DICT);
	tn_dict["name"] = tn_name;
    tn_dict["age"] = tn_age;
	tn_dict["salary"] = tn_salary;

Serializing maps:

	cout << "dict: " << tn_dict << endl;

Accessing a tnetstring in a map by key:

    cout << "dict element by key: " << tn_dict["name"] << endl;

Map size can be viewed through size method:

    cout << "map size: " << tn_dict.size() << endl; // returns 3

Map can have lists:

	tn_dict["list"] = tn_list1;
	cout << "dict with list: " << tn_dict << endl;

Map can have sub maps and lists (any tnetstring):

    tnetstring tn_dict2(DICT);
    tn_dict2["submap"] = tn_dict;
    tn_dict2["sublist"] = tn_list;
    tn_dict2["name"] = tn_name;
	cout << "dict with list and maps: " << tn_dict2 << endl;

### Decoding (Parse)

To parse a buffer of data of encoded tnetstrings:

    char* buffer = new char[buffer_size];
    ... read buffer: 59:11:Carlo Pires,2:39#25:55545888.2585199996829033^4:true!0:~]
    
	tnetstring p;
	bool is_parsed = p.parse(buffer, buffer_size);
	if (is_parsed) {
        // access parsed tnetstring (a list in this case)
    	cout << "parsed name: " << *(p[0].str_val()) << endl;
    	cout << "parsed age: " << *(p[1].lint_val()) << endl;
    	cout << "parsed sallary: " << *(p[2].double_val()) << endl;
		cout << "parsed is_married: " << *(p[3].bool_val()) << endl;
	} else
		cout << "parse ERROR" << endl;

### More

#### tnetstrings can be copied:
    
    tnetstring name1("Carlo Pires")
    tnetstring name2(name1);

#### Accessing a tnetstring in a map by reference:

	tnetstring* x = &(tn_dict["name"]);
	cout << "dict element by ref: " << *(*x.str_val()) << endl;

### Compiling

There is makefile for *tnetstring_tests.cpp*. Just do a make:
    
    make
    ./tnetstring_tests

### Using

There is no dependences. Just copy the **tnetstring.h** and **tnetstring.cpp** to your project.

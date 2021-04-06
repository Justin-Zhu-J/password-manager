#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <sstream>
#include <stdexcept>
#include ".secrets.h"
#include "password.cpp"
#include <cstdlib> // TODO this might be deprecated, copied from http://www.cplusplus.com/articles/E6vU7k9E/
using namespace std;

const string welcome_text = "Hi! Welcome to Justin's Password Manager.\n";

string prompt_key(void) {
	
	string key;

	key = getpass("Please enter the key.");
	
	if(!check_key(key)) {
	
		cout << "Incorrect Key" << endl;
		return "";
	}
	return key;
}





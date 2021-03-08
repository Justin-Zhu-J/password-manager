#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <stdexcept>
#include ".secrets.h"
#include "copied_code.cpp"
#include <cstdlib> // TODO this might be deprecated, copied from http://www.cplusplus.com/articles/E6vU7k9E/
using namespace std;

const string welcome_text = "Hi! Welcome to Justin's Password Manager.\n";


// Note: this function only supports one parameter
string create_command(string format, string parameter) {
	// +2 is an arbitrary number meant to reduce chances of an out-of-bounds error
	char *temp = new char[format.length() + parameter.length() + 2];
	sprintf(temp, format.c_str(), parameter.c_str());
	//cout << "command: " << temp << endl;
	return temp;
}

string remove_endline(string text) {
	//cout << "text begin: " << text << " text end" << endl;
	return text.substr(0, text.length() - 1);
}

// Note: this function only supports two parameters
string create_cipher_command(string format, string text, string passw) {
	// +2 is an arbitrary number meant to reduce chances of an out-of-bounds error
	char *temp = new char[format.length() + text.length() + passw.length() + 2];
	sprintf(temp, format.c_str(), text.c_str(), passw.c_str());
	//cout << "command2: " << temp << endl;
	return temp;
}

void copy_to_clipboard(string text) {
	string command = create_command("echo \"%s\" | xclip -selection c", text);
	//cout << "clip command: " << command << endl;
	system(command.c_str());
}


// for summary of options and which ones are used, go to https://wiki.openssl.org/index.php/Enc, 
// run [openssl enc -help]
// and check out https://wiki.openssl.org/index.php/Command_Line_Utilities
string encrypt(string text, string passw) {
	//cout << "stuff: " << remove_endline(text) << " " << passw << endl;
	string command = create_cipher_command("echo \"%s\" | openssl enc -aes-256-cbc -salt -pbkdf2 -e -a -pass pass:%s", text, passw);
	/*system(command.c_str());
	cout << "system done" << endl;
	
	FILE* pipe = popen(command.c_str(), "r");
    char buffer[128];
    string result = "";
    if (!pipe) throw runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
	cout << "popen done" << endl;
	cout << result << endl;*/
	return exec(command.c_str());
}
string decrypt(string text, string passw) {
	return exec(create_cipher_command("echo \"%s\" | openssl enc -aes-256-cbc -salt -pbkdf2 -d -a -pass pass:%s", text, passw).c_str());
}

string apply_hash(string text) {
	string result = exec(create_command("echo \"%s\" | sha256sum", text).c_str());
	string suffix = "  -\n";
	//cout << result << endl;
	return result.substr(0, result.length() - suffix.length());
}

bool check_key(string key) {
	return strcmp(apply_hash(key).c_str(), key_hash.c_str()) == 0;
}


string prompt_key(void) {
	
	string key;

	key = getpass("Please enter the key.");
	//cout << "key: " << key << endl;
	//cin >> key;
	
	if(!check_key(key)) {
	
		cout << "Incorrect Key" << endl;
		return "";
	}
	return key;
}


bool is_lower_letter(char c) {
	return c >= 'a' and c <= 'z';
}

bool is_number(char c) {
	return c >= '0' and c <= '9';
}

// Should change a letter to capital letter
// Should add at least one non-alphanumeric character (convert a number)
string increase_complexity(string password) {
	
	bool letter_found = false;
	bool number_found = false;
	for(int i = 0; i < password.length(); i++) {
		if(!letter_found and is_lower_letter(password[i])) {
			letter_found = true;
			password[i] -= 'a' - 'A'; // convert to uppercase
		}
		if(!number_found and is_number(password[i])) {
			number_found = true;
			password[i] = '=';
		}
	}
	return password;
}



#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <sstream>
#include <stdexcept>
#include <cstdlib> // TODO this might be deprecated, copied from http://www.cplusplus.com/articles/E6vU7k9E/
using namespace std;

const string welcome_text = "Hi! Welcome to Justin's Password Manager.\n";

string prompt_key(void) {
	
	//cout << "Prompting" << endl;
	
	string key;

	key = getpass("Please enter the key.");
	
	if(!check_key(key)) {
	
		//cout << "Incorrect Key" << endl;
		return "";
	}
	return key;
}


string get_menu(vector<string> options) {
	string result = "";
	for(int i = 0; i < options.size(); i++)
	{
		//cout << i << ") " << options[i] << endl;
		result += to_string(i + 1) + ") " + options[i] + "\n"; 
	}
	return result;
}

int get_choice(int num_options) {
	do
	{
		int input;
		cin >> input;
		try
		{
			// TODO I'm tired and I'm not sure how cin works
			// (is this part even necessary?)
			int result = input;
			return result - 1;
		}
		catch(...)
		{
			cout << "Invalid input, please enter a number 1-" << num_options << endl;
			continue;
		}
	} while(true);
}


int menu_choice(vector<string> options, string title) {
	cout << title << endl;
	cout << get_menu(options) << endl;
	return get_choice(options.size());
}

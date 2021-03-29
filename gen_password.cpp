#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
using namespace std;

ifstream urandom("/dev/urandom");


char to_base64(char c)
{
	unsigned int temp = c;
	temp %= 64;
	if(temp < 26)
		temp += 'A';
	else if (temp < 52)
		temp = temp - 26 + 'a';
	else if (c < 62)
		temp = temp - 52 + '0';
	else if (c == 62)
		temp = '+';
	else if (c == 63)
		temp = '/';
	return (char) temp;
}


int main(int argc, char **argv)
{
	int len = 20;
	if(argc > 1)
	{
		
		if(!sscanf(argv[1], "%d", &len))
		{
			cout << "Command must be in the format \"./gen_password %d\"" << endl;
		}
		
		if(len < 15)
		{
			cout << "WARNING: To be safer, the length should be longer" << endl;
		}
		
	}
	
	string pass = "";
	for(int i = 0; i < len; i++)
	{
		char c;
		urandom >> c;
		char alphanum = to_base64(c);
		//cout << alphanum << endl;
		pass += alphanum;
	}
	
	cout << pass << endl;
}

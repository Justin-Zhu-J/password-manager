#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

// Copied from: https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po
// Gets stdout output of a command in the command-line
string exec(const char* cmd) {
	//cout << "Exec command: " << cmd << endl;
		FILE* pipe = popen(cmd, "r");
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
		return result;
}


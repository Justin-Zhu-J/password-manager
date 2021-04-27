#include <string>

using namespace std;

// NOT SCALABLE
// If want to make scalable: https://www.tutorialspoint.com/variable-number-of-arguments-in-cplusplus
// I thought of using a variable number of parameters, but that might not work best for the format string
// Unless you feed the format string into itself, and fill in one element at a time.
// That might work. But for now, I'm just doing this.
char* format_command(string format, string a = "", string b = "", string c = "", string d = "", string e = "")
{
	// +2 is an arbitrary number meant to reduce chances of an out-of-bounds error
	char *temp = new char[format.length() + a.length() + b.length() + c.length() + d.length() + e.length() + 2];
	sprintf(temp, format.c_str(), a.c_str(), b.c_str(), c.c_str(), d.c_str(), e.c_str());
	printf("Command: %s\n", temp);
	return temp;
}

void copy_to_clipboard(string text) {
	string command = format_command("echo \"%s\" | xclip -selection c", text);
	//cout << "clip command: " << command << endl;
	system(command.c_str());
}


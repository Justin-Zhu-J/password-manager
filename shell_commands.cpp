#include <string>

// Note: this function only supports one parameter
std::string create_command(std::string format, std::string parameter) {
	// +2 is an arbitrary number meant to reduce chances of an out-of-bounds error
	char *temp = new char[format.length() + parameter.length() + 2];
	sprintf(temp, format.c_str(), parameter.c_str());
	return temp;
}

// Note: this function only supports two parameters
std::string create_cipher_command(std::string format, std::string text, std::string passw) {
	// +2 is an arbitrary number meant to reduce chances of an out-of-bounds error
	char *temp = new char[format.length() + text.length() + passw.length() + 2];
	sprintf(temp, format.c_str(), text.c_str(), passw.c_str());
	return temp;
}

void copy_to_clipboard(std::string text) {
	std::string command = create_command("echo \"%s\" | xclip -selection c", text);
	//cout << "clip command: " << command << endl;
	system(command.c_str());
}



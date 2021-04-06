#include "shell_commands.cpp"
#include "exec.cpp"
#include <string>

// for summary of options and which ones are used, go to https://wiki.openssl.org/index.php/Enc, 
// run [openssl enc -help]
// and check out https://wiki.openssl.org/index.php/Command_Line_Utilities
std::string encrypt(std::string text, std::string passw) {
	//cout << "stuff: " << remove_endline(text) << " " << passw << endl;
	std::string command = create_cipher_command("echo \"%s\" | openssl enc -aes-256-cbc -salt -pbkdf2 -e -a -pass pass:%s", text, passw);
	return exec(command.c_str());
}
std::string decrypt(std::string text, std::string passw) {
	return exec(create_cipher_command("echo \"%s\" | openssl enc -aes-256-cbc -salt -pbkdf2 -d -a -pass pass:%s", text, passw).c_str());
}

std::string apply_hash(std::string text) {
	std::string result = exec(create_command("echo \"%s\" | sha256sum", text).c_str());
	std::string suffix = "  -\n";
	//cout << result << endl;
	return result.substr(0, result.length() - suffix.length());
}

bool check_key(std::string key) {
	return strcmp(apply_hash(key).c_str(), key_hash.c_str()) == 0;
}



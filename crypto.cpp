#include "shell_commands.cpp"
#include "exec.cpp"
#include ".secrets.h"
#include <string>

// for summary of options and which ones are used, go to https://wiki.openssl.org/index.php/Enc, 
// run [openssl enc -help]
// and check out https://wiki.openssl.org/index.php/Command_Line_Utilities
std::string encrypt(std::string text, std::string passw) {
	//cout << "stuff: " << remove_endline(text) << " " << passw << endl;
	return exec(format_command("echo \"%s\" | openssl enc -aes-256-cbc -salt -pbkdf2 -e -a -pass pass:%s", text, passw));
}
std::string decrypt(std::string text, std::string passw) {
	return exec(format_command("echo \"%s\" | openssl enc -aes-256-cbc -salt -pbkdf2 -d -a -pass pass:%s", text, passw));
}

std::string apply_hash(std::string text) {
	std::string result = exec(format_command("echo \"%s\" | sha256sum", text));
	std::string suffix = "  -\n";
	//cout << result << endl;
	return result.substr(0, result.length() - suffix.length());
}

bool check_key(std::string key) {
	return strcmp(apply_hash(key).c_str(), key_hash.c_str()) == 0;
}

bool encryptPasswordFile(string key)
{
	/*string error_text = exec(format_command("echo %s > openssl rsautl -decrypt -inkey %s -in %s -out %s", key, PRIVATE_KEY, ENC_KEY_FILE, KEY_FILE));
	if(error_text.length() != 0)
		return false;
	
	error_text = exec(format_command("openssl enc -aes-256-cbc -salt -in %s -out %s -pass file:%s -pbkdf2", PASSWORD_FILE, ENC_PASSWORD_FILE, KEY_FILE));
	if(error_text.length() != 0)
		return false;
	
	exec(format_command("rm %s", KEY_FILE));
	exec(format_command("rm %s", PASSWORD_FILE));*/
	
	return error_text.length() == 0;
}

bool decryptPasswordFile(string key)
{
	/*string error_text = exec(format_command("echo %s > openssl rsautl -decrypt -inkey %s -in %s -out %s", key, PRIVATE_KEY, ENC_KEY_FILE, KEY_FILE));
	if(error_text.length() != 0)
		return false;
	
	error_text = exec(format_command("openssl enc -d -aes-256-cbc -in %s -out %s -pass file:%s -pbkdf2", ENC_PASSWORD_FILE, PASSWORD_FILE, KEY_FILE));
	if(error_text.length() != 0)
		return false;
	
	exec(format_command("rm %s", KEY_FILE));*/
	
	return true;
}

/*
bool oneTimeSetup(void)
{
	string error_text = exec(format_command("echo %s > openssl rsautl -decrypt -inkey %s -in %s -out %s", key, PRIVATE_KEY, ENC_KEY_FILE, KEY_FILE));
	if(error_text.length() != 0)
		return false;
	return true;
}*/




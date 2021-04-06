
using namespace std;

bool is_lower_letter(char c) {
	return c >= 'a' and c <= 'z';
}

bool is_number(char c) {
	return c >= '0' and c <= '9';
}

string remove_endline(string text) {
	return text.substr(0, text.length() - 1);
}


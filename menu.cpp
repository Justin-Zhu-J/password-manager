#include "password.cpp"
using namespace std;

string getPassword(string key);
string createPassword(string key);
string changePassword(string key);
string removePassword(void);


int main(void)
{
    
    cout << welcome_text << endl;
    
    string key;
    
    int fails = 0;
    for(int i = 0; i < 3; i++)
    {
		key = prompt_key();
		if(strcmp(key.c_str(), "") == 0) // if invalid key
			fails++;
		else
			break;
	}
	if(fails >= 3)
		return 0;
    
    vector<string> options {"Get Password", "Create Password", "Change Password", "Remove Password"};
    
    int choice = menu_choice(options);
    string password;
    
    switch(choice)
    {
    	case 0:
    		// get password
    		password = getPassword(key);
    		break;
    	case 1:
    		// create password
    		password = createPassword(key);
    		break;
    	case 2:
    		// change password
    		password = changePassword(key);
    		break;
    	case 3:
    		// remove password
    		password = removePassword();
    		break;
    }
    
    copy_to_clipboard(password);
	cout << "Password has been copied to clipboard." << endl;
	
	cout << "Have a nice day!" << endl;
	system("sleep 1");
    
    return 0;
}


string getPassword(string key)
{
	vector<Password*> pwlist = Password::parseFile(PASSWORD_FILE);
	try {   		
		if(pwlist.size() <= 0)
		{
			cout << "No passwords stored. Please store passwords before accessing them." << endl;
			exit(0);
		}
		
		cout << Password::passwordMenu(pwlist) << endl;
		
		int acct_num = get_choice(pwlist.size());
		
		string plaintext = decrypt(pwlist[acct_num]->getCiphertext(), key);
		
		plaintext = remove_endline(plaintext);
		
		return plaintext;
	}
	catch (int e) {
		cout << "An exception occurred. Please try again." << endl;
		exit(0);
	}
}


string createPassword(string key)
{
	cout << "Please enter the name of the account you would like to store the password of:" << endl;
	string account;
	cin >> account;
		
	cout << "Would you like to create a new password [0] or enter an existing one [1]?" << endl;
	string choice;
	cin >> choice;
	
	string plaintext;
	if(!atoi(choice.c_str())) {
		plaintext = remove_endline(exec("./gen_password"));
	}
	else {
		plaintext = getpass("Please enter your existing password: ");
	}
	
	string ciphertext = remove_endline(encrypt(plaintext, key));
	Password::appendFile(PASSWORD_FILE, new Password(account, NONE, ciphertext));
	return plaintext;
}

string changePassword(string key)
{
	return "";
}

string removePassword(void)
{
	return "";
}


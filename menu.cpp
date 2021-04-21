#include "password.cpp"
using namespace std;

void viewAccounts(void);
string getPassword(string key);
string createPassword(string key);
string changePassword(string key);
string deletePassword(string key);

// TODO fix memory leaks

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
    
    vector<string> options {"View Accounts", "Get Password", "Create Password", "Change Password", 
    						"Delete Password", "Reset Password", "Update Password Tier", "Change Password Group", "Exit"};
    
    while(true)
    {
		int choice = menu_choice(options, "Main Menu");
		string password;
		bool exiting = false;
		switch(choice)
		{
			case 0:
				// View accounts
				viewAccounts();
				break;
			case 1:
				// Get password
				password = getPassword(key);
				break;
			case 2:
				// Create password
				password = createPassword(key);
				break;
			case 3:
				// Change password
				password = changePassword(key);
				break;
			case 4:
				// Delete password
				password = deletePassword(key);
				break;
			case 5:
				// Reset password
				// TODO
				password = deletePassword(key);
				break;
			case 6:
				// TODO
				// Update password tier
				password = deletePassword(key);
				break;
			case 7:
				// TODO
				// Change password group
				password = deletePassword(key);
				break;
			default:
				cout << "Exiting..." << endl;
				exiting = true;
				break;
		}
		if(exiting)
		{
			copy_to_clipboard("FILLER_TEXT");
			break;
		}
		if(choice > 0)
		{
			copy_to_clipboard(password);
			cout << "Password has been copied to clipboard." << endl;
		}
	}
	
	cout << "Have a nice day!" << endl;
	system("sleep 1");
    
    return 0;
}


void viewAccounts(void)
{	
	vector<Password*> pwlist = Password::parseFile(PASSWORD_FILE); 	
	if(pwlist.size() <= 0)
	{
		cout << "No passwords stored. Please enter passwords before accessing them." << endl;
		exit(0);
	}
	cout << Password::passwordMenu(pwlist) << endl;
}

string getPassword(string key)
{
	vector<Password*> pwlist = Password::parseFile(PASSWORD_FILE);
	try {   		
		if(pwlist.size() <= 0)
		{
			cout << "No passwords stored. Please enter passwords before accessing them." << endl;
			exit(0);
		}
		
		cout << Password::passwordMenu(pwlist) << endl;
		
		cout << "Please choose a password to retrieve" << endl;
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
	Password::appendFile(PASSWORD_FILE, new Password(account, NONE, ciphertext, "NONE"));
	return plaintext;
}

string changePassword(string key)
{
	vector<Password*> pwlist = Password::parseFile(PASSWORD_FILE);
	try {   		
		if(pwlist.size() <= 0)
		{
			cout << "No passwords stored. Please store passwords before removing them." << endl;
			exit(0);
		}
		
		cout << Password::passwordMenu(pwlist) << endl;
		
		cout << "Please choose a password to change" << endl;
		int acct_num = get_choice(pwlist.size());
		
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
		Password *temp = pwlist[acct_num];
		pwlist[acct_num] = new Password(temp, ciphertext);
		delete temp;
		Password::writeFile(PASSWORD_FILE, pwlist);
		
		return plaintext;
	}
	catch (int e) {
		cout << "An exception occurred. Please try again." << endl;
		exit(0);
	}
}

string deletePassword(string key)
{
	vector<Password*> pwlist = Password::parseFile(PASSWORD_FILE);
	try {   		
		if(pwlist.size() <= 0)
		{
			cout << "No passwords stored. Please store passwords before removing them." << endl;
			exit(0);
		}
		
		cout << Password::passwordMenu(pwlist) << endl;
		
		cout << "Please choose a password to delete" << endl;
		int acct_num = get_choice(pwlist.size());
		
		string plaintext = decrypt(pwlist[acct_num]->getCiphertext(), key);
		
		plaintext = remove_endline(plaintext);
		pwlist.erase(pwlist.begin() + acct_num);
		Password::writeFile(PASSWORD_FILE, pwlist);
		
		return plaintext;
	}
	catch (int e) {
		cout << "An exception occurred. Please try again." << endl;
		exit(0);
	}
}


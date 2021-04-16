#include "password.cpp"
using namespace std;

ifstream pass_fin(".passwords.txt");
/*
vector< pair<string, string> > accounts;

void load_pass_file(void) {
	// Based on code copied from here: https://stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c
	string acct, passw;
	while (getline(pass_fin, acct))
	{
		getline(pass_fin, passw);
		accounts.push_back(make_pair(acct, passw));
		getline(pass_fin, acct); // there is an empty line separating entries
	}
}

string gen_account_list(void) {
	string menu = "List of Accounts:\n";
	for(int i = 0; i < accounts.size(); i++) {
		char *temp = new char[to_string(i + 1).length() + accounts[i].first.length() + 10]; // added 10 for insurance, really only need 3
		//cout << accounts[i].first << endl;
		
		char account[accounts[i].first.length() + 1];
		strcpy(account, accounts[i].first.c_str());
		sprintf(temp, "%d) %s\n", (i + 1), account);
		menu += temp;
	}
	return menu + "\n";
}*/

int main(void)
{
    
    cout << welcome_text << endl;
    
    vector<Password*> pwlist = Password::parseFile(PASSWORD_FILE);
    
    int exception_count = 0;
    
    for(int i = 0; i < 3; i++) {
    	try {   		
    		string key = prompt_key();
    		if(strcmp(key.c_str(), "") == 0) // if invalid key
    			continue;
    		
    		if(pwlist.size() <= 0)
    		{
    			cout << "No passwords stored. Please store passwords before accessing them." << endl;
    			break;
			}
			
			cout << Password::passwordMenu(pwlist) << endl;
			
    		int acct_num = get_choice(pwlist.size()) - 1;
    		
    		cout << __LINE__ << endl;
    		
    		string plaintext = decrypt(pwlist[acct_num]->getCiphertext(), key);
    		
    		cout << __LINE__ << endl;
    		plaintext = remove_endline(plaintext);
    		
    		
    		
    		copy_to_clipboard(plaintext);
    		cout << "Password has been copied to clipboard." << endl;
    		
    		cout << "Have a nice day!" << endl;
    		system("sleep 1");
    		break;
    	}
    	catch (int e) {
    		cout << "An exception occurred. Please try again." << endl;
    		exception_count++;
    	}
    	
    	if(exception_count >= 3) {
    		cout << "Too many failed attempts. Exiting." << endl;
    		break;
		}
    }
    
    return 0;
}




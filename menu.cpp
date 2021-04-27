#include "password.cpp"
using namespace std;

string getPassword(Password *pw,string key);
Password* createPassword(string key);
Password* changePassword(Password *pw,string key);
string deletePassword(Password *pw, string key);
void updatePasswordTier(Password *pw);
void changePasswordGroup(Password *pw);

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
		
	pair<vector<Password*>, vector<string> > stored_data = Password::parseFile(PASSWORD_FILE, key);
	vector<Password*> pwlist = stored_data.first;
	vector<string> groups = stored_data.second;
	
	vector<string> options {"Reselect Group", "Get Password", "Create Password", "Change/Reset Password", 
    						"Delete Password", "Update Password Tier", "Change Password Group", "Exit"};
	
	while(true)
	{
		int group_num = menu_choice(groups, "Please select a password group to access or exit:");
		if(group_num < 0 or group_num >= groups.size())
		{
			cout << "Exiting..." << endl;
			break;
		}
		vector<Password*> group = Password::filterGroup(pwlist, groups[group_num]);
		
		int index = menu_choice(Password::toString(group), "Please select a password from " + groups[group_num] + ", or enter a number out of range for no selection:");
		bool selected;
		if(index < 0 or index >= group.size())
		{
			selected = false;
			cout << "No Password Selected" << endl;
		}
		else
		{
			selected = true;
			cout << group[index]->getAccount() << " selected" << endl;
			cout << group[index]->toString() << endl;
		}
		
		const string UNSELECTED_MESSAGE = "No password selected, please exit this menu and reselect a password for this operation";
		
		while(true)
		{
			int choice = menu_choice(options, "Actions");
			string password;
			bool exiting = false;
			bool change_group = false;
			Password* temp_ptr;
			switch(choice)
			{
				case 0:
					// Reselect group
					change_group = true;
					break;
				case 1:
					// Get password
					if(not selected)
					{
						cout << UNSELECTED_MESSAGE << endl;
					}
					password = getPassword(group[index], key);
					break;
				case 2:
					// Create password
					group.push_back(createPassword(key));
					break;
				case 3:
					// Change password
					if(not selected)
					{
						cout << UNSELECTED_MESSAGE << endl;
					}
					group[index] = changePassword(group[index], key);
					break;
				case 4:
					// Delete password
					if(not selected)
					{
						cout << UNSELECTED_MESSAGE << endl;
					}
					password = deletePassword(group[index], key);
					temp_ptr = group[index];
					group.erase(group.begin() + index);
					exiting = true;
					break;
				case 5:
					// Update password tier
					if(not selected)
					{
						cout << UNSELECTED_MESSAGE << endl;
					}
					updatePasswordTier(group[index]);
					break;
				case 6:
					// Change password group
					if(not selected)
					{
						cout << UNSELECTED_MESSAGE << endl;
					}
					changePasswordGroup(group[index]);
					
					// if no longer in the group, move it out
					if(strcmp(groups[group_num].c_str(), group[index]->getGroup().c_str()) != 0)
					{
						pwlist.push_back(group[index]);
						
						if(count(groups.begin(), groups.end(), group[index]->getGroup()) == 0)
						{
							groups.push_back(group[index]->getGroup());
						}
						
						group.erase(group.begin() + index);
					}
					break;
				default:
					cout << "Exiting..." << endl;
					exiting = true;
					break;
			}
			
			if(change_group)
			{
				break;
			}
			if(exiting)
			{
				copy_to_clipboard("FILLER_TEXT");
				break;
			}
			if(choice > 0 and choice < 6) // options that involve changing passwords
			{
				copy_to_clipboard(password);
				cout << "Password has been copied to clipboard." << endl;
			}
		}
		
		pwlist.insert(pwlist.end(), group.begin(), group.end());
		group.clear();
	}
    
    cout << "Saving changes..." << endl;
    Password::writeFile(PASSWORD_FILE, pwlist, key);
	
	cout << "Have a nice day!" << endl;
	system("sleep 1");
    
    return 0;
}

string getPassword(Password* pw, string key)
{
	try {   		
		string plaintext = decrypt(pw->getCiphertext(), key);
		
		plaintext = remove_endline(plaintext);
		
		return plaintext;
	}
	catch (int e) {
		cout << "An exception occurred. Please try again." << endl;
		exit(0);
	}
}

string inputPassword(void)
{
	cout << "Would you like to create a new password [0] or enter an existing one [1]?" << endl;
	string choice;
	cin >> choice;
	
	string plaintext;
	if(!atoi(choice.c_str())) {
		return remove_endline(exec("./gen_password"));
	}
	return getpass("Please enter your existing password: ");
}

Password* createPassword(string key)
{
	cout << "Please enter the name of the account you would like to store the password of:" << endl;
	string account;
	cin >> account;
	
	string plaintext = inputPassword();
	
	string ciphertext = remove_endline(encrypt(plaintext, key));
	return new Password(account, NONE, ciphertext, "NONE");
}

Password* changePassword(Password *pw, string key)
{
	string plaintext = inputPassword();
	
	string ciphertext = remove_endline(encrypt(plaintext, key));
	Password *temp = pw;
	pw = new Password(temp, ciphertext);
	delete temp;
	return pw;
}

string deletePassword(Password *pw, string key)
{
	string plaintext = decrypt(pw->getCiphertext(), key);
	
	plaintext = remove_endline(plaintext);
	delete pw;
	
	return plaintext;
}

void updatePasswordTier(Password *pw)
{
	vector<string> tiers = Password::tierStringList();
	int choice = menu_choice(tiers, "Select a Tier:");
	if(choice < 0 or choice >= tiers.size())
	{
		return;
	}
	
	pw->setTier(choice);
	
	//cout << "Tier updated to " << tiers[choice] << endl;
}

void changePasswordGroup(Password *pw)
{
	cout << "Enter new group for password: ";
	string group;
	cin >> group;
	pw->setGroup(group);
}


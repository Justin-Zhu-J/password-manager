#include "password.cpp"
using namespace std;


int main(void)
{
    cout << welcome_text << endl;
    
    //cout << exec("echo \"hey\" | openssl enc -aes-256-cbc -salt -pbkdf2 -pass pass:hi") << endl;
    
    int exception_count = 0;
    
    for(int i = 0; i < 3; i++) {
    	try {
    		
    		string key = prompt_key();
    		if(strcmp(key.c_str(), "") == 0) // if invalid key
    			continue;
    		
    		cout << "Please enter the name of the account you would like to store the password of:" << endl;
    		string account;
    		cin >> account;
    			
    		string plaintext;
    			
    		cout << "Would you like to create a new password [0] or enter an existing one [1]?" << endl;
    		string choice;
    		cin >> choice;
    		if(!atoi(choice.c_str())) {
    			plaintext = remove_endline(exec("./gen_password"));
			}
    		else {
    			plaintext = getpass("Please enter your existing password: ");
    		}
    		
    		//cout << plaintext << endl;
    		
    		string ciphertext = remove_endline(encrypt(plaintext, key));
    		
    		copy_to_clipboard(plaintext);
    		cout << "Password has been copied to clipboard." << endl;
    		
    		Password::appendFile(PASSWORD_FILE, new Password(account, NONE, ciphertext));
    		
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




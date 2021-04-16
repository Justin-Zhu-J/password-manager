#include <fstream>
#include <sstream>
#include <vector>
#include "crypto.cpp"
#include "utils.cpp"
#include "helpers.cpp"
using namespace std;

/*typedef struct password {
	string account;
	int priority;
	string ciphertext;
} pw;*/

enum Tier {NONE, EMAIL, PERSONAL, COMPROMISING};
pair<Tier, string> tiers[] = {
	make_pair(NONE, "No info"), make_pair(EMAIL, "Public Contact Info"), 
	make_pair(PERSONAL, "Personal Data"), make_pair(COMPROMISING, "Secure Information (DO NOT STORE HERE)")
};

class Password
{
	private:
		const static string STORAGE_FORMAT;
		
		string account;
		Tier tier;
		string ciphertext;
		
	public:
		
		Password(string acct, Tier t)
		{
			account = acct;
			tier = t;
			ciphertext = remove_endline(exec("./gen_password"));
		}
		
		Password(string acct, Tier t, string ciphertxt)
		{
			account = acct;
			tier = t;
			ciphertext = ciphertxt;
		}
		
		static Tier intToTier(int t)
		{
			return tiers[t].first;
		}
		
		static vector<Password*> parseFile(string fileName)
		{
			ifstream fin(fileName);
			vector<Password*> list;
			
			while(!fin.eof())
			{
				string acct, ciphertxt, comma;
				int tier;
				
				fin >> acct;
				if(fin.eof())
					break;
				
				fin >> tier >> comma >> ciphertxt;
				//cout << "Account: " << acct << endl << tier << endl << comma << endl << ciphertxt << endl;
				//exit(0);
				acct = remove_endline(acct);
				//cout << acct << tier << endl;
				
				list.push_back(new Password(acct, intToTier(tier), ciphertxt));
			}
			
			fin.close();
			
			return list;
		}
		
		// Side effect: clear pwlist to ensure passwords are not retained in memory (might not be zeroed)
		static void writeFile(string fileName, vector<Password*> &pwlist)
		{
			ofstream fout(fileName);
			
			for(int i = 0; i < pwlist.size(); i++)
			{
				fout << pwlist[i]->account << ", " << pwlist[i]->tier << ", " << pwlist[i]->ciphertext << endl;
				delete pwlist[i];
			}
			
			pwlist.clear();
			fout.close();
		}
		
		// Side effect: delete pw to ensure nothing sensitive is stored in memory
		static void appendFile(string fileName, Password *pw)
		{
			ofstream fout;
			fout.open(fileName, ios_base::app);
			fout << pw->account << ", " << pw->tier << ", " << pw->ciphertext << endl;
			delete pw;
			fout.close();
		}
		
		static string passwordMenu(vector<Password*> &pwlist)
		{
			vector<string> accts;
			for(int i = 0; i < pwlist.size(); i++)
			{
				accts.push_back(pwlist[i]->account);
			}
			
			return get_menu(accts);
		}
		
		
		// TODO be more secure about giving this out
		string getCiphertext(void)
		{
			return ciphertext;
		}
};


const string Password::STORAGE_FORMAT = "%s, %d, %s\n";


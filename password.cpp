#include <fstream>
#include <sstream>
#include "crypto.cpp"
#include "utils.cpp"
using namespace std;

/*typedef struct password {
	string account;
	int priority;
	string ciphertext;
} pw;*/

class Password
{
	private:
		string account;
		int flags;
		string ciphertext;
		
		const static string STORAGE_FORMAT;
	
	public:
		
		Password(string acct, int flgs)
		{
			account = acct;
			flags = flgs;
			ciphertext = remove_endline(exec("./gen_password"));
		}
		
		Password(string acct, int flgs, string ciphertxt)
		{
			account = acct;
			flags = flgs;
			ciphertext = ciphertxt;
		}
		
		static vector<Password*> parseFile(string fileName)
		{
			ifstream fin(fileName);
			vector<Password*> list;
			
			while(!fin.eof())
			{
				string acct, ciphertxt;
				int flg;
				
				cin >> acct >> flg >> ciphertxt;
				
				list.push_back(new Password(acct, flg, ciphertxt));
			}
			
			return list;
		}
		
		// Side effect: clear pwlist to ensure passwords are not retained in memory (might not be zeroed)
		static void writeFile(string fileName, vector<Password*> &pwlist)
		{
			ofstream fout(fileName);
			
			for(int i = 0; i < pwlist.size(); i++)
			{
				fout << pwlist[i]->account << "," << pwlist[i]->flags << "," << pwlist[i]->ciphertext << endl;
				delete pwlist[i];
			}
			
			pwlist.clear();
		}
		
		// Side effect: delete pw to ensure nothing sensitive is stored in memory
		static void appendFile(string fileName, Password *pw)
		{
			ofstream fout;
			fout.open(fileName, ios_base::app);
			fout << pw->account << "," << pw->flags << "," << pw->ciphertext << endl;
			delete pw;
		}
};


const string Password::STORAGE_FORMAT = "%s,%d,%s\n";


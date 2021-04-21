#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include "crypto.cpp"
#include "utils.cpp"
#include "helpers.cpp"
using namespace std;

enum Tier {NONE, EMAIL, PERSONAL, COMPROMISING};
// These are the factors that you multiply by to compare to the standard of updating once per year
const long long MINUTE = 60, HOUR = 3600, DAY = 86400, MONTH = 2592000, YEAR = (long long) MONTH * 12;


long long time_now(void)
{
	time_t now = time(NULL);
	return (long long) now;
}

class Password
{
	private:
		const static string STORAGE_FORMAT;
		
		const static pair<Tier, string> tiers[];

		const static long long tier_freq[];
		
		string account;
		Tier tier;
		string ciphertext;
		string group;
		long long last_update;
		
		static Password* readPassword(ifstream &fin)
		{
		
			string acct, ciphertxt, comma, group;
			int tier;
			long long time_of_last_update;
			
			fin >> acct;
			fin >> tier >> comma >> ciphertxt >> group >> time_of_last_update;
			
			// password is not correctly formatted
			if(acct.length() == 0 or ciphertxt.length() == 0 or group.length() == 0)
				return NULL;
			acct = remove_endline(acct); // remove the comma
			ciphertxt = remove_endline(ciphertxt); // remove the comma
			group = remove_endline(group); // remove the comma
			
			return new Password(acct, intToTier(tier), ciphertxt, group, time_of_last_update);
		}
				
		static void writePassword(ofstream &fout, Password *pw)
		{
			fout << pw->account << ", " << pw->tier << ", " << pw->ciphertext << ", " << pw->group << ", " << pw->last_update << endl;
		}
		
		static long long readTime(ifstream &fin)
		{
			int sec, min, hour, day, mon, year; // since 00:00:00 Jan 1st 1900
			
			fin >> sec >> min >> hour >> day >> mon >> year;
			
			struct tm stored = {sec, min, hour, day, mon, year};
			
			long long seconds = (long long) (mktime(&stored));
			return seconds;
		}
		
		static long long timeToReset(Password *pw)
		{
			// Don't worry about resetting if it's already publicly accessible
			if(pw->tier <= EMAIL or tier_freq[pw->tier] <= 0)
			{
				return -1;
			}
			return max((YEAR / tier_freq[pw->tier]) - (time_now() - pw->last_update), (long long) 0);
		}
		
		static int daysToReset(Password *pw)
		{
			long long time_to_reset = timeToReset(pw);
			if(time_to_reset < 0)
				return -1;
			return timeToReset(pw) / DAY;
		}
		
		static bool needsReset(Password *pw)
		{
			return daysToReset(pw) == 0;
		}
		
		static void resetPassword(Password *pw)
		{
			pw->ciphertext = remove_endline(exec("./gen_password"));
			pw->last_update = time_now();
		}
		
		
		Password(string acct, Tier t, string ciphertxt, string g, long long _last_update)
		{
			account = acct;
			tier = t;
			ciphertext = ciphertxt;
			group = g;
			last_update = _last_update;
		}

		
	public:
		
		Password(string acct, Tier t)
		{
			account = acct;
			tier = t;
			ciphertext = remove_endline(exec("./gen_password"));
			group = "NONE";
			last_update = time_now();
		}
		
		Password(string acct, Tier t, string ciphertxt, string g)
		{
			account = acct;
			tier = t;
			ciphertext = ciphertxt;
			group = g;
			last_update = time_now();
		}
		
		Password(Password *other, string ciphertxt)
		{
			account = other->account;
			tier = other->tier;
			ciphertext = ciphertxt;
			group = other->group;
			last_update = time_now();
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
				char c = fin.get();
				if(fin.eof() or c == 0)
					break;
				fin.putback(c);
				if(fin.eof() or c == 0)
					break;
				try
				{
					Password *pass = readPassword(fin);
					
					// TODO this is only a bandaid :(
					// for whatever reason, the stream doesn't close after the final character
					if(pass == NULL)
						break;
					list.push_back(pass);
				}
				catch(int e)
				{
					cout << "Error in reading password file, terminating read prematurely..." << endl;
					break;
				}
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
				if(needsReset(pwlist[i]))
					resetPassword(pwlist[i]);
				writePassword(fout, pwlist[i]);
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
			writePassword(fout, pw);
			delete pw;
			fout.close();
		}
		
		static string passwordMenu(vector<Password*> &pwlist)
		{
			vector<string> accts;
			for(int i = 0; i < pwlist.size(); i++)
			{
				// TODO do proper padding instead of just tabs
				accts.push_back(pwlist[i]->account + "\t\t" + tiers[pwlist[i]->tier].second);
				int days_to_reset = daysToReset(pwlist[i]);
				if(days_to_reset > 0)
					accts[i] += "\t\t" + to_string(days_to_reset) + " days until next password reset";
				if(days_to_reset == 0)
					accts[i] += "\t\tRESETTING ON EXIT";
			}
			
			return "Stored Passwords\n" + get_menu(accts);
		}
		
		
		// TODO be more secure about giving this out
		string getCiphertext(void)
		{
			return ciphertext;
		}
};

// INIT STATIC VALUES
const string Password::STORAGE_FORMAT = "%s, %d, %s, %s, %d\n";
const long long Password::tier_freq[] = {0, 0, 4, 12};
const pair<Tier, string> Password::tiers[] = 
	{
		make_pair(NONE, "No info"), make_pair(EMAIL, "Public Contact Info"), 
		make_pair(PERSONAL, "Personal Data"), make_pair(COMPROMISING, "Secure Information (DO NOT STORE HERE)")
	};


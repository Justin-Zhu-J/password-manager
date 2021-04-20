#include "password.cpp"
using namespace std;



int main(void)
{
	// Time tester
	// NOTE: this function will normally be private, 
	//		but is manually made public while testing
	// should be "0 0 0 24 11 116" in testing.txt
	ifstream fin("testing.txt");
	cout << Password::readTime(fin) << endl;
}

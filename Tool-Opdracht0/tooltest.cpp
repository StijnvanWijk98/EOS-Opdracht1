#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::string;
using std::endl;

int main(int argc, char *argv[]){
	string a;
	for(;;){
		cin >> a;
		if(cin.eof()){
			return 0;
		}
		cout << a << argv[1] << a << endl;
	}
	return 0;
}

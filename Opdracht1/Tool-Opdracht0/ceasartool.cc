#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::getline;
using std::string;
using std::endl;
using std::atoi;


string ceasarcipher(string ceasarstring, int rotation){	
	for(int movecounter = 0; movecounter < rotation; movecounter++){
		for(int indexstr = 0; indexstr < ceasarstring.size(); indexstr++){
			char currentletter = ceasarstring[indexstr];
			if(currentletter == 'z' ){
				ceasarstring[indexstr] = 'a';
			} else if(currentletter == 'Z'){
				ceasarstring[indexstr] = 'A';
			} else{
				char change = currentletter++;
				ceasarstring[indexstr] = currentletter++;
			}
		}
	}
  return ceasarstring;
}

int main(int argc, char *argv[]){
	if(argc != 2){
		std::cerr << "Ik zou het leuk vinden als je 1 argument mee geeft, anders kan ik mijn werk niet doen.";
		return -1; 
	}
	
	string tocipher;
	int rotation = atoi(argv[1]);
	while(true){
		std::getline(cin, tocipher);
		if(std::cin.eof()){
			return 0;
		}
		cout << ceasarcipher(tocipher, rotation) << endl;
	}
}
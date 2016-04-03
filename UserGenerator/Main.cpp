#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "WordGen.h"
using namespace std;


int main(){
	string file = "dict.txt";
	WordGen eng;

	//------------------building the initial piece bank--------------
	//cout << "eng.refineRaw(file);" << endl;
	//eng.refineRaw(file);
	//cout << "eng.buildSubWordVector();" << endl;
	//eng.buildSubWordVector();

	//eng.saveSub();

	//cout << "eng.giveStringRanking();" << endl;
	//eng.giveStringRanking();
	//cout << "eng.stringSort();" << endl;
	//eng.stringSort();
	//cout << "eng.saveSubwordString();" << endl;
	//eng.saveSubwordString();
	//eng.loadSubWord("subword.txt");

	//cout << "eng.quicksort();" << endl;
	//eng.quicksort();
	//cout << "eng.saveSubwordFreq();" << endl;
	//eng.saveSubwordFreq();
	//--------------------------------------------------------------------


	//-----------------------loading a preprocessed set of word fragments--------------

	eng.loadSubWord("SORTEDsubwordsBySTRING.txt");
	eng.giveStringRanking();
	//eng.stringSort();
	//eng.saveSubwordString();
	//eng.saveVector();
	//----------------------------------------------------------------------------------


	//-------------------------Creating the username---------------------------
	vector<string> myUsernames;
	vector<string> nameList;
	nameList.resize(10);
	string letter = "";
	cout << "-------USERNAME GENERATOR------\nI would like a... \n[1] completely random name\n[2] name starting with...\n";
	int response;
	cin >> response;
	if (response == 2){
		cout << "What would you like the name to start with?" << endl;
		cin >> letter;
		for (int i = 0; i < letter.size(); i++)
		{
			letter[i] = toupper(letter[i]);
		}
		while (true){
			for (int i = 0; i < 10; i++)
			{
				//char achar = '0' + i+17;
				//letter = achar;
				nameList[i] = eng.makeName(letter.substr(letter.size() - 1, 1));// give last letter to makename
				nameList[i] = (letter.substr(0, letter.size() - 1)) + nameList[i];// add all but the last letter to the generated portion
				for (int j = 1; j < nameList[i].size(); j++){ nameList[i][j] = tolower(nameList[i][j]); }
				cout << "username [" << i << "]: " << nameList[i] << endl;
			}
			system("PAUSE");
			cout << "\n";
		}
	}else{
		while (true){
			for (int i = 0; i < 10; i++)
			{
				nameList[i] = eng.makeName("");
				for (int j = 1; j < nameList[i].size(); j++){nameList[i][j] = tolower(nameList[i][j]);}
				cout << "username [" << i << "]: " << nameList[i] << endl;
			}
		system("PAUSE");
		cout << "\n";
		}
	}



	
	string kyros = eng.makeName("SO");
	reverse(kyros.begin(), kyros.end());
	cout << kyros << endl;
	cout << eng.makeName("KRA") << endl;
	system("PAUSE");
	return 0;
}  
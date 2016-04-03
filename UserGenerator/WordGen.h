#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

// QUICK RUNTHROUGH OF PROGRAM
// example input word: "dictionary"
//[d] [di] [dic] [i] [ic] [ict] [c] [ct] [cti] [t] [ti] [tio]...... //every possible combination of letters are stored according to english spelling
//suppose the last letter on our username so far was 'i'
//loop{
//- what comes after i?- -> look in array for entries starting with i
// (with weighted random) select an entry starting with i and take the rest of its following characters
// add characters to the end of the username currently being built
//}
//stop when new usernames length has gone to desired length
// summary: each letter in the username is chosen by its likelihood of coming after the preceeding letter, determined from a parsed dictionary

class WordGen{
public:

	struct subword{
		string item;
		int frequency;
		int stringPriority;
		subword(string i){
			item = i;
			frequency = 1;
			stringPriority = -1;
		}
		subword(string i, int a){
			item = i;
			frequency = a;
			stringPriority = -1;
		}
	};
	vector<string> wordVec;// holds raw words from the dictionary
	vector<subword> subwords;// vector of structures holding the letters and their frequency
	// default constructor
	WordGen(){
		srand(time(NULL));
	}
	//nondefault
	WordGen(string filename){
		srand(time(NULL));
		refineRaw(filename);
	}


	//################################### FUNCTIONS USED TO PROCESS FILES / BUILD VECTORS #######################


	void refineRaw(string filename){// parse dictionary file into distinct words then insert to wordvector (TURN DICTIONARY INTO VECTOR OF WORDS)
	  ifstream fin;
	  fin.open(filename);
	  string word;
	    for (int i = 0; !fin.eof(); i++){
	    	fin >> word;
	    	for (unsigned int i = 0; i < word.size(); i++) {
	    		if (word[i] == '-'){
	    			string word2 = word.substr(i + 1, word.size() - i);
	    			word = word.substr(0, i);
					wordVec.push_back(word2);
	    		}
	    	} 
			wordVec.push_back(word);
	    }
	}

	void loadSubWord(string filename){// loads from the saved txt file of sub word components (LOAD WORD PIECES INTO ARRAY)
		if (filename == "SORTEDsubwordsFREQ.txt"){ cout << "please load the vector using the file sorted by STRING,\n binary search cannot be performed by frequency.\n"; return; }
		int frequency = 0;
		string item = "";
		ifstream fin;
		fin.open(filename);
		for (int i = 0; !fin.eof(); i++){
			fin >> item;
			fin >> frequency;
			subword word(item, frequency);
			subwords.push_back(word);
		}

	}


	void buildSubWordVector(){// once the vector of dictionary words has been created, build new vector holding 1,2, and 3 length strings of each letter sequence
	    string onechar = "";
	    string twochar = "";
	    string threechar = "";
	    bool oneexists = false;
	    bool twoexists = false;
	    bool threeexists = false;
	    
		for (unsigned int i = 0; i < wordVec.size(); i++)// go through list of words
		{
			if (i % 1000 == 0){cout << i << endl;}
			for (unsigned int j = 0; wordVec[i][j] != '\0'; j++)// go through each letter of each word
	    	{
				onechar = wordVec[i][j];
	    		//cout << onechar << endl;///
				if (j + 1 < wordVec[i].size()){// adding pieces up all at once causes the pieces to be interpreted as ascii table numbers
					twochar = wordVec[i][j];
					twochar += wordVec[i][j + 1];
	    		}
	    		else{ twochar = ""; }
	    		//cout << twochar << endl;///
				if (j + 2 < wordVec[i].size()){// only builds a set of 3 characters if there are more than 3 chars left in the array from j's location
					threechar = wordVec[i][j];
					threechar += wordVec[i][j + 1];
					threechar += wordVec[i][j + 2];
	    		}
	    		else{ threechar = ""; }
	    		//cout << threechar << endl;///
	    
				for (unsigned int h = 0; h < subwords.size(); h++)// compare the letter to the subword database
	    		{
					if (wordVec[i][j] != '\0' && onechar == subwords[h].item){ subwords[h].frequency++; oneexists = true; }// if its a match increase frequency
					if (j + 1 <= wordVec[i].size() && twochar == subwords[h].item){ subwords[h].frequency++; twoexists = true; }
					if (j + 2 <= wordVec[i].size() && threechar == subwords[h].item){ subwords[h].frequency++; threeexists = true; }
	    		}
	    		if (!oneexists && onechar != ""){
	    			subword piece(onechar);
	    			subwords.push_back(piece);
	    		}
	    		if (!twoexists && twochar != ""){
	    			subword piece(twochar);
	    			subwords.push_back(piece);
	    		}
	    		if (!threeexists && threechar != ""){
	    			subword piece(threechar);
	    			subwords.push_back(piece);
	    		}
	    
	    		oneexists = false;
	    		twoexists = false;
	    		threeexists = false;
	    
	    	}
	    }
	}
	
	// can only be performed after substring is created. (see similar code in binarysearch(string) for more comments)
	void giveStringRanking(){// if subword array has already been made but has not been assigned rank values for alphabetical sorting:
		for (unsigned int i = 0; i < subwords.size(); i++)
		{
			subwords[i].stringPriority = 0;
			int charvalue = 0;
			int workingPiece = 0;
			int multiplier = 1000000;
			for (int j = 0; j < subwords[i].item.size(); j++)
			{
				workingPiece = subwords[i].item[j] - 64;
				workingPiece *= multiplier;
				multiplier /= 100;
				charvalue += workingPiece;

				//charvalue = subwords[i].item[j] - 64;
				//charvalue *= multiplier;

				//subwords[i].stringPriority *= 100;
				//subwords[i].stringPriority += charvalue;

			}
			subwords[i].stringPriority = charvalue;
		}
		
	}
	//##################################### SAVING FUNCTIONS ###########################################
	void saveSub(){// saves the vector of word pieces as it was originally read in from the dictionary
	    ofstream fout;
	    fout.open("subwordtest.txt");
	    ofstream fout2;
	    fout2.open("subsourcetest.txt");
		for (unsigned int i = 0; i < subwords.size(); i++)
	    {
	    	fout << subwords[i].item << " " << subwords[i].frequency << endl;
	    	fout2 << subwords[i].item << endl;
	    } 
	}

	void saveSubwordString() {// sorts subwords by string, then saves the array to file
		ofstream fout3;
		stringQsort(subwords, 0, subwords.size());
		fout3.open("SORTEDsubwordsBySTRING.txt");// filename for string value sorted
		for (unsigned int i = 0; i < subwords.size(); i++)
		{
			fout3 << subwords[i].item << " " << subwords[i].frequency << endl;
		}
	}

	void saveSubwordFreq() {// sorts sub words by frequency of occurance, then saves to file
		ofstream fout3;
		qsort(subwords, 0, subwords.size());
		fout3.open("SORTEDsubwordsFREQ.txt");// filename for frequency sorted
		for (unsigned int i = 0; i < subwords.size(); i++)
		{
			fout3 << subwords[i].item << " " << subwords[i].frequency << endl;
		}
	}

	void saveRaw(){ //save original dictionary words to file
	  ofstream fout;
	  fout.open("dict.txt");
	  for (unsigned int i = 0; i < wordVec.size(); i++)
	  {
		  fout << wordVec[i] << endl;
	  }
	}

	//################################### Crafting a Username ###############################

	// argument is any given string that the user wants the name to begin with
	string makeName(string username){//
		if (username == ""){ username = (rand() % 25) + 65; }// give a random starting letter if none is provided
		int loc;
		int nameSize = 8;// arbitrary size for the length of name we want

		while (username.length() < nameSize+1) // namesize+1 allows the loop to check at username[namesize]
		{
			loc = isInvalid(username);// holds either the location of problem in username, or -1 signifying no errors detected.
			if (loc >= 0)
			{
				pickNew(username, loc);// replace letter in username
			}
			else
			{
				pickNew(username, username.size() - 1);// add letters on the end
			}
		}


		return username.substr(0,nameSize);// safety precaution to ensure username is of size requested.
	}

	// even though the words are constructed with general trends, each letter is chosen oblivously to unadjacent letters. "MSLYNOAOA" could technically happen
	// function scans word and determines if any rules of fluency are broken, returns the index of the first problem letter detected
	int isInvalid(string uName){
		int last = uName.size() - 1;

		int j = 1; int k = 2; int l = 3;// the displaced incrementers prevent checking out of bounds
		for (int i = 0; i < uName.size(); i++){
			if (uName[i] == 'Q' && uName[i + 1] != 'U'){ return i; }// U must follow Q
			if (k <= last && isVowel(uName[i]) && isVowel(uName[j]) && isVowel(uName[k])){ return i; }// no 3 vowels in a row
			if (k <= last && !isVowel(uName[i]) && !isVowel(uName[j]) && !isVowel(uName[k])){ return i; }// no 3 consonants in a row
			if (j <= last &&!validDouble(uName[i], uName[j])){ return i; }// only allow some double letters
			if (l <= last && (uName[i] == uName[k]) && (uName[j] == uName[l])){ return j; }//if repeat, redo the second instance
			j++; k++; l++;
		}


		if (uName[last] == 'Q' || uName[last] == 'V' || uName[last] == 'J' || uName[last] == 'C'
			|| uName[last] == 'U' || uName[last] == 'I' || uName[last] == 'I'){ return last-1; }// no names ending in these


		if (uName.size() > 1){
			if (!isVowel(uName[0]) && !isVowel(uName[1])){// R H S L are the only consonants that can be after another consonant in the beginning
				if (uName[1] != 'R' && uName[1] != 'S' && uName[1] != 'L' && uName[1] != 'H'){
					return 0;
				}
			}
			if (isVowel(uName[0]) && uName[0] != 'A' && isVowel(uName[1])){// Names typically dont start on 2 vowels except 'A'
				return 0;
			}
			if (!isVowel(uName[last]) && (uName[last-1] != 'R' && uName[last-1] != 'L'
				&& uName[last-1] != 'S' && uName[last-1] != 'H' )){
				return last - 1;// If ending on 2 consonants, there must be a vowel then R L S or H then the final consonant
			}
		}
		return -1;
	}// returns the first letter of the problem. e.g. hfank would return 0, myrej would return 3



	bool repeating(string name){// if there are repeating sequences like "KERURURL"
		for (int i = 0; i < name.size() - 5; i++){
			if ((name[i] == name[i + 2]) && (name[i + 1] == name[i + 3])){ return true; }
		}
		return false;
	}

	bool validDouble(char first, char second){// only some letters can be double, like "HATTEL", not "NURWWA"
		if (first == second){
			if (first == 'O' || first == 'S' || first == 'M' || first == 'T' || first == 'N'){ return true; }
			return false;
		}
		return true;
	}

	bool isVowel(char letter){// returns if vowel
		if (letter == 'A' || letter == 'E' || letter == 'I' || letter == 'O' || letter == 'U' || letter == 'Y'){ return true; }
		return false;
	}

// picks a new letter starting at the specified location of the string
	void pickNew(string & uName, int startLoc){	
		int begin;
		int end;
		int resultingIndex = -1;
		long sumOfWeights = 0;
			begin = b_1st(0, subwords.size() - 1, uName[startLoc]) + 1;// binary search from index 0 to max, using last letter from "startletters"
			end = b_Lst(0, subwords.size() - 1, uName[startLoc]);
			for (int j = begin; j < end; j++)	// calculate the sum of all weights (frequencies)
			{
				sumOfWeights += subwords[j].frequency;
			}
			int random = rand() % sumOfWeights;	// pick a random number between 0 and the sum of all the weights combined
			for (int j = begin; j <end; j++)// peel each weight off of rand until rand is < frequency
			{
				if (random < subwords[j].frequency){
					resultingIndex = j;
					break;
				}
				random -= subwords[j].frequency;
			}

			int remLen = subwords[resultingIndex].item.size() - 1; // remaining length on chosen string
			string addPortion = subwords[resultingIndex].item.substr(1, remLen);// the found string minus the first letter it matched with

			if (startLoc == uName.size() - 1){// if were simply appending, then +=
			uName += addPortion;
			}
			else{// rewriting a former letter
				uName.replace(startLoc+1, remLen, addPortion);
			}
			//str.replace(starting location, duration, copy source)
			//str.substr(starting location, size from location to copy)
	}

// binary search which returns the first index of a starting letter, ex: b_1st("C") = index of "CAA" 
	int b_1st(int first, int last, char searchChar){
	
		if (first <= last){
			int mid = (first + last) / 2;

			if (subwords[mid].item[0] == searchChar){
				if (mid == 0) {return mid;} // if we are looking at index 0, we cant check index-1
				else if (subwords[mid - 1].item[0] != searchChar){ return mid; } // if the item before it doesnt have the same first letter, its first
				return b_1st(first, mid-1, searchChar);// only calls if we are in the middle of the letter were finding the first of
			}
			else if (subwords[mid].item[0] < searchChar){
				return b_1st(mid+1, last, searchChar);
			}
			else{ return b_1st(first, mid-1, searchChar); }

		}
		return -1;
	}
	// find the last index of a letter category
	int b_Lst(int first, int last, char searchChar){
		if (first <= last){
			int mid = (first + last) / 2;

			if (subwords[mid].item[0] == searchChar){
				if (mid == subwords.size()-1) { return mid; } // if mid has gotten to the end, we cant check beyond that
				else if (subwords[mid + 1].item[0] != searchChar){ return mid; }
				return b_Lst(mid+1, last, searchChar);
			}
			else if (subwords[mid].item[0] < searchChar){
				return b_Lst(mid + 1, last, searchChar);
			}
			else{ return b_Lst(first, mid - 1, searchChar); }

		}
		return -1;
	}



	//############################## SEARCH AND SORTS #######################################

	// public accessor to binary search
	int binarySearch(string searchItem){// ONLY TO BE USED AFTER SORTING BY STRINGVALUE
										// string priority must also be set for binarysearch to function
			int stringVal = 0;
			int charvalue = 0;
			// creates a integer that represents its placement in an alphabetical sort
			// each letter is converted into the number that corresponds to its place in the alphabet
			// "AD" would generate 100 + 4 = 104, which would be less than "BZC" 20,000 + 260 + 3 = 20103, allowing alphabetical sorting
			for (int j = 0; j < searchItem.size(); j++)
			{
				charvalue = searchItem[j] - 64;// assigns charvalue to hold the integer of its placement, e.g. A=1, B=2, C=3, D=4
				stringVal *= 100;
				stringVal += charvalue;

			}
		//cout << subwords[0].stringPriority << " " << subwords[subwords.size()-1].stringPriority << endl;
			if (stringVal >= subwords[0].stringPriority && stringVal <= subwords[subwords.size()-1].stringPriority){// if the input letters are between the first and last entries...
				
				return bSearch(0, subwords.size(), stringVal);
			}else{ return -5; }
		
	}


	int bSearch(int first, int last, int searchItem){// returns index of desired string
		int middle = (first + last) / 2;
		if (subwords[middle].stringPriority == searchItem){
			return middle;
		}
		else if (subwords[middle].stringPriority < searchItem){
			return bSearch(middle +1, last, searchItem);
		}
		else if (subwords[middle].stringPriority > searchItem){
			return bSearch(first, middle -1, searchItem);
		}
		return -1;
	
	}

	// Public accessor to recursive quicksort
	void quicksort(){// sorts based on frequency

			//testPartition();
			//cout << "---------------" << endl;
			//testSort();

		qsort(subwords, 0, subwords.size());
	}

	// Public accessor to recursive string quicksort
	void stringSort(){// sorts based on string
		stringQsort(subwords, 0, subwords.size());
	}


private:////////////////////////// PRIVATE METHODS ///////////////////////////////////////////

		//partitioning component of quicksort, by string
		int stringPartition(vector<subword>& A, int left, int right, int pivot) {
			for (int i = left; i<right; ++i) {
				if (A[i].stringPriority <= pivot) {
					swap(A[i], A[left]);
					left++;
				}
			}
			return left - 1;
		}
			// quicksort by string value
		void stringQsort(vector<subword>& A, int left, int right) {
			if (left >= right) return;

			int middle = left + (right - left) / 2;
			swap(A[middle], A[left]);
			int midpoint = stringPartition(A, left + 1, right, A[left].stringPriority);
			swap(A[left], A[midpoint]);
			stringQsort(A, left, midpoint);
			stringQsort(A, midpoint + 1, right);
		}

		// partitioning component of quicksort by frequency
		int partition(vector<subword>& A, int left, int right, int pivot) {
			for (int i = left; i<right; ++i) {
				if (A[i].frequency <= pivot) {
					swap(A[i], A[left]);
					left++;
				}
			}
			return left - 1;
		}
		// quicksort by frequency value
		void qsort(vector<subword>& A, int left, int right) {
			if (left >= right) return;

			int middle = left + (right - left) / 2;
			swap(A[middle], A[left]);
			int midpoint = partition(A, left + 1, right, A[left].frequency);
			swap(A[left], A[midpoint]);
			qsort(A, left, midpoint);
			qsort(A, midpoint + 1, right);
		}



		//void testPartition() {
		//	int elements[] = { 1, 3, 1, 1, 3 };
		//	vector<int> A(elements, elements + 5);
		//	int n = partition(A, 0, 5, 1);
		//	cout << n << endl;
		//	printVector(A);
		//}

		//void testSort() {
		//	int elements[] = { 1, 12, 2, 2, 2, 6, 20, 22 };
		//	vector<int> A(elements, elements + 8);
		//	qsort(A, 0, A.size());
		//	printVector(A);
		//}



};
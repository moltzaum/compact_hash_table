#include <iostream>
#include <string>
#include <list>
#include <fstream>

//These include statements are for an interesting word frequency feature. I am using a
//map here because of the easy iterator it provides. I also figured it would be ok since
//I'm using the map for a portion which does not fit in the original requirements
#include <map>
#include <utility>	//pair
#include <vector>	//for frequency vector

using namespace std;
typedef list<string> Bucket;
typedef Bucket* HashTable;

bool is_an_int(string str) {
	for (int i = 0; i < str.size(); i++)
		if (!isdigit(str[i])) return false;
	return true;
}

//returns false if the token isn't found
bool foundToken(Bucket bucket, string token) {
	for (auto it = bucket.begin(); it != bucket.end(); it++)
		if (*it == token) return true;
	return false;
}

int main() {
		
	//Word count is the number of all tokens that are separated by whitespace, excluding
	//"words" that are completely non-alphanumeric. "--" is one such "word".
	int wordCount = 0;
	int dictWordCount = 0;
	int misspelledCount = 0;
	string token;
	
	//Changing the number of buckets changes which file I will read in. I included this
	//because I thought dict.dat.txt had too few words. I got words.txt from
	//"/usr/share/dict/words", then I created a program to go through each "misspelled"
	//word to verify whether it was really misspelled (though this dictionary isn't 
	//perfect either). Many of the misspelled words are from dialoge, or are pronouns.
	//1373 is from the original specification. 12907 was chosen for the larger dictionary
	const int SIZE = (true ? 1373 : 12907);
	
	//READ IN DICTIONARY
	//Bucket hashTable[SIZE]; //for a static array
	//If you want to call it a HashTable using the typedef it needs to be a dynamic array
	HashTable hashTable = new Bucket[SIZE];
	hash<string> hashFunctor;
	string fileName = (SIZE == 1373 ? "dict.dat.txt" : "myDictionary.txt");
	ifstream dictFile(fileName);
	if (!dictFile) {
		cout << "There was an error opening the file." << endl;
		return 1;
	}
	while (dictFile >> token) {
		dictWordCount++;
		hashTable[hashFunctor(token) % SIZE].push_back(token);
	} dictFile.close();
	
	cout << "What is the name of the file to spell check?" << endl;
	getline(cin, fileName);
	ifstream in(fileName);
	if (!in) {
		cout << "There was an error opening the file." << endl;
		return 1;
	}
	
	//Here you can uncomment the variable wordCounts (and some code below) to
	//determine the frequency of each word. You can specifically allow either
	//correctly spelled, or incorrectly spelled words
	//map<string, int> wordCounts;
	while (in >> token) {
		//erase the back as long as the back is not an alphanumeric character and
		//the string isn't empty. (I iterate for quotations and parentheses mostly).
		//I say while !token.empty() because "--" will go to ""
		while (!token.empty() && !isalnum(token[0])) token.erase(0, 1);
		while(!token.empty() && !isalnum(token.back())) token.pop_back();
		for (int i = 0; i < token.size(); i++) token[i] = tolower(token[i]); //to lowercase
		if (token.empty()) continue;
		wordCount++;
		//numbers aren't included in the dictionary file, so continue here
		if (is_an_int(token)) continue;
		
		Bucket bucket = hashTable[hashFunctor(token) % SIZE];
		if (foundToken(bucket, token)) {
			//incrementing a map like this works because the first time a map is
			//called it will return 0 (for integers). map[key]++ translates to
			//map[key] = map[key] + 1, which is really saying map[key] = 0 + 1
			//wordCounts[token]++;
		} else {
			//We can figure out how many unique misspelled words there are by
			//using this map. If we iterate it, the number of times we need to
			//iterate is the number of unique misspelled words 
			//wordCounts[token]++;
			misspelledCount++;
			cout << "\"" << token << "\" could be misspelled." << endl;
		} 
	} in.close();
	
	cout << "There are " << dictWordCount << " words in the dictionary." << endl;
	cout << "There are " << wordCount << " words in the file." << endl;
	cout << "There are " << misspelledCount << " (non-unique) misspelled words in the file." << endl;
	
	/*
	ofstream out("frequency.txt");
	vector<pair<int, string>> frequency;
	for (auto it = wordCounts.begin(); it != wordCounts.end(); it++)
		frequency.push_back(make_pair(it->second, it->first));
	sort(frequency.rbegin(), frequency.rend());
	for (int i = 0; i < frequency.size(); i++)
		out << frequency[i].first << '\t' << frequency[i].second << endl;
	out.close();
	//*/
	
	delete [] hashTable;
}

//The dict.dat.txt dictionary uses 14.6% of words that are considered to be in use:
//(https://en.oxforddictionaries.com/explore/how-many-words-are-there-in-the-english-language)


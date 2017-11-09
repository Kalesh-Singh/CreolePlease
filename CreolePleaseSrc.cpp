#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <ctime>
#include <bitset>

using namespace std;

// This function converts an entire string to lowercase character by character.
string ConvertStrToLower(string input) {
	string output = input;
	for (unsigned int i = 0; i < input.length(); i++) {
		output[i] = tolower(output[i]);
	}
	return output;
}

// This function takes a string of words and separates them into a vector of strings.
vector<string> StringToVectorOfWords(string input) {
	vector<string> Words;
	string new_word = "";

	for(unsigned int i = 0; i < input.length(); i++){
		if(input[i] == ' ' && new_word.length() == 0){
			continue;
		}
		else if(input[i] == ' ' && new_word.length() != 0){
			Words.push_back(new_word);
			new_word = "";
			continue;
		}

		new_word += string(1, input[i]);

		if(i == input.length()-1)
			Words.push_back(new_word);
	}
	return Words;
}

// This function removes punctuation from the end of a word and returns an integer code
// that signifies what punctuation should be appended to the end of the string once 
// replacements are made. 
int RemovePunctuation(string &input) {
	unsigned int punctuation_tracker = 0;

	if (input[input.length() - 1] == ',') {
		punctuation_tracker = 1;
	}
	else if (input[input.length() - 1] == '.') {
		punctuation_tracker = 2;
	}

	string output = "";
	for (char c : input) {
		if (isalnum(c))
			output += c;
	}
	input = output;

	return punctuation_tracker;
}

// This function aims to provide some level of variability to the generated speech.
// Given a input string and a map to possible replacements of that string this function 
// generates a random number from 0 to the size of the vector of replacement. The random 
// number becomes the index which selects which option to return or possibly if the random
// number is equal to the size of the array then the original word is left. 
string DetermineWord(string input, unordered_map<string, vector<string>> Words) {
	unsigned int size_of_vector = (Words.find(input)->second).size();
	unsigned int random_number = rand() % (size_of_vector + 1);
	
	if (random_number == (size_of_vector)){
		return (Words.find(input)->first);
	}
	else{
		return (Words.find(input)->second)[random_number];
	}
}

// This function attempts to replace single words with one of their possible replacements.
// If a word with punctuation removed is found to be in the map of words, it is replaced with
// one of its potential alternatives or left untouched. The punctuation teacker is used to append 
// the original punctuation at the end of the word once replacements are made.
vector<string> CheckMapAndReplace(vector<string> input, unordered_map<string, vector<string>> Words) {
	vector<string>::iterator it;
	string word = "";
	unsigned int punctuation_tracker = 0;

	for (it = input.begin(); it != input.end(); it++) {
		punctuation_tracker = RemovePunctuation((*it));
		if (Words.count((*it)) != 0) {
			word = DetermineWord((*it), Words);
			(*it) = word;
		}
		switch (punctuation_tracker) {
		case 1:
			(*it) += ",";
			break;
		case 2:
			(*it) += ".";
			break;
		default:
			break;
		};
	}
	return input;
}

// This function is used to reconstruct a string from a vector of strings it simply 
// concatenates the strings in the vectors with spaces between. For readability on the terminal
// a newline character is inserted approximately every 100 characters.
string ReconstructString(vector<string> input) {
	string output = "";
	unsigned int line_length = 0;
	for (auto word : input) {
		output += word;
		line_length += word.length();
		output += " ";
		if (line_length > 100) {
			output += "\n";
			line_length = 0;
		}
	}
	return output;
}

// This is the main function that performs a sequence of modifications and function calls on the 
// input string to convert it to Guyanese Creolese.
string CreolePlease(string input, unordered_map<string, vector<string>> WordConversions) {
	string output = "";
	vector<string> Words;
	vector<string>::iterator it;

	output = input;
	output = ConvertStrToLower(output);
	Words = StringToVectorOfWords(output);
	Words = CheckMapAndReplace(Words, WordConversions);
	output = ReconstructString(Words);
	return output;
}

// This function reads text from an input file until the end by appending each getline to string
// and returning the final string. 
string GetTextInputFromFile() {
	string user_input = "";
	string read_variable = "";

	ifstream infile;
	infile.open("infile.txt");

	while (!infile.eof()) {
		getline(infile, read_variable);
		user_input += read_variable + " ";
	}
	infile.close();
	return user_input;
}

// This function reads in word mappings in the for (key) : {values}. This allows for easy assignment
// and mapping via a text file. It opens the wordfile and reads lines until it encounter and eof. Each
// line is sent toa function that converts it into a vector of words. The first element of the vector
// is then used as a key and the remaining items are used as the values in the wordConversions map.
void GetWordMappingsFromFile(unordered_map<string, vector<string>> &WordConversions){
	ifstream wordfile;
	wordfile.open("wordfile.txt");

	string current_line = "";
	string key = "";
	vector<string> line_vector;

	while(true){
		getline(wordfile, current_line);

		if(wordfile.eof())
			break;

		line_vector = StringToVectorOfWords(current_line);
		key = line_vector[0];
		line_vector.erase(line_vector.begin());
		WordConversions[key] = line_vector;
	}
	wordfile.close();
	return;
}

int main() 
{
  	// TODO Phrase and Idiomatic Manipulations
	//do you =-> you
	// was not -> ain went
	// very bad -> bad bad
	unordered_map<string, vector<string>> WordConversions;
	GetWordMappingsFromFile(WordConversions);

	srand(time(NULL));

	string user_input = GetTextInputFromFile();
	cout << CreolePlease(user_input, WordConversions);
	return 0;
} 

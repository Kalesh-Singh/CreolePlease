#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <ctime>
#include <bitset>

using namespace std;

//Function Protoypes
string ConvertStrToLower(string input);
vector<string> StringToVectorOfWords(string input); 
int RemovePunctuation(string &input);
string DetermineWord(string input, unordered_map<string, vector<string>> Words); 
vector<string> CheckMapAndReplaceWords(vector<string> input, unordered_map<string, vector<string>> Words);
vector<string> CheckMapAndReplaceEndings(vector<string> input, unordered_map<string, vector<string>> Endings);
string ReconstructString(vector<string> input); 
string CreolePlease(string input, unordered_map<string, vector<string>> WordConversions, unordered_map<string, vector<string>> EndingsConversions); 
string GetTextInputFromFile();
void GetMappingsFromFile(unordered_map<string, vector<string>> &Conversions, string filename);
void WriteOutputToFile(string input);

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
vector<string> CheckMapAndReplaceWords(vector<string> input, unordered_map<string, vector<string>> Words) {
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


// This function checks the endings of words in the vector created from the input file. If an ending match is
// found then it is replaced with the associated value in the Endings map.
vector<string> CheckMapAndReplaceEndings(vector<string> input, unordered_map<string, vector<string>> Endings){
	vector<string>::iterator vec_it;
	unordered_map<string, vector<string>>::iterator map_it;
	int match_index = 0; 
	
	// For every string in the vector
	for(vec_it = input.begin(); vec_it != input.end(); vec_it++){
		// For every key in the map
		for(map_it = Endings.begin(); map_it != Endings.end(); map_it++){
			//If the key is found within the word
			match_index = (*vec_it).find(map_it->first);
			if(match_index != -1){
				//Words without punctuation at the end remove last chars and add new ending.
				if(match_index + (map_it->first).length() == (*vec_it).length()){
					(*vec_it).erase((*vec_it).begin()+match_index, (*vec_it).end());
					(*vec_it) += map_it->second[0];
				}	
			}
		}			
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
string CreolePlease(string input, unordered_map<string, vector<string>> WordConversions, unordered_map<string, vector<string>> EndingsConversions) {
	string output = "";
	vector<string> Words;
	vector<string>::iterator it;

	output = input;
	output = ConvertStrToLower(output);
	Words = StringToVectorOfWords(output);
	Words = CheckMapAndReplaceEndings(Words, EndingsConversions); 
	Words = CheckMapAndReplaceWords(Words, WordConversions);
	output = ReconstructString(Words);
	WriteOutputToFile(output);
	return output;
}

// This function reads text from an input file until the end by appending each getline to string
// and returning the final string. 
string GetTextInputFromFile(string filename) {
	string user_input = "";
	string read_variable = "";

	ifstream file;
	file.open("infile.txt");

	while (!file.eof()) {
		getline(file, read_variable);
		user_input += read_variable + " ";
	}
	file.close();
	return user_input;
}

// This function reads in word mappings in the for (key) : {values}. This allows for easy assignment
// and mapping via a text file. It opens the wordfile and reads lines until it encounter and eof. Each
// line is sent toa function that converts it into a vector of words. The first element of the vector
// is then used as a key and the remaining items are used as the values in the wordConversions map.
void GetMappingsFromFile(unordered_map<string, vector<string>> &Conversions, string filename){
	ifstream file;
	file.open(filename);

	string current_line = "";
	string key = "";
	vector<string> line_vector;

	while(true){
		getline(file, current_line);

		if(file.eof())
			break;
	
		line_vector = StringToVectorOfWords(current_line);
		key = line_vector[0];
		line_vector.erase(line_vector.begin());
		Conversions[key] = line_vector;	
	}
	file.close();
	return;
}

// This function takes the converted string created by the program and writes it to a file.
void WriteOutputToFile(string input){
	ofstream outfile;
	outfile.open("outfile.txt");
	outfile << input;
	outfile.close();
	return;
}

int main() 
{
  	// TODO Phrase and Idiomatic Manipulations
	//do you =-> you
	// was not -> ain went
	// very bad -> bad bad
	//TODO allow stringtovector to separate punctuations

	unordered_map<string, vector<string>> WordConversions;
	unordered_map<string, vector<string>> EndingsConversions;

	GetMappingsFromFile(WordConversions, "words.txt");
	GetMappingsFromFile(EndingsConversions, "endings.txt");

	srand(time(NULL));

	string user_input = GetTextInputFromFile("infile.txt");
	CreolePlease(user_input, WordConversions, EndingsConversions);
	return 0;
}

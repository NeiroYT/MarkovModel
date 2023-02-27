#include "head.h"

int searchforID(vector<Dictionary> &markov_model, string words);
void load_in_markov(vector<Dictionary> &markov_model, vector<string> &converted_input);
string doSentence(vector<Dictionary> &markov_model, int startPos = 0, int count = 0);

const int MAX_WORDS_LIMIT = 2000; // to prevent 1-word infinite loop

int main(int argc, char **argv) {
	//ofstream plog;
	//plog.open("log.txt", ios::app);
	string fname;
	string fname2;
	if (argc > 1) {
		fname2 = fname = argv[1];
		fname.append(".dat");
		fname2.append(".txt");
	}
	else {
		fname = "main.dat";
	}
	if (fname2 == "output.txt") {
		return 1;
	}
	int saveresult = 0; // was saved?
	int loadresult = 0; // was loaded?
	int waschanged = 0; // was changed?
	srand(time(NULL));
	setlocale(0, "");
	vector<Dictionary> markov_model;
	loadresult = loadmarkov(markov_model, fname);
	//plog << loadmarkov(markov_model, fname) << endl;
	// update data
	vector<string> input;
	ifstream f;
	f.open(fname2);
	if (f.good()) {
		string a;
		char data[256];
		while (!f.eof()) {
			f.getline(data, 256);
			a = data;
			if (a == "") {
				continue;
			}
			input.push_back(a);
			if (a == "%END%") {
				if (input.size() > 2) {
					load_in_markov(markov_model, input);
					waschanged = 1;
				}
				input.clear();
			}
		}
	}
	f.close();
	// 

	// main moves
	if (argc > 4 && atoi(argv[2]) == 1 && markov_model.size() > 0) {
		int startpos;
		int length;
		startpos = atoi(argv[3]);
		length = atoi(argv[4]);
		string a;
		if (startpos == -1) {
			startpos = rand() % (markov_model.size());
		}
		a = doSentence(markov_model, startpos, length);
		ofstream output;
		output.open("output.txt");
		if (output.good()) {
			output << a;
		}
		output.close();
	}
	//
	if (waschanged) {
		// save
		saveresult = savemarkov(markov_model, fname);
		if (!loadresult) {
			string bckup = fname + ".bckup";
			savemarkov(markov_model, bckup);
		}
		//plog << saveresult << endl;
		if (!saveresult) {
			ofstream f1;
			f1.open(fname2);
			f1.clear();
			f1.close();
		}
	}
	//plog.close();
	return 0;
}

int searchforID(vector<Dictionary> &markov_model, string words) {
	for (size_t j = 0; j < markov_model.size(); j++) {
		if (markov_model[j].words == words) {
			return j;
		}
	}
	return -1;
}

void load_in_markov(vector<Dictionary> &markov_model, vector<string> &converted_input) {
	int err = 0;
	int result = -1;
	int good = 0;
	for (size_t i = 0; i < converted_input.size() - 1; i++) {
		result = searchforID(markov_model, converted_input[i]);
		if (result != -1) {
			err = markov_model[result].update(converted_input[i + 1]);
			good = 1;
		}
		else {
			good = 0;
		}
		if (!good) {
			Dictionary a = Dictionary(converted_input[i]);
			err = a.update(converted_input[i + 1]);
			markov_model.push_back(a);
		}
	}
}

string doSentence(vector<Dictionary> &markov_model, int startPos, int count) {
	if (startPos > markov_model.size() - 1) {
		startPos = 0;
	}
	if (count < 0) {
		count = 0;
	}
	int counter = 0;
	string a = markov_model[startPos].words;
	string res = "";
	if (a != "%START%") {
		res = a;
		counter++;
	}
	if (count == 1) {
		return res;
	}
	int rint;
	if (count == 0) {
		while (a != "%END%") {
			if (res != "") {
				res += ' ';
			}
			a = markov_model[searchforID(markov_model, a)].return_random_words();
			if (a != "%END%") {
				res += a;
				counter++;
				if (counter == MAX_WORDS_LIMIT) {
					a = "%END%";
				}
			}
		}
		return res;
	}
	while (counter != count) {
		if (res != "") {
			res += ' ';
		}
		a = markov_model[searchforID(markov_model, a)].return_random_words();
		if (a == "%END%") {
			rint = rand() % (markov_model.size()-1) + 1;
			a = markov_model[rint].words;
		}
		res += a;
		counter++;
	}
	return res;
}
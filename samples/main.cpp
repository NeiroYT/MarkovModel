#include "head.h"

string extractinput(string filename);

int main(int argc, char **argv) {
	//ofstream plog;
	//plog.open("log.txt", ios::app);
	string dat_file;
	string txt_file;
	if (argc > 1) {
		txt_file = dat_file = argv[1];
		dat_file.append(".dat");
		txt_file.append(".txt");
	}
	else {
		dat_file = "main.dat";
	}
	if (txt_file == "output.txt") {
		return 1;
	}
	int saveresult = 0; // was saved?
	int loadresult = 0; // was loaded?
	int waschanged = 0; // was changed?
	srand(time(NULL));
	setlocale(0, "");
	MarkovModel markov_model;
	loadresult = markov_model.load(dat_file);
	// update data
	vector<string> input;
	ifstream f;
	f.open(txt_file);
	if (f.good()) {
		string cur_word;
		char data[1024];
		while (!f.eof()) {
			f.getline(data, 1024);
			cur_word = data;
			if (cur_word == "") {
				continue;
			}
			input.push_back(cur_word);
			if (cur_word == "%END%") {
				if (input.size() > 2) {
					markov_model.load_in(input);
					waschanged = 1;
				}
				input.clear();
			}
		}
	}
	f.close();
	// 

	// main moves
	if (argc > 4 && atoi(argv[2]) >= 1 && markov_model.size() > 0) {
		int startpos;
		int length;
		startpos = atoi(argv[3]);
		length = atoi(argv[4]);
		string res;
		string stwordfname;
		string sugword;
		if (argc > 5) {
			stwordfname = argv[5];
			sugword = extractinput(stwordfname);
			startpos = markov_model.searchforID(sugword);
		}
		if (startpos == -1) {
			startpos = rand() % (markov_model.size());
		}
		switch (atoi(argv[2])) {
		case 1:
			res = markov_model.doSentence(startpos, length);
			break;
		case 2:
			res = markov_model.doSentence2(startpos, length);
			break;
		default:
			res = markov_model.doSentence(startpos, length);
			break;
		}
		ofstream output;
		output.open("output.txt");
		if (output.good()) {
			output << res;
		}
		output.close();
	}
	//
	if (waschanged) {
		// save
		saveresult = markov_model.save(dat_file);
		if (!loadresult) {
			string bckup = dat_file + ".bckup";
			markov_model.save(bckup);
		}
		//plog << saveresult << endl;
		if (!saveresult) {
			ofstream f1;
			f1.open(txt_file);
			f1.clear();
			f1.close();
		}
	}
	//plog.close();
	return 0;
}

string extractinput(string filename) {
	ifstream file;
	string result;
	char data[50];
	filename += ".txt";
	file.open(filename, ios::in | ios::binary);
	file.getline(data, 50);
	result = data;
	file.close();
	return result;
}
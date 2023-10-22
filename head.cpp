#include "head.h"

Word_Freq Word_Freq::operator++(int n) {
	freq++;
	return *this;
}
Word_Freq Word_Freq::operator--(int n) {
	freq--;
	return *this;
}

int Dictionary::searchWordid(string inword) {
	for (size_t i = 0; i < wf.size(); i++) {
		if (wf[i].words == inword) {
			return i;
		}
	}
	return -1;
}

int Dictionary::update(string next) {
	for (size_t i = 0; i < wf.size(); i++) {
		if (wf[i].words == next) {
			(wf[i])++;
			allfreqs++;
			return 1;
		}
	}
	Word_Freq a = Word_Freq(next);
	wf.push_back(a);
	allfreqs++;
	return 0;
}

string Dictionary::return_random_words() {
	//srand(time(NULL));
	int rchoice = (rand() % (allfreqs))+1;
	int temp_count = 0;
	for (size_t i = 0; i < wf.size(); i++) {
		temp_count += wf[i].get_freq();
		if (temp_count >= rchoice) {
			return wf[i].words;
		}
	}
	return "NULL";
}

int savemarkov(vector<Dictionary> &markov_model, string file_name) {
	ofstream f;
	f.open(file_name, ios::out | ios::binary);
	if (!f.good()) {
		return 1;
	}
	f << 0xDEADBEEF << '\n';
	for (size_t i = 0; i < markov_model.size(); i++) {
		f << markov_model[i].wf.size() << '\n';
		for (size_t j = 0; j < markov_model[i].wf.size(); j++) {
			f << markov_model[i].wf[j].words << '\n';
			f << markov_model[i].wf[j].get_freq() << '\n';
		}
		f << markov_model[i].allfreqs << '\n';
		f << markov_model[i].words << '\n';
	}
	f.close();
	return 0;
}

int loadmarkov(vector<Dictionary> &markov_model, string file_name) {
	int tmplen = 0;
	char data[1024];
	int i = 0;
	int counter;
	string data2;
	ifstream f;
	f.open(file_name, ios::in | ios::binary);
	if (!f.good()) {
		return 1;
	}
	f.ignore(1000, '\n');
	while (!f.eof()) {
		f.getline(data, 50);
		if (strcmp(data, "") == 0) {
			break;
		}
		Dictionary a = Dictionary("NULL");
		markov_model.push_back(a);
		counter = atoi(data);
		i = counter;
		while (i > 0) {
			f.getline(data, 1024);
			data2 = data;
			markov_model[tmplen].update(data2);
			f.getline(data, 50);
			markov_model[tmplen].wf[counter-i].set_freq(atoi(data));
			i--;
		}
		markov_model[tmplen].allfreqs -= counter;
		f.getline(data, 50);
		markov_model[tmplen].allfreqs += atoi(data);
		f.getline(data, 1024);
		data2 = data;
		markov_model[tmplen].words = data2;
		tmplen++;
	}
	f.close();
	return 0;
}
#include "head.h"

Word_Freq Word_Freq::operator++(int n) noexcept {
	freq++;
	return *this;
}
Word_Freq Word_Freq::operator--(int n) noexcept {
	freq--;
	return *this;
}

int Dictionary::searchWordid(string inword) const {
	for (size_t i = 0; i < wf.size(); i++) {
		if (wf[i].get_word() == inword) {
			return i;
		}
	}
	return -1;
}

bool Dictionary::update(string next) {
	for (size_t i = 0; i < wf.size(); i++) {
		if (wf[i].get_word() == next) {
			(wf[i])++;
			allfreqs++;
			return true;
		}
	}
	Word_Freq a = Word_Freq(next);
	wf.push_back(a);
	allfreqs++;
	return false;
}

string Dictionary::return_random_words() const {
	//srand(time(NULL));
	int rchoice = (rand() % (allfreqs))+1;
	int temp_count = 0;
	for (size_t i = 0; i < wf.size(); i++) {
		temp_count += wf[i].get_freq();
		if (temp_count >= rchoice) {
			return wf[i].get_word();
		}
	}
	return "NULL";
}

int MarkovModel::save(string file_name) const {
	ofstream f;
	f.open(file_name, ios::out | ios::binary);
	if (!f.good()) {
		return 1;
	}
	f << 0xDEADBEEF << '\n';
	for (size_t i = 0; i < markov_model.size(); i++) {
		f << markov_model[i].wf.size() << '\n';
		for (size_t j = 0; j < markov_model[i].wf.size(); j++) {
			f << markov_model[i].wf[j].get_word() << '\n';
			f << markov_model[i].wf[j].get_freq() << '\n';
		}
		f << markov_model[i].allfreqs << '\n';
		f << markov_model[i].get_word() << '\n';
	}
	f.close();
	return 0;
}

int MarkovModel::load(string file_name) {
	int tmplen = 0;
	int i = 0;
	int counter;
	char data[1024];
	string data2;
	ifstream f;
	f.open(file_name, ios::in | ios::binary);
	if (!f.good()) {
		return 1;
	}
	f.getline(data, 11);
	data[10] = '\0';
	if (strcmp(data, "3735928559") != 0) {
		return 1;
	}
	while (!f.eof()) {
		f.getline(data, 50);
		if (strcmp(data, "") == 0) {
			break;
		}
		Dictionary a = Dictionary();
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
		markov_model[tmplen].set_word(data2);
		tmplen++;
	}
	f.close();
	return 0;
}

int MarkovModel::searchforID(string words) const {
	for (size_t j = 0; j < markov_model.size(); j++) {
		if (markov_model[j].get_word() == words) {
			return j;
		}
	}
	return -1;
}

void MarkovModel::load_in(vector<string> &converted_input) {
	int err = 0;
	int result = -1;
	for (size_t i = 0; i < converted_input.size() - 1; i++) {
		result = searchforID(converted_input[i]);
		if (result == -1) {
			Dictionary a = Dictionary(converted_input[i]);
			err = a.update(converted_input[i + 1]);
			markov_model.push_back(a);
		}
		else {
			err = markov_model[result].update(converted_input[i + 1]);
		}
	}
}

string MarkovModel::doSentence(int startPos, int count) const {
	if (startPos > markov_model.size() - 1) {
		startPos = 0;
	}
	if (count < 0) {
		count = 0;
	}
	int counter = 0; // words count
	string cur_word = markov_model[startPos].get_word();
	string res = "";
	if (cur_word != "%START%") {
		res = cur_word;
		counter++;
	}
	if (count == 1) {
		return res;
	}
	int idword = 0;
	if (count == 0) {
		while (cur_word != "%END%") {
			idword = searchforID(cur_word);
			if (res != "") {
				if (idword == -1) {
					res += '\n';
				}
				else {
					res += ' ';
				}
			}
			if (idword != -1) {
				cur_word = markov_model[idword].return_random_words();
			}
			else {
				cur_word = markov_model[0].return_random_words();
			}
			if (cur_word != "%END%") {
				res += cur_word;
				counter++;
				if (counter == MAX_WORDS_LIMIT) {
					cur_word = "%END%";
				}
			}
		}
		return res;
	}
	int counter2 = 0;
	int nextend = -1;
	int rint;
	float chance1 = 0.0f;
	float nextchance = 0.0f;
	while (cur_word != "%END%" && counter < 2 * count) {
		idword = searchforID(cur_word);
		if (res != "") {
			if (idword == -1) {
				res += '\n';
			}
			else {
				res += ' ';
			}
		}
		if (idword != -1) {
			int end_id;
			do {
				end_id = markov_model[idword].searchWordid("%END%");
				if (markov_model[idword].get_freqs() > 0 && markov_model[idword].freqfromid(end_id) != -1) {
					chance1 = markov_model[idword].freqfromid(end_id) / (float)markov_model[idword].get_freqs();
				}
				cur_word = markov_model[idword].return_random_words();
			} while (counter < count && cur_word == "%END%" && chance1 < 0.98f && markov_model[idword].size() > 2);
		}
		else {
			cur_word = markov_model[0].return_random_words();
		}
		if (cur_word == "%END%" && counter < count) {
			//rint = rand() % (markov_model.size() - 1) + 1;
			//a = markov_model[rint].words;
			do {
				//srand(time(NULL));
				rint = rand() % (markov_model.size() - 1) + 1;
				cur_word = markov_model[rint].get_word();
				nextend = markov_model[rint].searchWordid("%END%");
				nextchance = (float)markov_model[rint].freqfromid(nextend);
				if (nextchance != -1 && markov_model[rint].get_freqs() != 0) {
					nextchance = nextchance / (float)markov_model[rint].get_freqs();
				}
			} while (nextend != -1 && nextchance > 0.7f);
			//res += '\n';
			counter2 = 0;
		}
		if (cur_word != "%END%") {
			res += cur_word;
		}
		counter++; counter2++;
	}
	return res;
}
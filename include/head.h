#pragma once
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <cstring>

using namespace std;

class MarkovModel;
const int MAX_WORDS_LIMIT = 2000; // to prevent 1-word infinite loop

// pair of string and its frequency 
class Word_Freq {
public:
	Word_Freq(string iword = "NULL") noexcept {
		word = iword;
		freq = 1;
	};
	Word_Freq operator++(int n) noexcept;
	Word_Freq operator--(int n) noexcept;
	int get_freq() const noexcept { return freq; }
	void set_freq(int ifreq) noexcept { freq = ifreq; }
	void set_word(string to) noexcept { word = to; }
	const string& get_word() const noexcept { return word; }
private:
	int freq;
	string word;
};

// all next nodes after a string forms some dictionary 
class Dictionary {
public:
	friend class MarkovModel;
	// returns -1 if not found
	int searchWordid(string inword) const;
	// true if word already exists
	bool update(string next);
	Dictionary(string iword = "NULL") noexcept {
		word = iword;
		allfreqs = 0;
	}
	int count(string word) const {
		for (size_t i = 0; i < wf.size(); i++) {
			if (word == wf[i].get_word()) {
				return wf[i].get_freq();
			}
		}
		return 0;
	}
	int size() const noexcept {
		return wf.size();
	}
	// -1 if bad id
	int freqfromid(int id) const noexcept {
		if (id < 0 || id > wf.size() - 1) {
			return -1;
		}
		return wf[id].get_freq();
	}
	int get_freqs() const noexcept { return allfreqs; }
	const string& get_word() const noexcept { return word; }
	void set_word(string to) noexcept { word = to; }
	string return_random_words() const;
private:
	vector<Word_Freq> wf;
	int allfreqs;
	string word;
};

class MarkovModel {
public:
	size_t size() const { return markov_model.size(); }
	// 1 - error
	int save(string file_name) const;
	int load(string file_name);
	// -1 - not found
	int searchforID(string word) const;
	void load_in(vector<string> &converted_input);
	string doSentence(int startPos = 0, int count = 0) const;
	string doSentence2(int startPos = 0, int count = 0) const;
private:
	vector<Dictionary> markov_model;
};
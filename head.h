#pragma once
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>

using namespace std;

class Word_Freq {
public:
	string words;
	Word_Freq(string iwords = "NULL") {
		words = iwords;
		freq = 1;
	};
	Word_Freq operator++(int n);
	Word_Freq operator--(int n);
	int get_freq() {
		return freq;
	}
	void set_freq(int ifreq) {
		freq = ifreq;
	}
private:
	int freq;
};

class Dictionary {
public:
	string words;
	friend int savemarkov(vector<Dictionary> &markov_model, string file_name);
	friend int loadmarkov(vector<Dictionary> &markov_model, string file_name);
	int searchWordid(string inword);
	int update(string next);
	Dictionary(string iwords = "NULL") {
		words = iwords;
		allfreqs = 0;
	}
	int count(string word) {
		for (size_t i = 0; i < wf.size(); i++) {
			if (word == wf[i].words) {
				return wf[i].get_freq();
			}
		}
		return 0;
	}
	int countnode() {
		return wf.size();
	}
	int freqfromid(int id) {
		if (id < 0 || id > wf.size() - 1) {
			return -1;
		}
		return wf[id].get_freq();
	}
	int getfreqs() {
		return allfreqs;
	}
	string return_random_words();
private:
	vector<Word_Freq> wf;
	int allfreqs;
};

int savemarkov(vector<Dictionary> &markov_model, string file_name);
int loadmarkov(vector<Dictionary> &markov_model, string file_name);
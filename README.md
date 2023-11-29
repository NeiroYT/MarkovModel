# MarkovModel
## Main idea:  
name.txt -> name.dat (markov's chain)  

Compile and use from command prompt (Windows):  
``\...>markov.exe (name) (function) (startPos) (length of sentence)``  
  
*name*: name of your txt file (input)  
  
*functions*: 1 = doSentence (make sentence using startPos and length) -> output.txt (program saves just sentence)  
2 = doSentence2 (no forcing random words) -> output.txt (program saves just sentence)  
...  
  
*startPos*: first word in sentence (type -1 to use random word)  
  
*length of sentence*: amount of words (type 0 to go until the %END% word)  
  
# name.txt (your input) structure:  
%START%  
Your  
Text  
Is  
Here  
%END%  
%START%  
...  
%END%  
  
do your input using parsing  
  
# name.dat structure:  
0xDEADBEEF  
(amount of words)  
(word1_word)  
(word1_freq)  
(word2_word)  
(word2_freq)  
...  
(sum of freqs)  
(mainword_word)  
...repeat  

this means program saw  
mainword word1, mainword word2, ...  
and made a model  
  

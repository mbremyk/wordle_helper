#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <regex>

using namespace std;

string guesses[6];
char known_pos[5];
vector<char> known_chars;
vector<string> words;
vector<char> disallowed;

void reset();
int main_loop();
bool read_words(string wordfile);
int guess_word(string guess);
regex create_regex(int num_guesses);
bool get_feedback(string guess);
vector<string> find_matches(vector<string> word_list, regex re);

int main() {
    if(!read_words("words")) {
        cout << "File didn't open fuck you" << endl;
        exit(1);
    }
    reset();

    return main_loop();
}

void reset() {
    fill(guesses, guesses + 6, "");
    fill(known_pos, known_pos + 5, 0);
    disallowed.clear();
    known_chars.clear();
}

int main_loop() {
    while(true) {
        reset();
        string guess;
        int num_guesses = 0;
        vector<string> possible_words = words;
        cout << "Wordle word helper" << endl;
        
        while(num_guesses < 6) {
            cout << "Enter a five letter word (guess number " << num_guesses + 1 << "): ";
            cin >> guess;
            if(!guess.compare("l")) {
                if(possible_words.size() > 50) {
                    cout << "More than 50 words match your current guesses. Are you sure you want to list them all? [y/N]";
                    string ans;
                    cin >> ans;
                    if(ans.compare("y")) {
                        continue;
                    }
                }
                for(vector<string>::iterator p = possible_words.begin(); p != possible_words.end(); ++p) {
                    cout << *p << '\t';
                }
                cout << endl;
                continue;
            }
            transform(guess.begin(), guess.end(), guess.begin(), ::toupper);
            switch(guess_word(guess)) { 
                case 0:
                    guesses[num_guesses] = guess;
                    ++num_guesses;
                    break;
                case 1:
                    cout << "Word needs to be five alphabetic letters" << endl;
                    continue;
                case 2:
                    cout << "Word already guessed" << endl;
                    continue;
                case 3:
                    cout << "Word not in word list" << endl;
                    continue;
            }
            while(!get_feedback(guess));
            
            regex re = create_regex(num_guesses);
            possible_words = find_matches(possible_words, re);
            cout << "Found " << possible_words.size() << " possible matches" << endl;
            cout << "Write 'l' instead of a guess to list possible matches" << endl;
        }
    }
}

bool read_words(string wordfile) {
    ifstream word_file;
    word_file.open(wordfile, ios::in);

    if(word_file.is_open()) {
        string word;
        while(getline(word_file, word)) {
            words.push_back(word);
        }
        word_file.close();
    }
    else{
        return false;
    }
    return true;
}

int guess_word(string guess) {
    if(!regex_match(guess, regex("^[A-Z]{5}$"))) return 1;
    if(find(guesses, guesses + 6, guess) != guesses + 6) return 2;
    if(!(find(words.begin(), words.end(), guess) != words.end())) return 3;
    return 0;
}

regex create_regex(int num_guesses) {
    string re = "^";
    for(vector<char>::iterator p = known_chars.begin(); p != known_chars.end(); ++p) {
        string t = "(?=.*";
        t += *p;
        t += ')';
        re += t;
    }
    re += '(';
    for (int i = 0; i < 5; ++i) {
        if(known_pos[i]) {
            re += known_pos[i];
            continue;
        }
        vector<char> temp_disallowed;
        for(int j = 0; j < num_guesses; ++j) {
            if(find(known_chars.begin(), known_chars.end(), guesses[j][i]) != known_chars.end()) temp_disallowed.emplace_back(guesses[j][i]);
        }
        string c = "[^";
        for(vector<char>::iterator p = disallowed.begin(); p < disallowed.end(); ++p) c += *p;
        for(vector<char>::iterator p = temp_disallowed.begin(); p != temp_disallowed.end(); ++p) c += *p;
        c += ']';
        re += c;
    }
    re += ')';
    re += '$';
    //cout << re << endl;
    return regex(re);
}

bool get_feedback(string guess) {
    cout << "Which were correct/wrong? (0=wrong, 1=misplaced, 2=right; 5 numbers[XXXXX]): ";
    string feedback;
    cin >> feedback;
    if(!regex_match(feedback, regex("^[012]{5}$"))) {
        return false;
    }
    for (int i = 0; i < 5; ++i) {
        switch(feedback[i]) {
            case '0':
                if((find(disallowed.begin(), disallowed.end(), guess[i]) == disallowed.end()) && (find(known_chars.begin(), known_chars.end(), guess[i]) == known_chars.end())) disallowed.emplace_back(guess[i]);
                break;
            case '1':
                if(!(find(known_chars.begin(), known_chars.end(), guess[i]) != known_chars.end()) && !(find(known_pos, known_pos + 5, guess[i]) != known_pos + 5)) known_chars.emplace_back(guess[i]);
                break;
            case '2':
                //if((find(disallowed.begin(), disallowed.end(), guess[i]) == disallowed.end())) disallowed.emplace_back(guess[i]);
                //remove(known_chars.begin(), known_chars.end(), guess[i]);
                known_pos[i] = guess[i];
                break;
        }
    }
    return true;
}

vector<string> find_matches(vector<string> word_list, regex re) {
    vector<string> m;
    for(vector<string>::iterator p = word_list.begin(); p != word_list.end(); ++p) {
        if(regex_match(*p, re)) m.emplace_back(*p);
    }
    return m;
}
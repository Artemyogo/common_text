#include<map>
#include<vector>
#include<string>

#define ll long long

using namespace std;

vector<double> get_char_percent(const vector<string>& st); // процент символов
int get_avg_len_of_words(const vector<string>& st); //средняя длина слов
int get_avg_len_of_sentences(const vector<string>& st); //средняя длина предложений
int get_avg_len_of_pharagraphs(const vector<string>& st); //средняя длина абзацев
map<ll, int> get_count_of_words(const vector<string>& st); //кол-во всех слов(ХЕШИ)
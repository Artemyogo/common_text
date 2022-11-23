#include "factors.h"
#include "neurons.h"

using namespace std;

void input(vector<string>& st, string open) {
	ifstream fin;
	fin.open(open);

	string str;

	while (getline(fin, str))
		st.push_back(str);
}

int main() {
	srand(time(NULL));
	ofstream fout;
	ifstream fin;

	fout.open("log.txt");

	const string first = "first", second = "second", res = "res";

	const int l = 3, n = 100;
	double ra = 0;

	int size[l] = { input_size, 10, 1};
	network nn;

	vector<string> st1, st2;
	
	double* linput = new double[input_size];

	int right_res;
	for (int e = 1; ra / n * 100 < 99; e++) {
		fout << "Epoch " << e << '\n';

		ra = 0;
		for (int i = 1; i <= n; i++) {
			string suf = to_string(i) + ".txt";
			input(st1, first + suf);
			input(st2, second + suf);
			fin.open(res + suf);
			fin >> right_res;

			vector<double> p1 = get_char_percent(st1);
			vector<double> p2 = get_char_percent(st2);

			for (int j = 0; j < size1; j++)
				linput[j] = abs(p1[j] - p2[j]);

			linput[input_size] = abs(get_avg_len_of_words(st1) - get_avg_len_of_words(st2))/20.;
			linput[input_size + 1] = abs(get_avg_len_of_sentences(st1) - get_avg_len_of_sentences(st2)) / 1000.;
			linput[input_size + 2] = abs(get_avg_len_of_pharagraphs(st1) - get_avg_len_of_pharagraphs(st2)) / 100000.;

			nn.set_input(linput);

			int result = nn.ForwardFeed();
		}
	}
}
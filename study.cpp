#include "factors.h"
#include "neurons.h"
#include "study.h"
#include "setting.h"

using namespace std;



void input(vector<string>& st, string open) {
	ifstream fin;
	fin.open(open);

	string str;

	while (getline(fin, str))
		st.push_back(str);
}

double* create_input(vector<string>& st1, vector<string>& st2) {
	double* linput = new double[input_size];

	vector<double> p1 = get_char_percent(st1);
	vector<double> p2 = get_char_percent(st2);

	for (int j = 0; j < size1; j++)
		linput[j] = abs(p1[j] - p2[j]);

	linput[input_size] = abs(get_avg_len_of_words(st1) - get_avg_len_of_words(st2)) / 20.;
	linput[input_size + 1] = abs(get_avg_len_of_sentences(st1) - get_avg_len_of_sentences(st2)) / 1000.;
	linput[input_size + 2] = abs(get_avg_len_of_pharagraphs(st1) - get_avg_len_of_pharagraphs(st2)) / 100000.;

	return linput;
}

void study() {
	srand(time(NULL));
	ofstream fout;
	ifstream fin;

	fout.open("log.txt");

	const string first = "first", second = "second", res = "res";
	const int n = 100;
	double ra = 0;

	network nn;

	nn.setLayers(l, sizes);

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

			linput = create_input(st1, st2);

			nn.set_input(linput);

			int result = nn.ForwardFeed();

			if (result == right_res) {
				fout << "Guess " << i << "\t\t\t****\n";
				ra++;
			}
			else {
				double BP_start = clock();
				nn.BackPropogation(result, right_res, 0.5);
				double BP_stop = clock();
			}
		}
		cout << e << ") Right answers: " << ra / n * 100 << '\n';
	}
	if (nn.SaveWeights("weights.txt")) {
		cout << "Weights saved!\n";
	}
}
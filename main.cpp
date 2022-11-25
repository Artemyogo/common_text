#include <string>
#include <vector>
#include "study.h"
#include "neurons.h"
#include "setting.h"

using namespace std;

int main() {
	const bool to_study = 1;
	const string name1 = "", name2 = "";


	vector<string> st1, st2;

	input(st1, string(name1 + ".txt"));
	input(st2, string(name2 + ".txt"));

	network nn;

	double* linput;

	linput = create_input(st1, st2);

	if (to_study)
		study();
	else
		nn.setLayersNotStudy(l, sizes);

	nn.set_input(linput);

	double res = nn.ForwardFeed();

	cout << res;
}
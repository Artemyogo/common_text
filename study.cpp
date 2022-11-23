#include "factors.h"
#include "neurons.h"

using namespace std;

int main() {
	srand(time(NULL));
	ifstream fin;
	ofstream fout;
	fout.open("log.txt");

	const int l = 3, n = 100;
	double ra = 0;

	int size[l] = { input_size, 10, 1};
	network nn;

	for (int e = 1; ra / n * 100 < 98; e++) {
		fout << "Epoch " << e << '\n';
			
	}
}
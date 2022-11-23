#pragma once
#include<fstream>
#include<thread>
#include<random>
#include<vector>
#include<time.h>
#include<Windows.h>
#include<iostream>

using namespace std;

const int ipnut_size = 64;

struct neuron {
	double value;
	double error;
	void act() {
		value = (1 / (1 + pow(2.71828, -value)));
	}
};

struct data_one {
	double info[ipnut_size];
	char rresult;
};

class network {
public:
	int layers;
	int* size;
	bool predict(double x) {
		if (x >= 0.9) {
			return 1;
		}
		else {
			return 0;
		}
	}
	void setLayersNotStudy(int n, int* p) {
		ifstream fin;
		fin.open("weights.txt");
		srand(time(NULL));
		layers = n;
		neurons = new neuron * [n];
		weights = new double** [n - 1];
		size = new int[n];
		for (int i = 0; i < n; i++) {
			size[i] = p[i];
			neurons[i] = new neuron[p[i]];
			if (i < n - 1) {
				weights[i] = new double* [p[i]];
				for (int j = 0; j < p[i]; j++) {
					weights[i][j] = new double[p[i + 1]];
					for (int k = 0; k < p[i + 1]; k++) {
						fin >> weights[i][j][k];
						//cout << weights[i][j][k];
					}
				}
			}
		}
	}
	void setLayers(int n, int* p) {
		srand(time(NULL));
		layers = n;
		neurons = new neuron * [n];
		weights = new double** [n - 1];
		size = new int[n];
		for (int i = 0; i < n; i++) {
			size[i] = p[i];
			neurons[i] = new neuron[p[i]];
			if (i < n - 1) {
				weights[i] = new double* [p[i]];
				for (int j = 0; j < p[i]; j++) {
					weights[i][j] = new double[p[i + 1]];
					for (int k = 0; k < p[i + 1]; k++) {
						weights[i][j][k] = ((rand() % 100)) * 0.01 / size[i];
					}
				}
			}
		}
	}
	void setRandomInput() {
		for (int i = 0; i < size[0]; i++) {
			neurons[0][i].value = (rand() % 256) / 255;
		}
	}
	void set_input(double* p) {
		for (int i = 0; i < size[0]; i++) {
			neurons[0][i].value = p[i];
		}
	}
	void show() {
		cout << "Neural network architecture: ";
		for (int i = 0; i < layers; i++) {
			cout << size[i];
			if (i < layers - 1) {
				cout << " - ";
			}
		}cout << endl;
		for (int i = 0; i < layers; i++) {
			cout << "\n#Layer " << i + 1 << "\n\n";
			for (int j = 0; j < size[i]; j++) {
				cout << "Neuron #" << j + 1 << ": \n";
				cout << "Value: " << neurons[i][j].value << endl;
				if (i < layers - 1) {
					cout << "Weights: \n";
					for (int k = 0; k < size[i + 1]; k++) {
						cout << "#" << k + 1 << ": ";
						cout << weights[i][j][k] << endl;
					}
				}
			}
		}
	}

	double ForwardFeed() {
		for (int i = 1; i < layers; i++)
			ForwardFeeder(i, 0, size[i]);
		double max = 0;
		double prediction = 0;
		for (int i = 0; i < size[layers - 1]; i++) {

			if (neurons[layers - 1][i].value > max) {
				max = neurons[layers - 1][i].value;
				prediction = i;
			}
		}
		return prediction;
	}
	void show_res() {
		for (int i = 0; i < size[layers - 1]; i++)
			std::cout << char(i + 65) << " : " << neurons[layers - 1][i].value << " ";
		cout << '\n';
	}

	void BackPropogation(double prediction, double rresult, double lr) {
		for (int i = layers - 1; i > 0; i--)
			ErrorCounter(i, 0, size[i], prediction, rresult, lr);
		//thread th1([&]() {ErrorCounter(i, 0, size[i], prediction, rresult, lr);});
		for (int i = 0; i < layers - 1; i++) {
			WeightsUpdater(0, size[i], i, lr);
		}
	}

	bool SaveWeights(string filename) {
		ofstream fout;
		fout.open(filename);
		for (int i = 0; i < layers; i++) {
			if (i < layers - 1) {
				for (int j = 0; j < size[i]; j++) {
					for (int k = 0; k < size[i + 1]; k++) {
						fout << weights[i][j][k] << " ";
					}
				}
			}
		}
		fout.close();
		return 1;
	}
private:
	neuron** neurons;
	double*** weights;
	double sigm_pro(double x) {
		if ((fabs(x - 1) < 1e-9) || (fabs(x) < 1e-9)) return 0.0;
		double res = x * (1.0 - x);
		return res;
	}
	void ForwardFeeder(int LayerNumber, int start, int stop) {
		for (int j = start; j < stop; j++) {
			for (int k = 0; k < size[LayerNumber - 1]; k++) {
				neurons[LayerNumber][j].value += neurons[LayerNumber - 1][k].value * weights[LayerNumber - 1][k][j];
			}
			neurons[LayerNumber][j].act();
		}
	}

	void ErrorCounter(int LayerNumber, int start, int stop, double prediction, double rresult, double lr) {
		if (LayerNumber == layers - 1) {
			for (int j = start; j < stop; j++) {
				if (j != int(rresult)) {
					neurons[LayerNumber][j].error = -pow((neurons[LayerNumber][j].value), 2);
				}
				else {
					neurons[LayerNumber][j].error = pow(1.0 - neurons[LayerNumber][j].value, 2);
				}
			}
		}
		else {
			for (int j = start; j < stop; j++) {
				double error = 0.0;
				for (int k = 0; k < size[LayerNumber + 1]; k++) {
					error += neurons[LayerNumber + 1][k].error * weights[LayerNumber][j][k];
				}
				neurons[LayerNumber][j].error = error;
			}
		}

	}
	void WeightsUpdater(int start, int stop, int i, double lr) {
		for (int j = 0; j < size[i]; j++) {
			for (int k = 0; k < size[i + 1]; k++) {
				weights[i][j][k] += lr * neurons[i + 1][k].error * sigm_pro(neurons[i + 1][k].value) * neurons[i][j].value;
			}
		}
	}

	void LayersCleaner(int LayerNumber, int start, int stop) {
		srand(time(0));
		for (int i = start; i < stop; i++) {
			neurons[LayerNumber][i].value = 0;
		}
	}
};



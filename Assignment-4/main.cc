#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <iomanip>
#include <utility>
#include <time.h>
#include <math.h>
#include <sys/time.h>
#include "function.h"

using namespace std;

int main(int argc, char* argv[]) {
	if (argc < 1) {
		cout << "Input Parameter Error!" << endl;
		return 1;
	}
	
	struct timeval start;
	struct timeval end;
	double time;

	//*****Open the file and read the data*****//
	int n, k;
	ifstream input(argv[1]);
	if (input.is_open() != 1) {
		cout << "Input File Open Error!" << endl;
		return 1;
	}
	string buf;
	const char* split = ",";
	//read the first line: days n and instances of problems k.
	getline(input, buf);
	char *s_input = (char *)buf.c_str();
	char *tmp = strtok(s_input, split);
	n = atoi(tmp);
	tmp = strtok(NULL, split);
	k = atoi(tmp);

	//*****Open output file*****//
	string outfile ("lcai63_output_dc_");
	outfile.append(to_string(n));
	outfile.append(".txt");
	ofstream output_dc;
	output_dc.open(outfile);
	if (output_dc.is_open() != 1) {
		cout << "Output File of dc Open Error!" << endl;
		return 1;
	}
	output_dc.setf(ios::fixed, ios::floatfield);
	output_dc.precision(2);

	outfile.assign("lcai63_output_dp_");
	outfile.append(to_string(n));
	outfile.append(".txt");
	ofstream output_dp;
	output_dp.open(outfile);
	if (output_dp.is_open() != 1) {
		cout << "Output File of dp Open Error!" << endl;
		return 1;
	}
	output_dp.setf(ios::fixed, ios::floatfield);
	output_dp.precision(2);


	///Read rates of different days and instances
	///And compute with two algorithms
	vector<vector<double> > rate(k, vector<double>(n));
	int row = 0;
	while (getline(input, buf)) {
		// string->char *
		s_input = (char *)buf.c_str();
		tmp = strtok(s_input, split);
		int col = 0;
		while (tmp != NULL) {
			// char * -> double
			double a = atof(tmp);
			//cout << a << endl;
			rate[row][col] = a;
			tmp = strtok(NULL, split);
			col++;
		}

		// for (int j = 0; j < n; j++) {
		// 	cout << rate[row][j] << " ";
		// }
		// cout << endl;

		//******divide and conquer*****//
		int low, high;

		gettimeofday(&start, NULL);
		double maxSum = max_dc(rate[row], 0, n - 1, &low, &high);
		gettimeofday(&end, NULL);
		time = (double)(end.tv_usec - start.tv_usec) / 1000;

		//output
		output_dc << maxSum << "," << low + 1 << "," << high + 1 << "," ;
		output_dc << setprecision(3) << time << endl;
		cout << setprecision(2) << maxSum << "," << low + 1 << "," << high + 1 << ",";
		cout << setprecision(3) << time << endl;

		//******dynamic programming******//

		gettimeofday(&start, NULL);
		low = -1; high = -1;
		maxSum = max_dp(rate[row], n, &low, &high);
		gettimeofday(&end, NULL);
		time = (double)(end.tv_usec - start.tv_usec) / 1000;

		//output
		output_dp << maxSum << "," << low + 1 << "," << high + 1 << ",";
		output_dp << setprecision(3) << time << endl;
		cout << setprecision(2) << maxSum << "," << low + 1 << "," << high + 1 << "," ;
		cout << setprecision(3) << time << endl << endl;

		row++;
	}

	// for (int i = 0; i < k; i++) {
	// 	for (int j = 0; j < n; j++) {
	// 		cout << rate[i][j] << " ";
	// 	}
	// 	cout << endl;
	// }

	output_dc.close();
	output_dp.close();

	return 0;

}



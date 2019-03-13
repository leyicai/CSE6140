#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <iostream>
#include <vector>

using namespace std;

double max(double a, double b, double c);
double max_Cross(vector<double> vec, int low, int high, int mid, int *l_ind, int *h_ind);
double max_dc(vector<double> vec, int low, int high, int *l_ind, int *h_ind);
double max_dp(vector<double>vec, int n, int *l_ind, int *h_ind);


#endif /* FUNCTION_H_ */

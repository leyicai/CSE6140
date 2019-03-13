#include "function.h"

//Function to return the max value between 3 values
double max(double a, double b, double c) {
	if (a >= b && a >= c) {return a;}
	else if (b >= a && b >= c) {return b;}
	else {return c;}
}

//Return the max value the cross the left and right subarray. 
//Used in divide-and-conquer.
double max_Cross(vector<double> vec, int low, int high, int mid, int *l_ind, int *h_ind) {
	double left_sum = -100, right_sum = -100, sum = 0;
	for (int i = mid; i >= low; i--) {
		sum += vec[i];
		if (sum > left_sum) {
			left_sum = sum;
			*l_ind = i;
		}
	}
	sum = 0;
	for (int i = mid + 1; i <= high; i++) {
		sum += vec[i];
		if (sum > right_sum) {
			right_sum = sum;
			*h_ind = i;
		}
	}
	return (left_sum + right_sum);
}

//Recursive function to find max subarray with divide-and-conquer strategy.
double max_dc(vector<double> vec, int low, int high, int *l_ind, int *h_ind) {
	if (low == high) {
		*l_ind = *h_ind = low;
		return vec[low];
	}
	int mid = (int)((high + low) / 2);
	int l_ind1 = -1, h_ind1 = -1;
	int l_ind2 = -1, h_ind2 = -1;
	int l_ind3 = -1, h_ind3 = -1;
	double low_sum = max_dc(vec, low, mid, &l_ind1, &h_ind1);
	double high_sum = max_dc(vec, mid + 1, high, &l_ind2, &h_ind2);
	double cross_sum = max_Cross(vec, low, high, mid, &l_ind3, &h_ind3);
	double sum = max(low_sum, high_sum, cross_sum);
	// cout << low_sum << "= [" << l_ind1 << ',' << h_ind1 << ']' << endl;
	// cout << high_sum << "= [" << l_ind2 << ',' << h_ind2 << ']' << endl;
	// cout << cross_sum << "= [" << l_ind3 << ',' << h_ind3 << ']' << endl << endl;

	//get the beginning and end indices of max subarray
	if (sum == low_sum) {
		*l_ind = l_ind1; *h_ind = h_ind1;
	}
	if (sum == high_sum) {
		*l_ind = l_ind2; *h_ind = h_ind2;
	}
	if (sum == cross_sum) {
		*l_ind = l_ind3; *h_ind = h_ind3;
	}
	return sum;
}


//Function to find max subarray with dynamic programming
double max_dp(vector<double>vec, int n, int *l_ind, int *h_ind) {
	double maxSum = 0;
	double thisSum = 0;
	*l_ind = 0;
	int tmp = 0;
	for (int i = 0; i < n; i++) {
		thisSum += vec[i];
		if (thisSum < 0) {
			thisSum = 0;
		}
		else if (thisSum > maxSum) {
			maxSum = thisSum;
			*h_ind = i;
		}
	}
	//compute backward to get the low index of max subarray
	thisSum = 0; maxSum = 0;
	for (int i = n - 1; i >= 0; i--) {
		thisSum += vec[i];
		if (thisSum < 0) {
			thisSum = 0;
		}
		else if (thisSum > maxSum) {
			maxSum = thisSum;
			*l_ind = i;
		}

	}
	return maxSum;
}

#include<iostream>
#include"omp.h"
#include<chrono>
#include <locale.h>
#include <Windows.h>

using namespace std;

const int arr_size = 20000;

int arr[arr_size][arr_size];

void init_arr();
long long part_sum(int, int, int);
int part_min_row(int, int, int);
int sizeThread = 4;

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	init_arr();
	omp_set_nested(1);
	double t1 = omp_get_wtime();

#pragma omp parallel sections
	{
#pragma omp section
		{
			cout << "row with minimal sum (thead count = " << 1 << "): " << part_min_row(0, arr_size, 4) << endl;
			cout << "row with minimal sum (thead count = " << 4 << "): " << part_min_row(0, arr_size, 4) << endl;
			cout << "row with minimal sum (thead count = " << 8 << "): " << part_min_row(0, arr_size, 8) << endl;
			cout << "row with minimal sum (thead count = " << 16 << "): " << part_min_row(0, arr_size, 16) << endl;
			cout << "row with minimal sum (thead count = " << 32 << "): " << part_min_row(0, arr_size, 32) << endl;
		}

#pragma omp section
		{
			cout << "sum of all (thead count = " << 1 << "): " << part_sum(0, arr_size, 4) << endl;
			cout << "sum of all (thead count = " << 4 << "): " << part_sum(0, arr_size, 4) << endl;
			cout << "sum of all (thead count = " << 8 << "): " << part_sum(0, arr_size, 8) << endl;
			cout << "sum of all (thead count = " << 16 << "): " << part_sum(0, arr_size, 16) << endl;
			cout << "sum of all (thead count = " << 32 << "): " << part_sum(0, arr_size, 32) << endl;
		}
	}
	double t2 = omp_get_wtime();
	cout << "whole time: " << t2 - t1 << " sec" << endl;
	return 0;
}

void init_arr() {
	for (int i = 0; i < arr_size; i++) {
		for (int j = 0; j < arr_size; j++) {
			arr[i][j] = (arr_size - i - 1) * arr_size + j;
		}
	}
}


long long part_sum(int start_index, int finish_index, int num_threads) {
	long long sum = 0;
	double t1 = omp_get_wtime();
#pragma omp parallel for reduction(+:sum) num_threads(num_threads)
	for (int i = start_index; i < finish_index; i++) {
		for (int j = 0; j < arr_size; j++) {
			sum += arr[i][j];
		}
	}
	double t2 = omp_get_wtime();
	cout << "time for sum (num of threads " << num_threads << " ):" << t2 - t1 << " sec" << endl;
	return sum;
}

int part_min_row(int start_index, int finish_index, int num_threads) {
	int min_row = -1;
	long long minsum = INT32_MAX;
	double t1 = omp_get_wtime();
#pragma omp parallel for num_threads(num_threads)
	for (int i = start_index; i < finish_index; i++) {
		long long sum = 0;
		for (int j = 0; j < arr_size; j++) {
			sum += arr[i][j];
		}
		if(minsum>sum){
			#pragma omp critical
			if (minsum > sum) {
				min_row = i;
				minsum = sum;
			}
		}
	}
	double t2 = omp_get_wtime();
	cout << "time for finding row with minimal thread  = " << num_threads << " : " << t2 - t1 << " sec" << endl;
	return min_row;
}

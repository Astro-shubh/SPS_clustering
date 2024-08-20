#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <cfloat>
#include <bits/stdc++.h>



std::vector<double> X, Y;
int id;

double Distance(int idx1){
	double dist=0.0, distx, disty;
	distx = X[idx1] - X[id];
	disty = Y[idx1] - Y[id];
	dist = sqrt (pow(distx, 2.0) + pow(disty, 2.0));
	return dist;
}

double increament(int idx1){
	return double(idx1+1.0);
}
using namespace std;
int main(){
	int i,j;
	vector<double> distances1, distances2(5000, 0.0);
	vector<int> idx, idx2(5000, 0);
	for(i=0; i<5000; i++){
		X.push_back(i*1.0);
		Y.push_back(i*1.0);
		idx.push_back(i);
	}
	
//	for(i=0; i<5000; i++){
//		id=i;
//		vector<double> distances1(5000, 0);
//		for(j=0; j<5000; j++){
//			distances1[j] = Distance(j);
//		}
//	}
	
	for(i=0; i<5000; i++){
		id=i;
		vector<double> distances2(5000, 0.0);
		transform(idx.begin(), idx.end(), distances2.begin(), Distance);
	}
}

		

		


#include "source/make_cartesian_clusters.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <cfloat>
#include <bits/stdc++.h>
#include <algorithm>

class width_clustering{
	public:
		std::vector<double> Widths, X, Y, SNR;
        	std::vector<make_cartesian_clusters::cluster> small_clusters;
		width_clustering(std::vector<double> X_locations, std::vector<double> Y_locations, std::vector<double> SNR, std::vector<double> Widths, std::vector<make_cartesian_clusters::cluster> input_clusters);
		void do_clustering();
		std::vector<std::vector<int>> final_clusters;
	private:
		void get_representative();
		std::vector<int> list_labels;
		void connect();
		std::vector<int> representative_elements;
}

using namespace std;

width_clustering::width_clustering(std::vector<double> X_locations, std::vector<double> Y_locations, std::vector<double> snr, std::vector<double> widths, std::vector<make_cartesian_clusters::cluster> input_clusters){
	small_clusters = input_clusters;
	Widths = widths;
	X = X_locations;
	Y = Y_locations;
	SNR = snr;
}


void width_clustering::get_representative(){
	int ii, jj, max_idx;
	for(ii = 0; ii<small_clusters.size(); ii++){
		vector<int> SNRs;
		for(jj=0; jj<small_clusters.elements.size(); jj++){
			SNRs.push_back(SNR[clusters.elements[jj]]);
		}
		int idx;
		idx = max_element(SNRs.begin(), SNRs.end()) - SNRs.begin();
		max_idx = clusters.elements[idx];
		representative_elements.push_back(max_idx);
	}
}

void width_clustering::distance(int idx1, int idx2){
	double x = X[idx1] - X[idx2];
	double y = Y[idx1] - Y[idx2];
	return sqrt(x*x + y*y);
}

void width_clustering::connect(){
	int ii, jj, kk;
	vector<int> status(representative_elements.size(), 0);
	for(ii=0; ii<representative_elements.size(); ii++){
		if(status[ii] == 0){
			vector<int> list_elements;
			for(jj=0; jj<small_clusters.elements.size(); jj++){
				list_elements.push_back(small_cluster.elements[jj]);
			}
			double current_W = Widths[representative_elements[ii]];

			for(jj=0; jj<representative_elements.size(); jj++){
				if(status[jj] == 0){
					double d12 = distance(representative_elements[ii], representative_elements[jj]);
					if(d12 < current_W){
						for(kk=0; kk<small_clusters.elements.size(); kk++){
							list_elements.push_back(small_clusters.elements[kk]);
						}
					}
					status[jj] = 1;
				}
			}
			status[ii] = 1;
			final_clusters.push_back(list_elements);
		}
	}
}



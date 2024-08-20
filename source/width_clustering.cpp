#include "width_clustering.h"
#include "make_cartesian_clusters.h"
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
                for(jj=0; jj<small_clusters[ii].elements.size(); jj++){
                        SNRs.push_back(SNR[small_clusters[ii].elements[jj]]);
                }
                int idx;
                idx = max_element(SNRs.begin(), SNRs.end()) - SNRs.begin();
                max_idx = small_clusters[ii].elements[idx];
                representative_elements.push_back(max_idx);
        }
}

double width_clustering::distance(int idx1, int idx2){
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
                        for(jj=0; jj<small_clusters[ii].elements.size(); jj++){
                                list_elements.push_back(small_clusters[ii].elements[jj]);
                        }
                        double current_W = Widths[representative_elements[ii]];
                        for(jj=0; jj<representative_elements.size(); jj++){
                                if(status[jj] == 0){
                                        double d12 = distance(representative_elements[ii], representative_elements[jj]);
                                        if(d12 < current_W){
                                                for(kk=0; kk<small_clusters[jj].elements.size(); kk++){
                                                        list_elements.push_back(small_clusters[jj].elements[kk]);
                                                }
                                        status[jj] = 1;
					}
                                }
                        }
                        status[ii] = 1;
                        final_clusters.push_back(list_elements);
                }
        }
}

void width_clustering::connect_recursive(int idx){
// Get the neighbours for idx first,

	int ii, jj;
	connections.push_back(idx);
	double current_W =  Widths[representative_elements[idx]];
	cout << "current width is " << current_W << endl;
	vector<int> neighbours;                             // neighbours stores the indices of neighbouring clusters in current small_clusters array
	for(jj=0; jj<representative_elements.size(); jj++){
		if(status[jj] == 0){
			double d12 = distance(representative_elements[idx], representative_elements[jj]);
			if(d12 < current_W){
				neighbours.push_back(jj);
				status[jj] = 1;           // Status of the connected neighbours is updated
			}
		}
	}

// Got the neighbours for idx
// Calling the recursive functions for these neighbours
	cout << neighbours.size() << " neighbours found " << endl;
	if(neighbours.size() > 0){
		for(ii=0; ii<neighbours.size(); ii++){
			connect_recursive(neighbours[ii]);
		}
	}
}

void width_clustering::do_clustering(){
	int ii, jj, kk;
	cout << "starting while loop" << endl;
	cout << " current size of cluster array: " << small_clusters.size() << endl;
	get_representative();
	while(representative_elements.size() > 0){
		cout << "starting while loop" << endl;
		cout << " current size of cluster array: " << small_clusters.size() << endl;
		vector<int> cluster_elements;
		status.clear();
		for(ii=0; ii<representative_elements.size(); ii++){status.push_back(0);}
		status[0] = 1;
		connections.clear();                        // connections store the indices of connected elements 
		cout << "calling recursive function" << endl;
		connect_recursive(0);
		cout << "connected " << connections.size() << " clusters " << endl;
		for(ii=0; ii<connections.size(); ii++){
			for(jj=0; jj<small_clusters[connections[ii]].elements.size(); jj++){
				cluster_elements.push_back(small_clusters[connections[ii]].elements[jj]);
			}
		}
		vector<int> rps = representative_elements;
		vector<make_cartesian_clusters::cluster> cls = small_clusters;
		representative_elements.clear();
		small_clusters.clear();
		final_clusters.push_back(cluster_elements);
//		for(ii=0; ii<connections.size(); ii++){
//			representative_elements.erase(representative_elements.begin()+connections[ii]);
//			small_clusters.erase(small_clusters.begin()+connections[ii]);
//		}
		for(ii=0; ii<status.size(); ii++){
			if(status[ii] == 0){
				representative_elements.push_back(rps[ii]);
				small_clusters.push_back(cls[ii]);
			}
		}
		cout << " current size of cluster array: " << small_clusters.size() << endl;
	}



		
}

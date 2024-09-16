#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <chrono>
#include "source/read_spccl.h"
#include "source/prepare_data.h"
#include "source/Spanning_tree_cartesian.h"
#include "source/make_cartesian_clusters.h"
#include "source/width_clustering.h"

using namespace std;
int main(){
	auto now = chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	auto milliseconds = chrono::duration_cast<chrono::milliseconds>(duration).count();
	cout << "Time:" << milliseconds << endl;
        string filename="tmpukqf8saz_2012_03_14_00_00_00.spccl";
	int i, j, k, size;
        long double d, d1;
	vector<float> DM_axis, Time_axis, branch_start, branch_end, weights;
	vector<float> X_axis, Y_axis;
	read_spccl spccl(filename);
        spccl.read();
	spccl.prepare();
	DM_axis=spccl.DM_Time;
	Time_axis=spccl.ToA;
	size = DM_axis.size();
	ofstream outfile1("DM_time_axes.txt");
        for(i=0; i<DM_axis.size(); i++){
                outfile1 << to_string(Time_axis[i])+"  "+to_string(DM_axis[i]) << endl;
        }
	outfile1.close();

	int block_size =  5000;
	vector<vector<float>> X_blocks, Y_blocks;
	vector<float> X_vec, Y_vec;
	vector<make_cartesian_clusters::cluster> all_clusters;
	if(size > block_size){
		for(i=1; i<size+1; i++){
			X_vec.push_back(Time_axis[i-1]);
			Y_vec.push_back(DM_axis[i-1]);
			if(i%block_size == 0){
				X_blocks.push_back(X_vec);
				Y_blocks.push_back(Y_vec);
				X_vec.clear();
				Y_vec.clear();
			}
			if(i == size){
				X_blocks.push_back(X_vec);
				Y_blocks.push_back(Y_vec);
			}
		}

		for(i = 0; i<X_blocks.size(); ++i){
		
	
			Spanning_tree_cartesian Tree(X_blocks[i], Y_blocks[i]);
			Tree.construct_tree();

			make_cartesian_clusters clustering(Tree.Parent, Tree.Weights, X_blocks[i].size(), 10);
			clustering.make_clusters();

			for(j=0; j<clustering.good_clusters.size(); ++j){
				for(k=0; k<clustering.good_clusters[j].elements.size(); ++k){
					clustering.good_clusters[j].elements[k] = clustering.good_clusters[j].elements[k] + i*block_size;
				}
				all_clusters.push_back(clustering.good_clusters[j]);
			}
		}
	}
	else{
		Spanning_tree_cartesian Tree(Time_axis, DM_axis);
		Tree.construct_tree();
		make_cartesian_clusters clustering(Tree.Parent, Tree.Weights, Time_axis.size(), 10);
		clustering.make_clusters();

		all_clusters = clustering.good_clusters;
	}
	width_clustering width_clstr(Time_axis, DM_axis, spccl.SigmaD, spccl.WidthD, all_clusters);
	width_clstr.do_clustering();

		

	ofstream labels1("cluster_labels1.txt");
        for(i=0; i<width_clstr.final_clusters.size(); i++){
              for(j=0; j<width_clstr.final_clusters[i].size(); j++){
                        labels1 << " " << to_string(width_clstr.final_clusters[i][j]);
                }
                labels1 << endl;
		cout << "wrote " << i << " clusters " << endl;
        }
	cout << "closing the file" << endl;
	labels1.close();
	cout << "data points: " << X_axis.size() << endl;
}	


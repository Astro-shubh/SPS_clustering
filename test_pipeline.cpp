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
        string filename="2012_03_14_00_00_00_wide.spccl";
	int i, j;
        long double d, d1;
	vector<float> DM_axis, Time_axis, branch_start, branch_end, weights;
	vector<float> X_axis, Y_axis;
	read_spccl spccl(filename);
        spccl.read();
	spccl.prepare();
	DM_axis=spccl.DM_Time;
	Time_axis=spccl.ToA;
	for(i=0;i< Time_axis.size();i++){
//		if(Time_axis[i] > 0000){
//			if(Time_axis[i] < 14300){
				X_axis.push_back(Time_axis[i]);
				Y_axis.push_back(DM_axis[i]);
//			}
//		}
	}
	now = chrono::system_clock::now();
	duration = now.time_since_epoch();
	milliseconds = chrono::duration_cast<chrono::milliseconds>(duration).count();
	cout << "Done preparing data" << endl;
	cout << "Time: " << milliseconds << endl;

	Spanning_tree_cartesian Tree(X_axis, Y_axis);
	
	Tree.construct_tree();
        now = chrono::system_clock::now();
        duration = now.time_since_epoch();
        milliseconds = chrono::duration_cast<chrono::milliseconds>(duration).count();
        cout << "Done computing the tree" << endl;
        cout << "Time: " << milliseconds << endl;

	vector<int> parent, child;
	ofstream outfile("Spanning_tree.txt");
	for(i=0; i<Tree.Parent.size(); i++){
		outfile << to_string(i)+"  "+to_string(Tree.Parent[i])+"  "+to_string(Tree.Weights[i]) << endl;
		child.push_back(i);
		parent.push_back(Tree.Parent[i]);
	}
	outfile.close();
	ofstream outfile1("DM_time_axes.txt");
	for(i=0; i<Y_axis.size(); i++){
		outfile1 << to_string(X_axis[i])+"  "+to_string(Y_axis[i]) << endl;
	}
	outfile1.close();
        now = chrono::system_clock::now();
        duration = now.time_since_epoch();
        milliseconds = chrono::duration_cast<chrono::milliseconds>(duration).count();
        cout << "Done constructing MST" << endl;
        cout << "Time: " << milliseconds << endl;
	make_cartesian_clusters clustering(Tree.Parent, Tree.Weights, X_axis.size(), 10);
	clustering.make_clusters();
//	for(i=0; i<clustering.clusters_stable.size(); i++){
//		for(j=0; j<clustering.clusters_stable[i].size(); j++){
//			cout << clustering.clusters_stable[i][j].child.size() << "<-" << clustering.clusters_stable[i][j].label << "->" << clustering.clusters_stable[i][j].stability << " ";
//		}
//		cout << endl;
//	}
	cout << "done making clusters" << endl;
        ofstream labels("cluster_labels.txt");
        for(i=0; i<clustering.full_labels.size(); i++){
              for(j=0; j<clustering.full_labels[i].size(); j++){
                        labels << " " << to_string(clustering.full_labels[i][j]);
                }
                labels << endl;
        }

        labels.close();
        now = chrono::system_clock::now();
        duration = now.time_since_epoch();
        milliseconds = chrono::duration_cast<chrono::milliseconds>(duration).count();
        cout << "Done making clusters" << endl;
        cout << "Time: " << milliseconds << endl;
	width_clustering width_clstr(X_axis, Y_axis, spccl.SigmaD, spccl.WidthD, clustering.good_clusters);

	width_clstr.do_clustering();
	cout << "done making clusters" << endl;
	cout << width_clstr.final_clusters.size() << endl;
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


#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <chrono>
#include "source/KD_tree.h"
#include "source/read_spccl.h"
#include "source/prepare_data.h"
#include "source/Spanning_tree_MRD.h"
#include "source/make_optimised_clusters.h"

using namespace std;
int main(){
	auto now = chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	auto milliseconds = chrono::duration_cast<chrono::milliseconds>(duration).count();
	cout << "Time:" << milliseconds << endl;
        string filename="2012_03_14_00_00_00.spccl";
	int i, j;
        long double d, d1;
	vector<double> DM_axis, Time_axis, branch_start, branch_end, weights;
	vector<double> X_axis, Y_axis;
	read_spccl spccl(filename);
        spccl.read();
	spccl.prepare();
	DM_axis=spccl.DM_Time;
	Time_axis=spccl.ToA;
	for(i=0;i<2000;i++){
		X_axis.push_back(Time_axis[i]);
		Y_axis.push_back(DM_axis[i]);
	}
	now = chrono::system_clock::now();
	duration = now.time_since_epoch();
	milliseconds = chrono::duration_cast<chrono::milliseconds>(duration).count();
	cout << "Done preparing data" << endl;
	cout << "Time: " << milliseconds << endl;

	Spanning_tree_MRD Tree(X_axis, Y_axis);

	Tree.Compute_coredist();
        now = chrono::system_clock::now();
        duration = now.time_since_epoch();
        milliseconds = chrono::duration_cast<chrono::milliseconds>(duration).count();
        cout << "Done computing core distance" << endl;
        cout << "Time: " << milliseconds << endl;

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
	make_optimised_clusters clustering(parent, child, Tree.Weights, Tree.Core_distance);
	cout << "calling makecluster" << endl;
	clustering.make_clusters();
//	cout << clustering.tree_labels[10].size() << endl;
//	cout << clustering.tree_connections[8].size() << endl;
//	cout << clustering.tree_sizes[10].size() << endl;
//	cout << clustering.full_labels[10].size() << endl;
	ofstream labels("cluster_labels.txt");
	for(i=0; i<10; i++){
		for(j=0; j<clustering.full_labels[i*20].size(); j++){
			labels << " " << to_string(clustering.full_labels[i*50][j]);
		}
		labels << endl;
	}
	labels.close();
        now = chrono::system_clock::now();
        duration = now.time_since_epoch();
        milliseconds = chrono::duration_cast<chrono::milliseconds>(duration).count();
        cout << "Done making clusters" << endl;
        cout << "Time: " << milliseconds << endl;

}


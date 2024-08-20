#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include "source/KD_tree.h"

using namespace std;
int main(){
        int i,j, core =10;
        vector<double> X, Y;
        vector<int> data_idx;
        struct tree_node *root;
        struct tree_node *root1;
        for(i=0; i<2000; i++){
                X.push_back(i*1.000);
                Y.push_back(i*1.000);
                data_idx.push_back(i);
        }

	KD_tree Tree(data_idx, X, Y, core);
	Tree.construct_tree();
	Tree.compute_coreD();
	for(i=0; i<Tree.coreD.size(); i++){
		cout << Tree.coreD[i] << endl;
	}
}


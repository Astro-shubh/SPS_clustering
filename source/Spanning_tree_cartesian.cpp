#include "Spanning_tree_cartesian.h"

using namespace std;

Spanning_tree_cartesian::Spanning_tree_cartesian(vector<double> X, vector<double> Y){
	X_locations=X;
	Y_locations=Y;
	size=X.size();
}

double Spanning_tree_cartesian::Cartesian(int idx1, int idx2){
	x_dist=X_locations[idx1]-X_locations[idx2];
	y_dist=Y_locations[idx1]-Y_locations[idx2];
	distance = sqrt(x_dist*x_dist + y_dist*y_dist);
	return (distance);
}


int Spanning_tree_cartesian::key_for_treenodes(vector<double> key, vector<bool> MST_status){
	double min_key = DBL_MAX;
	int min_index, v;
	for(v=0; v < size; v++){
		if(key[v] < min_key && MST_status[v] == false){
			min_key = key[v];
			min_index = v;
		}
	}
	return min_index;
}	




void Spanning_tree_cartesian::construct_tree(){
	vector<double> key;
	vector<bool> status;
	vector<int> parent(size, 0.0);
	vector<double> weights(size, 0.0);
	double max=DBL_MAX;
	int i,j;


	for(i=0; i<size; i++){
		key.push_back(max);

		status.push_back(false);
	}


	key[0] = 0.0;


	parent[0] = -1;
	for(j=0; j < size-1; j++){
		int u = key_for_treenodes(key, status), v, k;
		status[u] = true;
//		vector<double> distances;
//		#pragma omp parallel for num_threads(4)
//		for(k=0; k<size; k++){
//			distances.push_back(Cartesian(u, k));
//		}
		for(v=0; v<size; v++){
//			double mrd = distances[v];
			double mrd = Cartesian(u, v);
			if( status[v] == false && mrd < key[v]){
				parent[v]=u;
				key[v] = mrd;
				weights[v] = mrd;
			}
		}
	}
	Parent=parent;
	Weights=weights;
}

	 

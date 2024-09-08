#include "Spanning_tree_cartesian.h"

using namespace std;

Spanning_tree_cartesian::Spanning_tree_cartesian(vector<float> X, vector<float> Y){
	X_locations=X;
	Y_locations=Y;
	size=X.size();
}

float Spanning_tree_cartesian::Cartesian(int idx1, int idx2){
	x_dist=X_locations[idx1]-X_locations[idx2];
	y_dist=Y_locations[idx1]-Y_locations[idx2];
	distance = sqrt(x_dist*x_dist + y_dist*y_dist);
	return (distance);
}


int Spanning_tree_cartesian::key_for_treenodes(const vector<float>& key, const vector<bool>& MST_status){
	float min_key = FLT_MAX;
	int min_index, v;
	for(v=0; v < size; ++v){
		if(MST_status[v] == false && key[v] < min_key){
			min_key = key[v];
			min_index = v;
		}
	}
	return min_index;
}	




void Spanning_tree_cartesian::construct_tree(){
	vector<float> key(size, FLT_MAX);
	vector<bool> status(size, false);
	vector<int> parent(size, 0);
	vector<float> weights(size, 0.0);
	int i,j;
	
	key[0] = 0.0;
	parent[0] = -1;
        for(j=0; j < size-1; ++j){
                int u = key_for_treenodes(key, status), v, k;
                status[u] = true;
		for(v=0; v<size; ++v){
                        if( status[v] == false){
                                float mrd = Cartesian(u, v);
                                if(mrd < key[v]){
                                        parent[v]=u;
                                        key[v] = mrd;
                                      weights[v] = mrd;
                                }
                        }
                }
        }

	Parent=parent;
	Weights=key;
}

	 

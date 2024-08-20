#include "Spanning_tree_MRD.h"
#include "KD_tree.h"

using namespace std;

Spanning_tree_MRD::Spanning_tree_MRD(vector<double> X, vector<double> Y){
	X_locations=X;
	Y_locations=Y;
	size=X.size();
}

double Spanning_tree_MRD::Cartesian(int idx1, int idx2){
	x_dist=X_locations[idx1]-X_locations[idx2];
	y_dist=Y_locations[idx1]-Y_locations[idx2];
	distance = sqrt(x_dist*x_dist + y_dist*y_dist);
	return (distance);
}

double Spanning_tree_MRD::get_coreD(vector<double> Dist){
	vector<double> sorted_array;
	int i,j;
	for(i=0; i<core+1; i++){
		for(j=0; j<Dist.size(); j++){
			if(Dist[j]<Dist[j+1]){
				swap(Dist[j],Dist[j+1]);
			}
		}
	}
	return(Dist[Dist.size()-core]);
}

void Spanning_tree_MRD::Compute_coredist(){
	int i,j,k;
	vector<double> distances;
//	#pragma omp parallel for num_threads(4)
	for(i=0; i<size; i++){
//		#pragma omp parallel for num_threads(16)
		for(j=0; j<size; j++){
			if(i==j){distances.push_back(0.0);}
			else{distances.push_back(Cartesian(i,j));}
		}
		Core_distance.push_back(get_coreD(distances));
		distances.clear();
	}
}

void Spanning_tree_MRD::Compute_coredist1(){
	int i;
	vector<int> data_id;
	for(i=0; i<size; i++){
		data_id.push_back(i);
	}
	KD_tree KD(data_id, X_locations, Y_locations, core);
	KD.construct_tree();
	KD.compute_coreD();
	Core_distance = KD.coreD;
}


double Spanning_tree_MRD::MRD(int point1, int point2){
	double mrd, cd1, cd2, d12;
	cd1=Core_distance[point1];
	cd2=Core_distance[point2];
	d12=Cartesian(point1, point2);
	if(cd1 > cd2){
		if(cd1 > d12){
			mrd=cd1;
		}
		else{
			mrd=d12;
		}
	}
	else{
		if(cd2 > d12){
			mrd=cd2;
		}
		else{
			mrd=d12;
		}
	}
	return(mrd);
}

int Spanning_tree_MRD::key_for_treenodes(vector<double> key, vector<bool> MST_status){
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




void Spanning_tree_MRD::construct_tree(){
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
		int u = key_for_treenodes(key, status), v;
		status[u] = true;

//		#pragma omp parallel for num_threads(16)
		for(v=0; v<size; v++){
			double mrd = MRD(u, v);
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

	 

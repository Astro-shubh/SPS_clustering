#include <vector>
#include <iostream>
#include <chrono>
#include <bits/stdc++.h>
#include <math.h>

struct tree_node{
	tree_node *left;
	tree_node *right;
	int data_index;
	int label=1;
};

using namespace std;


struct tree_node* make_node(vector<int> data, vector<double> X_locations, vector<double> Y_locations, int depth){
	struct tree_node* parent=new tree_node;
	vector<double> locations;
	vector<int> left_data, right_data;
	int ii, jj, kk, median;
	if(data.size() > 1){
		if(depth % 2 == 0){locations = X_locations;}
		else{locations = Y_locations;}
		for(ii=0; ii<data.size(); ii++){
			for(jj=0; jj < data.size()-1; jj++){
				if(locations[data[jj]] > locations[data[jj+1]]){swap(data[jj], data[jj+1]);}
			}
		}
		if(data.size() % 2 == 0){kk = int(data.size()/2.0)-1;}
		else{kk = int(data.size()/2.0);}
		median = data[kk];
		for(ii=0; ii<data.size(); ii++){
			if(ii < kk){left_data.push_back(data[ii]);}
			if(ii > kk){right_data.push_back(data[ii]);}
		}
		parent->data_index = median;
		if(left_data.size() > 0){
		parent->left = make_node(left_data, X_locations, Y_locations, depth +1);
		}
		else{parent->left = NULL;}
		if(right_data.size() > 0){
		parent->right = make_node(right_data, X_locations, Y_locations, depth +1);
		}
		else{parent->right = NULL;}
	}
	if(data.size() == 1){
		median = data[0];
		parent->data_index = median;
		parent->left = NULL;
		parent->right = NULL;
	}
	return parent;

}

void print_tree(tree_node *node){
	if(node!=NULL){
		print_tree(node->left);
		cout << " " << node->data_index << "  ";
		print_tree(node->right);
	}
}

vector<int> return_elements(tree_node *node, vector<int> elements){
        if(node!=NULL){
                elements=return_elements(node->left, elements);
                elements.push_back(node->data_index);
                elements=return_elements(node->right, elements);
        }
	return elements;
}

double dist(int idx1, int idx2, vector<double> X, vector<double> Y){
	double xdist, ydist, distance;
	xdist = X[idx1] - X[idx2];
	ydist = Y[idx1] - Y[idx2];
	distance = sqrt(xdist*xdist + ydist*ydist);
	return distance;
}


tree_node* find_parent(tree_node* root, int index, vector<double> X_locations, vector<double> Y_locations){
	int depth=0, ii, jj;
	double seperator, seperation, location;

	tree_node *gparent, *parent;
	parent = root;
	gparent = root;
	while(parent->data_index != index){
		if(depth % 2 == 0){seperator = X_locations[parent->data_index]; location = X_locations[index];}
		else{seperator = Y_locations[parent->data_index]; location = Y_locations[index];}
		gparent = parent;
		if(location < seperator){parent = parent->left;}
		if(location > seperator){parent = parent->right;}
		depth = depth +1;
//		cout << "searching depth: " << depth << endl;
//		cout << "current index: " << parent->data_index << endl;
	}
	return gparent;	
}

vector<int> get_branch(tree_node *node, int index, vector<double> X, vector<double> Y){
//	cout << "get branch" << endl;
        vector<int> elements;
        tree_node *parent;

        while(elements.size() < 20){
                elements.clear();
                parent = find_parent(node, index, X, Y);
                elements = return_elements(parent, elements);
                index = parent->data_index;
//		cout << index << endl;
        }
        return elements;
}

vector<double> query_distances(tree_node *node, vector<double> distances, int index, int depth, vector<double> X, vector<double> Y){
	vector<int> elements;
	int ii;
	elements = return_elements(node, elements);
	cout << "element size " << elements.size() << endl;
	if(elements.size() > 10){
		if(node->data_index == index){
			int i;
			for(i=0; i<elements.size(); ++i){
				distances.push_back(dist(index, elements[i], X, Y));
			}

		}
	
		else{
			double seperator, seperation, location;
			tree_node *left, *right;
			int i, j;
			left = node->left;
			right = node->right;
			if(depth % 2 == 0){
				cout << "in X" << endl;
				seperator = X[node->data_index];
				location = X[index];
				cout << "seperator " << seperator << endl;
				if(location > seperator){
					seperation = location - seperator;
					if(dist(index, right->data_index, X, Y) > seperation){
						cout << "calling left and right" << endl;
						distances = query_distances(right, distances, index, depth+1, X, Y);
						distances = query_distances(left, distances, index, depth+1, X, Y);
					}
					else{
						cout << "calling right" << endl;
						distances = query_distances(right, distances, index, depth+1, X, Y);
					}
				}
				else{
					seperation = seperator - location;
					cout << "distance " << dist(index, left->data_index, X, Y) << endl;
					if(dist(index, left->data_index, X, Y) > seperation){
						cout << "calling left and right" << endl;
                                		distances = query_distances(right, distances, index, depth+1, X, Y);
                                		distances = query_distances(left, distances, index, depth+1, X, Y);
                        		}
                        		else{
						cout << "calling left" << endl;
                                		distances = query_distances(left, distances, index, depth+1, X, Y);
                        		}
				}
			}
			else{
				cout << "in Y" << endl;
				seperator = Y[node->data_index];
                		location = Y[index];

                		if(location > seperator){
                        		seperation = location - seperator;
                        		if(dist(index, right->data_index, X, Y) > seperation){
						cout << "calling left and right" << endl;
                                		distances = query_distances(right, distances, index, depth+1, X, Y);
                                		distances = query_distances(left, distances, index, depth+1, X, Y);
                        		}
                        		else{
						cout << "calling right" << endl;
                                		distances = query_distances(right, distances, index, depth+1, X, Y);
                        		}
                		}
                		else{
                        		seperation = seperator - location;
                        		if(dist(index, left->data_index, X, Y) > seperation){
						cout << "calling left and right" << endl;
                                		distances = query_distances(right, distances, index, depth+1, X, Y);
                                		distances = query_distances(left, distances, index, depth+1, X, Y);
                        		}
                        		else{
						cout << "calling left" << endl;
                                		distances = query_distances(left, distances, index, depth+1, X, Y);
                        		}
                		}
        		}
			distances.push_back(dist(index, node->data_index, X, Y));
			cout << dist(index, node->data_index, X, Y) << endl;
		}
	}
	else{
		for(ii=0; ii<elements.size(); ii++){
			distances.push_back(dist(index, elements[ii], X, Y));
		}
	}

	cout << "returning" << endl;
	return distances;
}




double find_coreD1(vector<double> distance, int core){
	double dist;
	int ii, jj, kk;
	for(ii=0; ii<core+1; ii++){
                for(jj=0; jj<distance.size(); jj++){
                        if(distance[jj]<distance[jj+1]){
                                swap(distance[jj],distance[jj+1]);
                        }
                }
        }
	dist = distance[distance.size()-core];
	return(distance[distance.size()-core]);
}

	


double find_coreD(vector<int> elements, int index, vector<double> X_locations, vector<double> Y_locations){
	vector<double> distances;
	int ii, jj, kk, core=10;
	double dist, xdist, ydist;
	for(ii=0; ii<elements.size(); ii++){
		xdist = X_locations[elements[ii]] - X_locations[index];
		ydist = Y_locations[elements[ii]] - Y_locations[index];
		dist = sqrt(xdist*xdist + ydist*ydist);
		distances.push_back(dist);
	}
	for(ii=0; ii<distances.size(); ii++){
                for(jj=0; jj<distances.size(); jj++){
                        if(distances[jj]<distances[jj+1]){
                                swap(distances[jj],distances[jj+1]);
                        }
                }
        }

	return(distances[distances.size()-core]);

}


int main(){
	int i,j;
	vector<double> X, Y;
	vector<int> data_idx;
	struct tree_node *root;
	struct tree_node *root1;
	for(i=0; i<100; i++){
		X.push_back(i*1.000);
		Y.push_back(i*1.000);
		data_idx.push_back(i);
	}
        auto now = chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto milliseconds = chrono::duration_cast<chrono::milliseconds>(duration).count();

        now = chrono::system_clock::now();
        duration = now.time_since_epoch();
        milliseconds = chrono::duration_cast<chrono::milliseconds>(duration).count();
        cout << "making the tree" << endl;
        cout << "Time: " << milliseconds << endl;

	root=make_node(data_idx, X, Y, 0);

        now = chrono::system_clock::now();
        duration = now.time_since_epoch();
        milliseconds = chrono::duration_cast<chrono::milliseconds>(duration).count();
        cout << "Done making the tree" << endl;
        cout << "Time: " << milliseconds << endl;
	vector<double> distances;
	distances = query_distances(root, distances, data_idx[1], 0, X, Y);
	distances = query_distances(root, distances, data_idx[2], 0, X, Y);
//	cout << "distances size" << distances.size() << endl;
//	for(i=0; i<distances.size(); i++){cout << distances[i] << endl;}

	vector<double> coreD;
//	for(i=0; i<data_idx.size(); i++){
//		root1=find_parent(root, data_idx[i], X, Y);
//		vector<int> elements;
//		elements = get_branch(root, data_idx[i], X, Y);
//		for(j=0; j<elements.size(); j++){cout << elements[j] << endl;}
//		coreD.push_back(find_coreD(elements, data_idx[i], X, Y));
//	}

//	for(i=0; i<coreD.size(); i++){cout << coreD[i] << endl;}  // Wrong values of core distances

	for(i=0; i<data_idx.size(); i++){
		vector<double> distances1;
		double dist;
		cout << "calling distance query" << endl;
		distances1 = query_distances(root, distances1, data_idx[i], 0, X, Y);
		cout << "distance size " << distances1.size() << endl;
//		dist = find_coreD1(distances1, 10);
//		cout << "core D" << dist << endl;
//		coreD.push_back(dist);
	}
	for(i=0; i<coreD.size(); i++){cout << coreD[i] << endl;}

	
}

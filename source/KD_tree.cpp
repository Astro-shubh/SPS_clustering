#include "KD_tree.h"
using namespace std;

KD_tree::KD_tree(vector<int> data_id, vector<double> X_locations, vector<double> Y_locations, int core_size){
	X=X_locations;
	Y=Y_locations;
	data_idx = data_id;
	data_len = data_idx.size();
	core = core_size;
}

struct KD_tree::tree_node* KD_tree::make_node(vector<int> data, int depth){
	struct tree_node* parent=new tree_node;
        vector<double> locations;
        vector<int> left_data, right_data;
        int ii, jj, kk, median;
        if(data.size() > 1){
                if(depth % 2 == 0){locations = X;}
                else{locations = Y;}
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
                parent->left = make_node(left_data, depth +1);
                }
                else{parent->left = NULL;}
                if(right_data.size() > 0){
                parent->right = make_node(right_data, depth +1);
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

void KD_tree::print_tree(tree_node *node){
        if(node!=NULL){
                print_tree(node->left);
                cout << " " << node->data_index << "  ";
                print_tree(node->right);
        }
}

vector<int> KD_tree::return_elements(tree_node *node, vector<int> elements){
        if(node!=NULL){
                elements=return_elements(node->left, elements);
                elements.push_back(node->data_index);
                elements=return_elements(node->right, elements);
        }
        return elements;
}

double KD_tree::dist(int idx1, int idx2){
        double xdist, ydist, distance;
        xdist = X[idx1] - X[idx2];
        ydist = Y[idx1] - Y[idx2];
        distance = sqrt(xdist*xdist + ydist*ydist);
        return distance;
}

KD_tree::tree_node* KD_tree::find_parent(tree_node* root, int index){
        int depth=0, ii, jj;
        double seperator, seperation, location;

        tree_node *gparent, *parent;
        parent = root;
        gparent = root;
        while(parent->data_index != index){
                if(depth % 2 == 0){seperator = X[parent->data_index]; location = X[index];}
                else{seperator = Y[parent->data_index]; location = Y[index];}
                gparent = parent;
                if(location < seperator){parent = parent->left;}
                if(location > seperator){parent = parent->right;}
                depth = depth +1;
        }
        return gparent;
}

vector<int> KD_tree::get_branch(tree_node *node, int index){
        vector<int> elements;
        tree_node *parent;

        while(elements.size() < 2*core){
                elements.clear();
                parent = find_parent(node, index);
                elements = return_elements(parent, elements);
                index = parent->data_index;
        }
        return elements;
}

vector<int> KD_tree::query_distances(tree_node *node, vector<int> distances, int index, int depth){
        size = data_len/pow(2,depth);
        if(size > 2*core){
                if(node->data_index == index){
                        vector<int> elements;
                        elements = return_elements(node, elements);
                        int i;
                        for(i=0; i<elements.size(); ++i){
                                distances.push_back(elements[i]);
                        }

                }

                else{
                        double seperator, seperation, location;
                        tree_node *left, *right;
                        int i, j;
                        left = node->left;
                        right = node->right;
                        if(depth % 2 == 0){
                                seperator = X[node->data_index];
                                location = X[index];
                                if(location > seperator){
                                        seperation = location - seperator;
                                        if(abs(X[index] - X[right->data_index]) > seperation){
                                                distances = query_distances(right, distances, index, depth+1);
                                                distances = query_distances(left, distances, index, depth+1);
                                        }
                                        else{
                                                distances = query_distances(right, distances, index, depth+1);
                                        }
                                }
                                else{
                                        seperation = seperator - location;
                                        if(abs(X[index] - X[left->data_index]) > seperation){
                                                distances = query_distances(right, distances, index, depth+1);
                                                distances = query_distances(left, distances, index, depth+1);
                                        }
                                        else{
                                                distances = query_distances(left, distances, index, depth+1);
                                        }
                                }
                        }
                        else{
                                seperator = Y[node->data_index];
                                location = Y[index];

                                if(location > seperator){
                                        seperation = location - seperator;
                                        if(abs(Y[index]-Y[right->data_index]) > seperation){
                                                distances = query_distances(right, distances, index, depth+1);
                                                distances = query_distances(left, distances, index, depth+1);
                                        }
                                        else{
                                                distances = query_distances(right, distances, index, depth+1);
                                        }
                                }
                                else{
                                        seperation = seperator - location;
                                        if(abs(Y[index]-Y[left->data_index]) > seperation){
                                                distances = query_distances(right, distances, index, depth+1);
                                                distances = query_distances(left, distances, index, depth+1);
                                        }
                                        else{
                                                distances = query_distances(left, distances, index, depth+1);
                                        }
                                }
                        }
                }
        }
        else{
                int ii;
                vector<int> elements;
                elements = return_elements(node, elements);
                for(ii=0; ii<elements.size(); ii++){
                        distances.push_back(elements[ii]);
                }
        }

        return distances;
}

double KD_tree::find_coreD(vector<int> elements, int index){
        vector<double> distances;
        int ii, jj;
        for(ii=0; ii<elements.size(); ii++){
                distances.push_back(dist(elements[ii], index));
        }
        for(ii=0; ii<core+1; ii++){
                for(jj=0; jj<distances.size(); jj++){
                        if(distances[jj]<distances[jj+1]){
                                swap(distances[jj],distances[jj+1]);
                        }
                }
        }

        return(distances[distances.size()-core]);

}

void KD_tree::construct_tree(){
	root = make_node(data_idx, 0);
}

void KD_tree::compute_coreD(){
	int ii, jj;
	double dist1;
	for(ii=0; ii<data_len; ++ii){
		vector<int> neighbors;
		neighbors = query_distances(root, neighbors, data_idx[ii], 0);
		dist1 = find_coreD(neighbors, data_idx[ii]);
		coreD.push_back(dist1);
	}
}





#include "Spanning_tree_cartesian.h"

// This class takes two co-ordinates of the data points and computes the Minimum spanning tree
using namespace std;

Spanning_tree_cartesian::Spanning_tree_cartesian(vector<float> X, vector<float> Y){
	// Constructor
	X_locations=X;
	Y_locations=Y;
	size=X.size();
	Parent.resize(size);   // resizing parent vector
	Parent[0] = -1;        // First element of the tree is the tree node and does not have a parent
	Weights.resize(size);  // Weights will store the weigth of a link, i.e. distance between the two linked points
}

void Spanning_tree_cartesian::Cartesian(int idx1, int idx2){
	// Utility function to compute distance between two given points
	x_dist=X_locations[idx1]-X_locations[idx2];
	y_dist=Y_locations[idx1]-Y_locations[idx2];
	distance = sqrt(x_dist*x_dist + y_dist*y_dist);
}




void Spanning_tree_cartesian::construct_tree(){
	vector<float> key(size, FLT_MAX);              // Initialised key. Key stores the distances of all the points from current tree
	vector<int> indices(size);                   // A array of indices ranging from 0 to size-1. We need this array to keep track of indices because we plan to remove elements from the key
	iota(indices.begin(), indices.end(),0);     // Fill indices using iota funciton
	key[0] = 0.0;                               // The first element will be part of the tree with no parent
        while(indices.size() > 1){                  //lopping over untill all elements are connected
                int u, v, index;
		auto min_index = min_element(key.begin(), key.end()) - key.begin();     // Get the index of smallest element in the key
                u = min_index;
		index = indices[u];                                             // Real index of the element that has minimum value of key (i.e. this element is nearest to the current tree)
		indices.erase(indices.begin()+u);                               // Remove the element
		key.erase(key.begin()+u);                                       // Remove the key
		for(v=0; v<indices.size(); ++v){                                // Loop over remaining elements that are not part of the tree
			Cartesian(index, indices[v]);                           // Compute the distance from the nearest element to the tree
                        if(distance < key[v]){                                  // If the distance between the new tree element and the index of v is less than the earlier distance of v from the tree
				Parent[indices[v]]=index;                       // Update the parent of each remaining element 
                                key[v] = distance;                              // Update the key of v because the tree has a new element and distance to the tree from the v is now less than before
                                Weights[indices[v]] = distance;                 // Update the Weights
                        }
                }                                   // After completing these two loops, all elements will have correct parents and weights
        }
}

	 

#include "make_optimised_clusters.h"
using namespace std;


// Constructor
make_optimised_clusters::make_optimised_clusters(vector<int> Parent, vector<int> Child, vector<double> Weights, vector<double> CoreD){
	child=Child;
	parent=Parent;
	weights=Weights;
	coreD=CoreD;
	weights_sorted=Weights;
	size=CoreD.size();
}

void make_optimised_clusters::break_clusters(double threshold){
	int ii, jj, pre, kk, lbl=1;     // The zeroth lebel to assign, then it will be increased by 1 for each new cluster. The cluster labels will be (1,2,3,4....)
	vector<int> labeling_status(size, 0), connect;  // labeling_status stores if a given element has be relebelled in this cycle or not, connect stores
							// the parent of new clusters
	connect.push_back(1);          // The first cluster in this step will be directly a chind of the first cluster of the previous node

// Make the labeling status of the first node to be 1, in order to avoid complications
	labeling_status[0] = 1;
// Label all the elements that have self-edge (core distance) larger than the threshold
	for(ii=0; ii<labels.size(); ii++){
		if(coreD[ii] > threshold){
			labels[ii]=0;
			labeling_status[ii]=1;
		}
	}
// Looping over elements to detect breaks and relebel the resultant chind clusters
	for(jj=0; jj<labels.size(); jj++){
		 if(labels[jj] != 0){           // Enter the loop only if the element is not noise
			 if(labeling_status[jj] == 0){  // enter the loop only if the element has not already been lebeled this cycle
				 vector<int> branch;    // Branch stores the unlabeled connected elements from the current element to the first node until a break is found 
				 branch.push_back(jj);  // First member of branch is current elelment
				 pre=parent[jj];        // pre is the index of the parent element of current element
				 while(labeling_status[pre] == 0 && weights[pre] <= threshold){   // go backwards towards the first node until we find a 
												  // lebeled element or we encounter a break
					 branch.push_back(pre);                  
					 pre=parent[pre];
				 }

				 if(labeling_status[pre] == 1){               // update the branch to the label of the connected labeled node
					 lbl = labels[pre];
				 }
				 if(weights[pre] > threshold){               // if we encounter a break in tree, we have got a new cluster. 
									     // label will be +1 of the previous recent lebel of current cycle
					 lbl = lbl + 1;
					 connect.push_back(labels[pre]);   // For a new cluster undate the connections vector with it's parent label
				 }
				 for(kk=0; kk<branch.size(); kk++){        // update the labels and the labeling status of the branch
					 labels[branch[kk]] = lbl;
					 labeling_status[branch[kk]] = 1;
				 }
			 }
		 }
	}
	tree_connections.push_back(connect);                            // Add the connect vector to the set of vectors collecting the connections for the final
	full_labels.push_back(labels);
	labeling_status.clear();
	connect.clear();
}

// condense_clusters uses the 2D array of labels_full and weights_sorted to remove the noise falling off the clusters (small clusters with size less than core_size) and label them according to the inheritence 
void make_optimised_clusters::condense_clusters(){
	int ii, jj, kk, last_label;
	for(ii=0; ii<full_labels.size(); ii++){

		// Compute the number of distinct labels and size of cluster associated to each label 
		int num_clusters, inst_label=0, lsize=0;
		vector<int> label_size, label_list;
		for(jj=0; jj<full_labels[ii].size(); jj++){
			if(full_labels[ii][jj] != inst_label){
				inst_label=full_labels[ii][jj];
				if(jj != 0){
					label_size.push_back(lsize);
					lsize=1;
					label_list.push_back(full_labels[ii][jj-1]);
				}
				else{
					lsize=1;
					inst_label=full_labels[ii][jj];
				}
			}
			else{
				lsize = lsize + 1;
			}
		}


		// After we get list of labels and their size for this value of ii, the next step is to 
		// drop all clusters that have size less than core size
		for(jj=0; jj<label_list.size(); jj++){
			if(label_size[jj]<core){
				for(kk=0; kk<full_labels[ii].size(); kk++){
					if(label_list[kk]==full_labels[ii][jj]){
						full_labels[ii][jj]=0;
					}
				}
			}
		}

		// After dropping the smaller clusters, relabel hte remaining clusters accordign to their inheritence
		// After the smaller clusters are dropped, relabel them according to their inheritence
	}
}

//void make_optimised_clusters::extract_clusters(){}









void make_optimised_clusters::stat_clusters(){
	int ii, jj, kk=0, num_clusters;
	vector<int> lbls, sizes;
	num_clusters = *max_element(labels.begin(), labels.end());  // If the labeling of each step is like (1,2,3,4,...) the max of this will be the num_clusters
	for(ii=1; ii<num_clusters+1; ii++){                  // Lopp over the current cluster labels
		lbls.push_back(ii);                          // store the labels
		for(jj=0; jj<labels.size(); jj++){          // Compute the size of clusters for each label
			if(labels[jj] == ii){
				kk=kk+1;
			}
		}
		sizes.push_back(kk);		// Store the size
	}
	tree_labels.push_back(lbls);             // Update the gobal tree arrays
	tree_sizes.push_back(sizes);
	lbls.clear();
	sizes.clear();
}



void make_optimised_clusters::make_clusters(){

// Initiate the vector of labels with 1 
	for(i=0; i<size; i++){
		labels.push_back(1);
	}

// Sorting the vector of weights

        for(i=0; i<weights_sorted.size(); i++){
                for(j=0; j<weights_sorted.size()-1; j++){
                        if(weights_sorted[j] < weights_sorted[j+1]){
                                swap(weights_sorted[j], weights_sorted[j+1]);
                        }
                }
        }
	var=0.0;
// Calling relevant functions with decreasing weights
	for(i=0; i<weights_sorted.size(); i++){
		if(weights_sorted[i] != var){
			var = weights_sorted[i];
// compute the stats (size and label) of existing clusters
//			stat_clusters();
// Break the cluster usign the decreased weight
			break_clusters(var);
		}
	}
	condense_clusters();


}
	










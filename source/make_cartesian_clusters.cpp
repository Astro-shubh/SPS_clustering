#include "make_cartesian_clusters.h"
using namespace std;


// Constructor
make_cartesian_clusters::make_cartesian_clusters(vector<int> Parent, vector<int> Child, vector<double> Weights, int data_size){
	child=Child;
	parent=Parent;
	weights=Weights;
	size= data_size;
}



void make_cartesian_clusters::break_clusters(double threshold){
	int ii, jj, pre, kk, lbl=1, max_label=1;     // The zeroth lebel to assign, then it will be increased by 1 for each new cluster. The cluster labels will be (1,2,3,4....)
	vector<int> labeling_status(size, 0), list_label, list_size, list_parent;  // labeling_status stores if a given element has be relebelled in this cycle or not, connect stores
							// the parent of new clusters
//	connect.push_back(1);          // The first cluster in this step will be directly a chind of the first cluster of the previous node

// Make the labeling status of the first node to be 1, in order to avoid complications
// 	labeling_status[0] = 1;
// Looping over elements to detect breaks and relebel the resultant child clusters
	for(jj=0; jj<size; jj++){
		 if(labels[jj] != 0){           // Enter the loop only if the element is not noise
			 
			 if(labeling_status[jj] == 0){  // enter the loop only if the element has not already been lebeled this cycle
				 vector<int> branch;    // Branch stores the unlabeled connected elements from the current element to the first node until a break is found 
				 branch.push_back(jj);  // First member of branch is current elelment
				 pre=jj;        // pre is the index of the current element
				 while(parent[pre] != -1 && weights[pre] < threshold){   // go backwards towards the first node until we find a 
					 if(labeling_status[parent[pre]] == 1){
						 break;
					 }
					 pre=parent[pre]; 
					 branch.push_back(pre);
				 }

				 if(labeling_status[parent[pre]] == 1 && weights[pre] < threshold){               // update the branch to the label of the connected labeled node
					 lbl = labels[parent[pre]];
						 
				 }
				 else{
					max_label = max_label + 1;
					lbl = max_label;
					parent[pre] = -1;	
				 }


				 for(kk=0; kk<branch.size(); kk++){        // update the labels and the labeling status of the branch
					 labels[branch[kk]] = lbl;
					 labeling_status[branch[kk]] = 1;
				 }
			 }
		 }
	}
	
	vector<int> uniq_labels;
	int lsize, itr;
	uniq_labels = labels;
	sort(uniq_labels.begin(), uniq_labels.end());
	auto id = unique(uniq_labels.begin(), uniq_labels.end());
	uniq_labels.resize(distance(uniq_labels.begin(), id));
//	cout << uniq_labels.size() << endl;
	for(jj=0; jj< uniq_labels.size(); jj++){
		if(uniq_labels[jj] != 0){
			lsize = count(labels.begin(), labels.end(), uniq_labels[jj]);
			if(lsize < core){
				replace(labels.begin(), labels.end(), uniq_labels[jj], 0);
				for(kk=0; kk<labels.size(); kk++){
					if(labels[kk] == uniq_labels[jj]){
						parent[kk] = -1;
					}
				}
			}
			else{
                                list_label.push_back(uniq_labels[jj]);
                                list_size.push_back(lsize);
                                itr = full_labels.size();
                                if(itr == 0){
                                        list_parent.push_back(1);
                                }
                                else{
                                        auto id = find(labels.begin(), labels.end(), uniq_labels[jj]);
                                        list_parent.push_back(full_labels[itr-1][id - labels.begin()]);
                                }
                        }

		}
	}
	current_size = *max_element(list_size.begin(), list_size.end());
	tree_connections.push_back(list_parent);	// Add the connect vector to the set of vectors collecting the connections for the final
	tree_labels.push_back(list_label);
	tree_sizes.push_back(list_size);
	full_labels.push_back(labels);
}

// condense_cluslist_labelters uses the 2D array of labels_full and weights_sorted to remove the noise falling off the clusters (small clusters with size less than core_size) and label them according to the inheritence 

void make_cartesian_clusters::get_stabilities(int location, int stage){
	int current_label = tree_labels[stage][location];   // Stores the label of the new cluster
	int first_label = current_label;                    // Stores the original label of cluster
	int current_size = tree_sizes[stage][location];     // Stores the current size of the new cluster
	int relative_stage = 0;                               // gives the stage relative to the initial stage
	int parent = tree_connections[stage][location];     // Stores the parent of the new cluster
	vector<int> children;                               // Will store the children labels of the cluster
	vector<int> members;                                // Will store the member of the original cluster if stability is non-zero
	double stability = 0;                               // Cluster stability parameter thats gets updated at each survival stage of the cluster
	double birth = 1.0/braking_threshold[stage];        // Inverse of threshold at birth
	int birth_stage = stage;                            // Stores the stage at which cluster is born
	int death_stage;                                    // Will store the stage at which cluster disappears if stability is non zero
	while(stage+relative_stage+1 < tree_connections.size()){                         // Loop untill the size of cluster goes bellow the core size
		int freq=0, ii, jj;
		vector<int> indices, connection;            // Indices at which the current label matches to the parent of the next label 
							    // (at which children are located in next stage), connection is vector to store parent of next stage

		connection = tree_connections[stage+relative_stage+1];
		for(ii=0; ii<connection.size(); ii++){       // loop over all the parents of next step
			if(connection[ii] == current_label){ // See where the current current label is present in the parent of next stage
				indices.push_back(ii);       // Store the locations of children
				freq = freq+1;               // If the current label is present more than once (i.e. cluster is breaking)
			}
		}
		if(freq > 1){                                // If cluster is breaking
			death_stage = stage+relative_stage;
			for(ii=0; ii<freq; ii++){            // For each child in the next label
				get_stabilities(indices[ii], stage+relative_stage+1);           // Run the same function get_stability for the children
				children.push_back(tree_labels[stage+relative_stage+1][indices[ii]]);  // Push the children labels that is going to be
													// stored in cluster struct
				int new_size = tree_sizes[stage+relative_stage][indices[ii]];          // Size of the child
				double death = 1.0/braking_threshold[stage + relative_stage];          // Inverse of threshold at death
				stability = stability + (current_size - new_size)*(death - birth);     // Stability contribution from child elements
			}

			break;                                                                         // Break the while loop as the cluster is broken now
		}
		if(freq == 1){                                       // If cluster survived this stage
			int new_size = tree_sizes[stage+relative_stage+1][indices[0]];                   // new size of cluster at this stage
			double death = 1.0/braking_threshold[stage + relative_stage];                  // Inverse of threshold at this stage  
			stability = stability + (current_size - new_size)*(death - birth);             // Contributions from the fallen points between this
												       // and previous stage
			current_size = new_size;                                                       // Update the size of cluster
			current_label = tree_labels[stage+relative_stage+1][indices[0]];                 // Update the label
			relative_stage = relative_stage + 1;                                             // Update the relative stage
		}
		if(freq == 0){
			break;
		}
	}
	if(stability > 0){        // If the cluster stability is non zero (i.e. it survived for at least one stage)
		int ii;
		for(ii=0; ii<full_labels[birth_stage].size(); ii++){      // loop over the number of labels at that stage
			if(full_labels[birth_stage][ii] == first_label){  // if the label at the initial stage matches to the first label of the cluster
				members.push_back(ii);                    // Push that index to members (indexes in full_labels represent the index of DM-Time data point
			}
		}
		cluster clstr;                                            // Prepare a cluster type object
		clstr.elements = members;
		clstr.child = children;
		clstr.parent = parent;
		clstr.label = first_label;
		clstr.stability = stability;
		clstr.birth_stage = birth_stage;
		clstr.death_stage = death_stage;
		clusters_core.push_back(clstr);                           // Push this object to a vector of cluster objects
	}
}


void make_cartesian_clusters::traceback_stability(){
	int ii, jj, kk;
	int ia, ja, ka;
	vector<cluster> final_clusters;
	for(ii=0; ii<clusters_stable.size()-1; ii++){
		vector<cluster> clusters_stage, clusters_stage1;
		ia = clusters_stable.size() - ii -1;
		clusters_stage = clusters_stable[ia];
		clusters_stage1 = clusters_stable[ia-1];
		for(jj = 0; jj<clusters_stage.size(); jj++){
			cluster clstr, clstr1;
			int current_label = clusters_stage[jj].label;
			vector<int> label_list;
//			cout << clusters_stable[ia-1].size() << endl;
			for(kk=0; kk<clusters_stable[ia-1].size(); kk++){
//				cout << "in loop " << kk << endl;
				label_list.push_back(clusters_stable[ia-1][kk].label);

			}
			auto id0 = find(label_list.begin(), label_list.end(), current_label);
			if(id0 != label_list.end()){                            // If the current label was found in the previous stage
				int idx = id0 - label_list.begin();
				clusters_stable[ia-1][idx].stability = clusters_stage[jj].stability;
			}
			else{                                                  // If the current label is not found in the previous stage (i.e. it's child of some label or a new cluster appearing
				vector<cluster> parent_clusters;
				vector<int> parent_idx;
				for(kk=0; kk<clusters_stage1.size(); kk++){
					if(clusters_stage1[kk].child.size() > 1){
						parent_clusters.push_back(clusters_stage1[kk]);
						parent_idx.push_back(kk);
					}
				}
				int parent_exist = 0, pidx, ll;
				for(kk=0; kk<parent_clusters.size(); kk++){
					for(ll=0; ll<parent_clusters[kk].child.size(); ll++){
						if(parent_clusters[kk].child[ll] == current_label){
							parent_exist = 1;
							pidx = parent_idx[kk];
						}
					}
				}

				if(parent_exist == 1){
					double children_stability = 0.0;
					int ll;
					for(kk=0; kk<clusters_stage1[pidx].child.size(); kk++){
						for(ll=0; ll<clusters_stage.size(); ll++){

							if(clusters_stage1[pidx].child[kk] == clusters_stage[ll].label){
								children_stability = children_stability + clusters_stage[ll].stability;
							}

						}
					}
					
					if(children_stability > clusters_stage1[pidx].stability){
						clusters_stable[ia-1][pidx].stability = children_stability;
					}
//					else{
//						cout << "cluster is stable" << endl;
//						final_clusters.push_back(clusters_stable[ia-1][pidx]);
//					}
				}
			}
		}
	}

	//   Now go through these clusters and find the stable ones
	
}

void make_cartesian_clusters::get_stable_clusters(){
	int ii, jj, kk;                        // Integers to use in loops
	int stage, num_labels;                 // stage of the breaking, number of labels including noise
	int max_lbl=1;                           // maximum label at current stage   
	vector<int> stage_lbls, stage1_lbls, uniq_lbls;     // All labels at this stage
	for(ii=0; ii<full_labels.size()-1; ii++){   // Loop over all braking stages
		stage_lbls = full_labels[ii];       // Stores labels of current stage
		stage1_lbls = full_labels[ii+1];    // Sotres labels of the next stage
		uniq_lbls = stage_lbls;             // Will eventually store the number of uniq_labels at current stage including noise (0 labels)
		vector<cluster> clusters_array;
		sort(uniq_lbls.begin(), uniq_lbls.end());           
		auto id = unique(uniq_lbls.begin(), uniq_lbls.end());
		uniq_lbls.resize(distance(uniq_lbls.begin(), id));  
		num_labels = uniq_lbls.size();
		int max1;
		max1 = *max_element(stage1_lbls.begin(), stage1_lbls.end());               // maximum label at the current stage
		max_lbl = max(max1, max_lbl);
		for(jj=0; jj<num_labels; jj++){			// ** Careful with this loop, assumes noise to be present in every stage **, if posible, use a condition.
		if(uniq_lbls[jj] != 0){
			vector<int> elements, child_labels, child_idx;        // Stores the index of the current label and child labels for the current label
			for(kk=0; kk < stage_lbls.size(); kk++){
				if(stage_lbls[kk] == uniq_lbls[jj]){
					elements.push_back(kk);
					if(stage1_lbls[kk] != 0){
						child_labels.push_back(stage1_lbls[kk]);  // Only stores non noise child labels
						child_idx.push_back(kk);
					}
				}
			}
			if(child_labels.size() > 0){                                    // If the current label has not disappeared in noise
				vector<int> uniq_lbls1;
				uniq_lbls1 = child_labels;                               // Will get the non-zero uniq child labels for current label in the stage
				sort(uniq_lbls1.begin(), uniq_lbls1.end());
				auto id1 = unique(uniq_lbls1.begin(), uniq_lbls1.end());
				uniq_lbls1.resize(distance(uniq_lbls1.begin(), id1));
				if(uniq_lbls1.size() == 1){                    // If there is only one child of the current label (i.e. cluster will survive next breaking
					if(clusters_stable.size() > 0){       // If there are already layers of clusters from previous stages
						vector<int> clstr_labels;
						for(kk=0; kk<clusters_stable[clusters_stable.size()-1].size(); kk++){  // Get the cluster with same label from previous step
							clstr_labels.push_back(clusters_stable[clusters_stable.size()-1][kk].label);
						}
						auto id2 = find(clstr_labels.begin(), clstr_labels.end(), uniq_lbls[jj]);
						if(id2 != clstr_labels.end()){
							cluster clstr;
							int idx = id2 - clstr_labels.begin();
							clstr = clusters_stable[clusters_stable.size()-1][idx];
							clstr.child = uniq_lbls1;     // Update the child labels (though only single child at this stage)
							clstr.stability = clstr.stability + (clstr.elements.size() - elements.size())*(1/braking_threshold[ii] - 1/braking_threshold[clstr.birth_stage]);
							clstr.elements = elements;    // Update the elements at this stage
							clusters_array.push_back(clstr);  // Store the cluster in the cluster array of the current stage
						}
						else{
							cluster clstr;
							clstr.elements = elements;
							clstr.child = uniq_lbls1;
							clstr.parent = uniq_lbls[jj];
							clstr.label = uniq_lbls[jj];
							clstr.stability = 0;
							clstr.birth_stage = ii;
							clstr.death_stage = 0;
							clusters_array.push_back(clstr);
						}


								
					} 
					else{				// If the vector of cluster arrays is empty, mabe its first step or there have been no stable label yet
						cluster clstr;               // Initialize a cluster now as this label is surviving 
						clstr.elements = elements;
						clstr.child = uniq_lbls1;
						clstr.parent = uniq_lbls[jj];    // Parent will be this label and should not be updated in the following stages
						clstr.label = uniq_lbls[jj];     // Current label is the label, this will also be same up to the point the cluster is surviving as we will be updating it
						clstr.stability = 0;             // initialize stability as 0
						clstr.birth_stage = ii;          // Birth stage will rmain same
						clstr.death_stage = 0;           // Death stage is not known yet
						clusters_array.push_back(clstr); // Push back this into the cluster array of current stage
					}
					for(kk=0; kk<child_idx.size(); kk++){
						full_labels[ii+1][child_idx[kk]] = uniq_lbls[jj];
					}
//					cout << " replacing " << uniq_lbls1[0] << " by " << uniq_lbls[jj] << endl;
//					replace(full_labels[ii+1].begin(), full_labels[ii+1].end(), uniq_lbls1[0], uniq_lbls[jj]); // Label is going to survive and hence replace 
																   // all the child labels by the current label
				}


				if(uniq_lbls1.size() > 1){      // If there are more than one children (i.e. cluster is splitting)
					cluster clstr;
					if(clusters_stable.size() > 0){    // If there already are parent clusters stored for this label
						vector<int> clstr_labels;
						for(kk=0; kk<clusters_stable[clusters_stable.size()-1].size(); kk++){
							clstr_labels.push_back(clusters_stable[clusters_stable.size()-1][kk].label);
						}
						auto id2 = find(clstr_labels.begin(), clstr_labels.end(), uniq_lbls[jj]);
						if(id2 != clstr_labels.end()){
							int idx = id2 - clstr_labels.begin();
							clstr = clusters_stable[clusters_stable.size()-1][idx];
							clstr.stability = clstr.stability + (clstr.elements.size())*(1/braking_threshold[ii] - 1/braking_threshold[clstr.birth_stage]);
							clstr.elements = elements;     // Update the elements
							clstr.death_stage = ii;        // Now we know the death stage of the cluster
							//clusters_array.push_back(clstr); // Push back the final stage of the cluster
						}
						else{
							clstr.elements = elements;
							//clstr.child = uniq_lbls1;
							clstr.parent = uniq_lbls[jj];
							clstr.label = uniq_lbls[jj];
							clstr.stability = 0;
							clstr.birth_stage = ii;
							clstr.death_stage = ii;
							//clusters_array.push_back(clstr);
						}
					}
					else{
						clstr.elements = elements;
						clstr.parent = uniq_lbls[jj];
						clstr.label = uniq_lbls[jj];
						clstr.stability = 0;
						clstr.birth_stage = ii;
						clstr.death_stage = ii;
					}

				  	vector<int> next_children;	
					for(kk=0; kk<uniq_lbls1.size(); kk++){
						int ll;
						vector<int> child_idx1;
						for(ll=0; ll<child_idx.size(); ll++){
							if(full_labels[ii+1][child_idx[ll]] == uniq_lbls1[kk]){
								child_idx1.push_back(child_idx[ll]);
							}
						}
						next_children.push_back(max_lbl+1+kk);

						for(ll=0; ll<child_idx1.size(); ll++){
							full_labels[ii+1][child_idx1[ll]] = max_lbl+1+kk;
						}
//						replace(full_labels[ii+1].begin(), full_labels[ii+1].end(), uniq_lbls1[kk], max_lbl+1+kk); // Relabel the child clusters
					}
					clstr.child = next_children;
					clusters_array.push_back(clstr);

					max_lbl = max_lbl+uniq_lbls1.size();  // update the max label

				}

			}
			else{                 // If there is no non zero child of the current cluster (i.e. cluster has become noise now)
				if(clusters_stable.size() != 0){ // If there is any previous cluster for this label, store it's final stage
					vector<int> clstr_labels;
					for(kk=0; kk<clusters_stable[clusters_stable.size()-1].size(); kk++){
						clstr_labels.push_back(clusters_stable[clusters_stable.size()-1][kk].label);
					}
					auto id2 = find(clstr_labels.begin(), clstr_labels.end(), uniq_lbls[jj]);
					if(id2 != clstr_labels.end()){
						cluster clstr;
						int idx = id2 - clstr_labels.begin();
						clstr = clusters_stable[clusters_stable.size()-1][idx];
						clstr.stability = clstr.stability + (clstr.elements.size())*(1/braking_threshold[ii] - 1/braking_threshold[clstr.birth_stage]);
						clstr.elements = elements;
						clstr.death_stage = ii;
						clusters_array.push_back(clstr);
					}
				}   // If there is no previous cluster for this label and its breaking at this stage, no point in storing this
			}

		}   // if statement for uniq_label[jj] != 0 ends here
		}  // Loop over jj (i.e. index for uniq labels in current stage) ends here
		clusters_stable.push_back(clusters_array);   // Store the current stage clusters for all jj values in the clusters_stable for each ii
	}          // Loop over ii (stages) ends here
}

void make_cartesian_clusters::extract_clusters(int stage, int index){
	cluster current_cluster = clusters_stable[stage][index];
	vector<cluster> next_stage = clusters_stable[stage+1];
	int num_child = current_cluster.child.size();
	int ii, jj, kk;
	if(num_child == 1){
//		// Try to find the current label in next_stage
		vector<int> list_labels;
		for(ii = 0; ii<next_stage.size(); ii++){
			list_labels.push_back(next_stage[ii].label);
		}
		auto id0 = find(list_labels.begin(), list_labels.end(), current_cluster.label);
		int idx;
		if(id0 != list_labels.end()){
				idx = id0 - list_labels.begin();
				if(stage+1 < clusters_stable.size()-1){
					extract_clusters(stage+1, idx);
				}
		}
//
		else{
			good_clusters.push_back(current_cluster);
		}
		
		// if same label is found, recall the next stage
		// if the same label is not found in the next_stage, sotre the cluster in the good_clusters array
	}
	else{
		// calculate the stability of children clusters
		double children_stability = 0.0;
		vector<int> indexes;
		for(ii=0; ii<num_child; ii++){
			for(jj=0; jj<next_stage.size(); jj++){
				if(next_stage[jj].label == current_cluster.child[ii]){
					children_stability = children_stability + next_stage[jj].stability;
					indexes.push_back(jj);
				}
			}
		}
		if(current_cluster.stability > 1.01*children_stability){
			good_clusters.push_back(current_cluster);
		}
		else{
			if(stage+1 < clusters_stable.size()-1){
				for(kk=0; kk<indexes.size(); kk++){
					extract_clusters(stage+1, indexes[kk]);
				}
			}
		}
	



		
		// if(children_stability > parent_stability){ call the extract_clusters for all children of the current cluster)
		// else{ store the cluster in good_cluster array
	}
}

void make_cartesian_clusters::stat_clusters(){
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



void make_cartesian_clusters::make_clusters(){
//	int brk_unit =2;
	int ii, jj;
// Initiate the vector of labels with 1 
	for(i=0; i<size; i++){
		labels.push_back(1);
	}

// Sorting the vector of weights
	vector<double> thresholds;
	for(i=0; i<weights.size(); i++){
		thresholds.push_back(double(int(weights[i])));
	}
	sort(thresholds.begin(), thresholds.end());
	auto id = unique(thresholds.begin(), thresholds.end());
	thresholds.resize(distance(thresholds.begin(), id));
	reverse(thresholds.begin(), thresholds.end());
	braking_threshold = thresholds;
//	for(i=0; i<thresholds.size(); i++){cout << thresholds[i] << endl;}

// Calling relevant functions with decreasing weights
	for(i=0; i < thresholds.size()-1; i++){
// Break the cluster usign the decreased weight
		break_clusters(thresholds[i]);
		if(current_size <= core){
			break;
		}
	
	}

	get_stable_clusters();
	traceback_stability();
	for(ii=0; ii<clusters_stable[0].size(); ii++){
		extract_clusters(0, ii);
	}
	ofstream good_labels("good_cluster_labels.txt");
	for(ii=0; ii<good_clusters.size(); ii++){
		for(jj=0; jj<good_clusters[ii].elements.size(); jj++){
			good_labels << good_clusters[ii].elements[jj] << " ";
		}
		good_labels << endl;
	}
	good_labels.close();

}
	










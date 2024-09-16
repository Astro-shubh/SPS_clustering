#ifndef MAKE_CARTESIAN_CLUSTERS_H
#define MAKE_CARTESIAN_CLUSTERS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <cfloat>
#include <bits/stdc++.h>
#include <algorithm>
#include <chrono>

class make_cartesian_clusters{
        public:
		struct cluster{
			std::vector<int> elements;
			std::vector<int> child;
			int parent;
			int label;
			float stability;
			int birth_stage;
			int death_stage;
		};
		std::vector<cluster> clusters_core;
		std::vector<cluster> good_clusters;
		std::vector<std::vector<cluster>> clusters_stable;
                std::vector<float> weights, weights_sorted;
                std::vector<int> child, parent, labels;
// do_clustering() executes all the functions sequentially
		make_cartesian_clusters(std::vector<int> Parent, std::vector<float> Weights, int data_size, int Core);
// make_clusters() sorts the weights and calls the relevant functions with decreasing order of threshold
		void make_clusters();
// 2D Vectors to store the dendogram of clustering, tree_labels to store the labels of clusters, tree_connections to store the labels of previous stage, 
// tree sizes to store the size of the clusters

		std::vector<std::vector<int>> full_labels;
        private:
// make_clusters() sorts the weights and calls the relevant functions with decreasing order of threshold
// break_clusters() breaks the Minimum spanning tree into clusters and relabels them according to the splits. 
// This also stores the connections of new clusters to older set of cluster in previous step
                void break_clusters(float threshold);
		void extract_clusters(int stage, int index);
		void get_stable_clusters();
		void traceback_stability();
// stat_clusters() stores the labels and sizes of each clusters at each state
//		void extract_clusters();
// core is used to compute the core distance
                int core;
                int i, j, k, size, current_size;
		std::vector<float> braking_threshold;
                float var;
};

#endif

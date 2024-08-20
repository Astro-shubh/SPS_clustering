#ifndef MAKE_OPTIMISED_CLUSTERS_H
#define MAKE_OPTIMISED_CLUSTERS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <cfloat>
#include <bits/stdc++.h>

class make_optimised_clusters{
        public:
                std::vector<double> weights, weights_sorted, coreD;
                std::vector<int> child, parent, labels;
// do_clustering() executes all the functions sequentially
		make_optimised_clusters(std::vector<int> Parent, std::vector<int> Child, std::vector<double> Weights, std::vector<double> CoreD);
// make_clusters() sorts the weights and calls the relevant functions with decreasing order of threshold
		void make_clusters();
// 2D Vectors to store the dendogram of clustering, tree_labels to store the labels of clusters, tree_connections to store the labels of previous stage, 
// tree sizes to store the size of the clusters

		std::vector<std::vector<int>> tree_labels, tree_connections, tree_sizes, full_labels;
        private:
// make_clusters() sorts the weights and calls the relevant functions with decreasing order of threshold
// break_clusters() breaks the Minimum spanning tree into clusters and relabels them according to the splits. 
// This also stores the connections of new clusters to older set of cluster in previous step
                void break_clusters(double threshold);
// stat_clusters() stores the labels and sizes of each clusters at each state
                void stat_clusters();
		void condense_clusters();
// core is used to compute the core distance
                const int core=10;
                int i, j, k, size;
                double var;
};

#endif

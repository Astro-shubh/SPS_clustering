#ifndef KD_TREE_H
#define KD_TREE_H
#include <iostream>
#include <vector>
#include <chrono>
#include <bits/stdc++.h>
#include <math.h>
#include <cmath>

class KD_tree{
	public:
		struct tree_node{
        		tree_node *left;
        		tree_node *right;
        		int data_index;
		};
		int data_len, size, core;
		std::vector<double> X, Y;
		std::vector<double> coreD;
		std::vector<int> data_idx;
		KD_tree(std::vector<int> data_id, std::vector<double> X_locations, std::vector<double> Y_locations, int core_size);
		void construct_tree();
		void compute_coreD();
		void print_tree(tree_node *node);
	private:
	struct tree_node* root;

	struct tree_node* make_node(std::vector<int> data, int depth);
	std::vector<int> return_elements(tree_node *node, std::vector<int> elements);
	double dist(int idx1, int idx2);
	tree_node* find_parent(tree_node* root, int index);
	std::vector<int> get_branch(tree_node *node, int index);
	std::vector<int> query_distances(tree_node *node, std::vector<int> distances, int index, int depth);
	double find_coreD(std::vector<int> elements, int index);
};
#endif	


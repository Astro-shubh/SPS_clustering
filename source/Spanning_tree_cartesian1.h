#ifndef SPANNING_TREE_CARTESIAN_H
#define SPANNING_TREE_CARTESIAN_H

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <cfloat>
//#include <omp.h>
class Spanning_tree_cartesian{
        public:
                std::vector<float> X_locations, Y_locations;
                std::vector<float> Weights;
                std::vector<int> Parent;
                Spanning_tree_cartesian(std::vector<float> X, std::vector<float> Y);
                void construct_tree();
        private:
                float x_dist, y_dist, distance;
                std::vector<float> dist_array, sorted_dist;
                const int core=10;
                int size;
                float Cartesian(int idx1, int idx2);
                int key_for_treenodes(const std::vector<float>& key, const std::vector<bool>& MST_status);

};
#endif


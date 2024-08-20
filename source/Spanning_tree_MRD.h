#ifndef SPANNING_TREE_MRD_H
#define SPANNING_TREE_MRD_H

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <cfloat>
#include <omp.h>
class Spanning_tree_MRD{
        public:
                std::vector<double> X_locations, Y_locations, Core_distance;
                std::vector<double> Weights;
                std::vector<int> Parent;
                Spanning_tree_MRD(std::vector<double> X, std::vector<double> Y);
                void construct_tree();
                void Compute_coredist();
		void Compute_coredist1();
        private:
                double x_dist, y_dist, distance;
                std::vector<double> dist_array, sorted_dist;
                const int core=10;
                int size;
                double Cartesian(int idx1, int idx2);
                double get_coreD(std::vector<double> Dist);
                double MRD(int point1, int point2);
                int key_for_treenodes(std::vector<double> key, std::vector<bool> MST_status);

};
#endif


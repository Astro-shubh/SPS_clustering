#include "source/Spanning_tree_MRD.h"

using namespace std;
int main(){
        vector<double> X_locations, Y_locations;
        int i,j;

        for (i=0; i<100; i++){
                X_locations.push_back(i*5.0);
                Y_locations.push_back(i*15.0);
        }

        Spanning_tree_MRD Tree(X_locations, Y_locations);
        Tree.Compute_coredist();
//      for(i=0; i<Tree.Core_distance.size(); i++){
//              cout << Tree.Core_distance[i] << endl;
//      }
        Tree.construct_tree();

        for(i=0; i<X_locations.size(); i++){
                cout << to_string(i)+"  "+to_string(Tree.Parent[i])+"  "+to_string(Tree.Weights[i]) << endl;
        }

}


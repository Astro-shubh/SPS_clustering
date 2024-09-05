Compilation : g++ -o test_pipeline test_pipeline.cpp source/read_spccl.cpp source/split_string.cpp source/prepare_data.cpp source/Spanning_tree_cartesian.cpp source/make_cartesian_clusters.cpp source/width_clustering.cpp 

Three Python scripts are available to plot the results: 1. plot_MST.py plots the minimum spanning tree, 2. plot_clusters.py plots clusters at each stage of breaking, 3. good_clusters.py plots the stable clusters after HDBSCAN clustering, 4. final_clusters.py plots final clusters after width based clustering on the stable clusters.

A number of .spccl files are available to test the clustering.

Please see the following document for details of implementation and uses: https://docs.google.com/document/d/1SBVH_nsud5A1wDQO5XZtqhMVBuVyXigxdFSI04RXcXU/edit?usp=sharing

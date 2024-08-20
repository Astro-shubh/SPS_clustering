// Takes the string array obtained from the spccl files for MJD, DM, Width, and SNR and change it to a vector of double precision floating point number. This also has
// functionality to change the DM to ms units and MJD to ToAs in miliseconds
#ifndef PREPARE_DATA_H
#define PREPARE_DATA_H

#include <iostream>
#include <fstream>
#include<bits/stdc++.h>

class prepare_data{
        public:
                std::vector<std::string> input_vector;
                prepare_data(std::vector<std::string> V);
                std::vector<double> output_vector;
                std::vector<double> processing_vector;
		std::vector<double> ToA;
		std::vector<double> Time;
		std::vector<double> Width;
		std::vector<double> Sigma;
                void MJD_to_ToA();
                void DM_to_Time();
                void str_to_double();
};

#endif

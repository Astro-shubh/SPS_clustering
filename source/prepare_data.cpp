// Takes the string array obtained from the spccl files for MJD, DM, Width, and SNR and change it to a vector of double precision floating point number. This also has
// functionality to change the DM to ms units and MJD to ToAs in miliseconds

#include "prepare_data.h"

using namespace std;

void prepare_data::MJD_to_ToA(){
	processing_vector = output_vector;
	float first_entry = processing_vector[0];
	int i;
	for(i=0; i < processing_vector.size(); ++i){
		ToA.push_back((processing_vector[i]-first_entry)*24.0*3600.0*1000.0);
//		cout << processing_vector[i] << endl;
	}
}

void prepare_data::DM_to_Time(){
        float const DM_constant=1/241.0;
	float const nu1=1.350;
	float const nu2=1.670;
	processing_vector = output_vector;
	int i;
	for(i=0; i < processing_vector.size(); ++i){
		Time.push_back(processing_vector[i]*DM_constant*(1/(nu1*nu1) - 1/(nu2*nu2))*1000.0);
//		cout << processing_vector[i]*DM_constant*(1/(nu1*nu1) - 1/(nu2*nu2))*1000.0 << endl;
	}
}

void prepare_data::str_to_double(){
	int i;
	for(i=0; i < input_vector.size(); ++i){
//		cout << i << endl;
		output_vector.push_back(stod(input_vector[i]));
//		cout << input_vector[i] << endl;
	}
}
void prepare_data::str_to_float(){
        int i;
        for(i=0; i < input_vector.size(); ++i){
//              cout << i << endl;
                output_vector1.push_back(stof(input_vector[i]));
//              cout << input_vector[i] << endl;
        }
}

prepare_data::prepare_data(vector<string> V){
	input_vector = V;
//	cout << input_vector.size() << endl;
}





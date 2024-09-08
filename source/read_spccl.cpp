#include "read_spccl.h"
#include "split_string.h"
#include "prepare_data.h"

using namespace std;

void read_spccl::read(){          // Function to perform the read
        int i=0;
        ifstream infile;
        infile.open(filename);
        string line;
        while(getline(infile,line)){
                if(i != 0){                                  ///   ignore the header of the spccl file
                        vector<string> v = string_split(line);
                        MJD.push_back(v[0]);
                        DM.push_back(v[1]);
                        Width.push_back(v[2]);
                        Sigma.push_back(v[3]);
                }
                ++i;
        }
        infile.close();
}

void read_spccl::prepare(){
	prepare_data MJD_Obj(MJD);
	MJD_Obj.str_to_double();
	MJD_Obj.MJD_to_ToA();
	ToA=MJD_Obj.ToA;
	prepare_data DM_Obj(DM);
	DM_Obj.str_to_double();
	DM_Obj.DM_to_Time();
	DM_Time=DM_Obj.Time;
	prepare_data Width_Obj(Width);
	Width_Obj.str_to_float();
	WidthD=Width_Obj.output_vector1;
	prepare_data Sigma_Obj(Sigma);
	Sigma_Obj.str_to_float();
	SigmaD=Sigma_Obj.output_vector1;

}
void read_spccl::message(){cout << filename << endl;}
read_spccl::read_spccl(string file_name) {filename=file_name;}


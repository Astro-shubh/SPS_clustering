#include <iostream>
#include <fstream>
#include<bits/stdc++.h>

using namespace std;


//    Function string split takes a string as an input and splits it into multiple strings that were separated by groups of spaces and returns a vector with all strings

vector<string> string_split(string line){
	vector<string> v;
	string s="";
	line=line+'\n';
	int i,j;
	j=1;
	for(i=0;i<line.size();i++){
		if(line[i] == ' ' || line[i] == '\n' || line[i] == '	'){
			if(j == 1){
				v.push_back(s);
				s="";
				j=0;
			}
			else{
				j=0;
			}
		}
		else{
			j=1;
		}
		if(j == 1){
			s=s+line[i];
		}
	}
	return v;
}


class read_spccl
{
	public:
		string filename;
		read_spccl(string file_name);	       
		
		vector<string> MJD;
		vector<string> DM;
		vector<string> Width;
		vector<string> Sigma;

		void read();
		void message();
};

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
		i++;
	}
	infile.close();
}

void read_spccl::message(){cout << filename << endl;}
read_spccl::read_spccl(string file_name) {filename=file_name;}




int main(){
	string filename="2012_03_14_00_00_00.spccl";
	long double d, d1;
	read_spccl spccl(filename);
	spccl.message();
	spccl.read();
//	cout << fixed << setprecision(numeric_limits<double>::max_digits10) << spccl.Width[161] << endl;
}
	

#ifndef READ_SPCCL_H
#define READ_SPCCL_H
#include <iostream>
#include <fstream>
#include<bits/stdc++.h>

class read_spccl
{
        public:
		std::string filename;
		read_spccl(std::string file_name);
		void prepare();

		std::vector<double> ToA;
		std::vector<double> DM_Time;
		std::vector<double> WidthD;
		std::vector<double> SigmaD;

                void read();
                void message();

	private:
                std::vector<std::string> MJD;
                std::vector<std::string> DM;
                std::vector<std::string> Width;
                std::vector<std::string> Sigma;

};

#endif

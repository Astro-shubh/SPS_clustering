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

		std::vector<float> ToA;
		std::vector<float> DM_Time;
		std::vector<float> WidthD;
		std::vector<float> SigmaD;

                void read();
                void message();

	private:
                std::vector<std::string> MJD;
                std::vector<std::string> DM;
                std::vector<std::string> Width;
                std::vector<std::string> Sigma;

};

#endif

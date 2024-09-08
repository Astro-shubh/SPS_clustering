#include "split_string.h"
using namespace std;


//    Function string split takes a string as an input and splits it into multiple strings that were separated by groups of spaces and returns a vector with all strings

vector<string> string_split(string line){
        vector<string> v;
        string s="";
        line=line+'\n';
        int i,j;
        j=1;
        for(i=0;i<line.size();++i){
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


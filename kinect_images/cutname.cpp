//
// Created by ubuntu on 19-4-14.
//

#include <iostream>
#include <sys/types.h>

#include <vector>
#include <string.h>
#include<fstream>
#include"algorithm"
using namespace std;

int main(){
	vector<string> sortpure;
        string s_raw;
        string s_pure1;
	string s_pure2;
	string s1;
        string s2;
        ifstream nameraw;
        ofstream namepure;
        nameraw.open("rgbraw.txt");
        namepure.open("rgb.txt");
        while(getline(nameraw,s1))
        {
            s_pure1 = s1.substr(5, 17);
	    sortpure.push_back(s_pure1);

        }
        nameraw.close();
        
	sort(sortpure.begin(), sortpure.end());
	for(int i=0; i <sortpure.size(); i++)
        {
	
	s_pure2 = sortpure[i].substr(0, 12);
	namepure<<"15552"<<s_pure2<<" "<<"imagesv2.0/"<<"15552"<<sortpure[i]<<endl;
        }    
        namepure.close();
        return 0;
}

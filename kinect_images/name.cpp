//头文件
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <string.h>
#include<fstream>
using namespace std;
 
 
void GetFileNames(string path,vector<string>& filenames)
{
    DIR *pDir;
    struct dirent* ptr;
    if(!(pDir = opendir(path.c_str())))
        return;
    while((ptr = readdir(pDir))!=0) {
        if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0)
	  
            filenames.push_back(ptr->d_name);
        
        }
    closedir(pDir);
}
 
int main() {
    vector<string> file_name;

    string path = "/home/ubuntu/visual_slam/dataset/kinect_dataset/imagesv2.0";
 
    GetFileNames(path, file_name);
	
	
	ofstream nameraw;
	nameraw.open("rgbraw.txt");
	
    for(int i = 0; i <file_name.size(); i++)
    { 
		
        nameraw<<file_name[i]<<endl;
    }
	
	nameraw.close();
    return 0;
}

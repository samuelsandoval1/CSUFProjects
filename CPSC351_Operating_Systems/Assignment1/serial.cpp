#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;
int main()
{
	vector<string> myUrls;
	string currLink;
	ifstream my_file;
	my_file.open("urls.txt");
	while(getline(my_file, currLink))
	{
		myUrls.push_back(currLink);
	}
	my_file.close();
	pid_t pid;
	string fileNamePrefix = "log";
	string fileName;
	for(int i = 0; i < myUrls.size(); i++)
	{
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			exit(-1);
		}
		else if(pid == 0)
		{

			fileName = fileNamePrefix + to_string(i+1) + ".txt";
			if(execlp("/usr/bin/wget", "wget", myUrls[i].c_str(), "-o", fileName.c_str(), NULL) < 0)
			{
				perror("execlp");
				exit(-1);
			}
		}
		else
		{
			if(wait(NULL) < 0)
			{
				perror("wait");
				exit(-1);
			}
		}
	}
	return 0;
}

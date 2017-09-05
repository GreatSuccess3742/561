// 561hw1.cpp : Defines the entry point for the console application.
//

#include<iostream>
#include<fstream>
#include<stdio.h>
#include<string>

using namespace std;

int main()
{
	string buffer;
	string algorithms;	
	//width & height of the square nursery
	int n = 0;
	//number of lizzards
	int p = 0;

	fstream inputFile;
	inputFile.open("input.txt", fstream::in );
	if (inputFile.is_open()) {
		getline(inputFile,algorithms);
		getline(inputFile, buffer);
		n = stoi(buffer);
		buffer.clear();
		getline(inputFile, buffer);
		p = stoi(buffer);
		buffer.clear();
		
	}
	else {
		perror ("Error opening the file.");
	}

    return 0;
}


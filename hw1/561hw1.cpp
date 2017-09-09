// 561hw1.cpp : Defines the entry point for the console application.
//

#include<iostream>
#include<fstream>
#include<stdio.h>
#include<string>
#include<vector>
#include <queue>

using namespace std;

// Global Variables

// Width & height of the square nursery
int n = 0;

// Number of lizzards
int p = 0;

class node {
    node* parent;
    node* child;
    int depth;
    public:
    char** state;
    bool CheckLizzardSafety(int x, int y){
        if(state[x][y] != '0') return false;
        for(int i = y; i >= 0; i--){
            if(state[x][i] == '2') break;
            if(state[x][i] == '1') return false;
        }
        for(int i = y; i < n; i++){
            if(state[x][i] == '2') break;
            if(state[x][i] == '1') return false;
        }
        for(int i = x; i >= 0; i--){
            if(state[i][y]=='2') break;
            if(state[i][y]=='1') return false;
        }
        for(int i = x; i < n; i++){
            if(state[i][y] == '2') break;
            if(state[i][y] =='1') return false;
        }
        for(int i = x, j = y; i >= 0 && j >= 0; i--,j--){
            if(state[i][j] == '2') break;
            if(state[i][j] =='1') return false;
        }
        for(int i = x, j = y; i < n && j < n; i++,j++){
            if(state[i][j] == '2') break;
            if(state[i][j] =='1') return false;
        }
        for(int i = x, j = y; i >= 0 && j < n; i--,j++){
            if(state[i][j] == '2') break;
            if(state[i][j] =='1') return false;
        }
        for(int i = x, j = y; i < n && j >= 0; i++,j--){
            if(state[i][j] == '2') break;
            if(state[i][j] =='1') return false;
        }
        return true;
    }
    void UpdateState(int n, char** newState){
        state = new char* [n];
        for(int i = 0; i < n; i++){
            state[i] = new char[n];
            memcpy(state[i], newState[i], n*sizeof(char));
        }
    }
};

void PrintResult(char ** state,int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout<<state[i][j]<<" ";
        }
        cout<<endl;
    }
}

void BFS(char** state, int numOfLizzard){
    queue<node> myQueue;
    int numOfInsertedLizzard = 0;
    node newNode;
    newNode.UpdateState(n, state);
    myQueue.push(newNode);
    while(!myQueue.empty()){
        unsigned int length = myQueue.size();
        cout<<"length = "<<length<<endl;
        for(int i = 0; i < length; i++){
            node& firstNode= myQueue.front();
            for(int x= 0; x< n; x++){
                for(int y = 0; y < n; y++){
                    if(firstNode.CheckLizzardSafety(x, y)){
                        node temp;
                        temp.UpdateState(n, firstNode.state);
                        temp.state[x][y] = '1';
                        myQueue.push(temp);
                    }
                }
            }
            myQueue.pop();
        }
        numOfInsertedLizzard++;
        if(numOfInsertedLizzard >= numOfLizzard && (!myQueue.empty()) ){
            // Output result
            cout<<"myq"<<myQueue.size()<<endl;
            cout<<"OK"<<endl;
            PrintResult(myQueue.front().state,n);
            break;
        }
        else{
            cout<<"FAIL"<<endl;
            break;
        }
    }
}

bool DFS(node currentNode, int numOfLizzard, int numOfInsertedLizzard){
    if(numOfInsertedLizzard > numOfLizzard){
        PrintResult(currentNode.state, n);
        return true;
    }
    else{
        for(int x = 0; x < n; x++){
            for(int y = 0; y < n; y++){
                if(currentNode.CheckLizzardSafety(x, y)){
                    node newNode;
                    newNode.UpdateState(n, currentNode.state);
                    newNode.state[x][y] = '1';
                    if (DFS(newNode, numOfLizzard, numOfInsertedLizzard + 1))
                        return true;
                }
            }
        }
        return false;
    }
}

int main()
{
	string buffer;
	string algorithms;


	fstream inputFile;
	inputFile.open("/Users/erichsieh/GoogleDrive/561/hw/561/hw1/input.txt", fstream::in );
	if (inputFile.is_open()) {
		getline(inputFile,algorithms);
		getline(inputFile, buffer);
		n = stoi(buffer);
		buffer.clear();
		getline(inputFile, buffer);
		p = stoi(buffer);
		buffer.clear();
        
        // Map of the nursery
        char** nursery = new char*[n];
        for(int i =0; i < n; i++){
            nursery[i] = new char[n];
            memset(nursery[i], '\0', n*sizeof(char));
        }
        for(int i = 0;i < n; i++){
            getline(inputFile, buffer);
            for (int j = 0; j < n; j++) {
                nursery[i][j] = buffer[j];
            }
        }
//        BFS(nursery, p);
        node temp;
        temp.UpdateState(n, nursery);
        DFS(temp, p, 1);
	}
	else {
		perror ("Error opening the file.");
	}

    return 0;
}


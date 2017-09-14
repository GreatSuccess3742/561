// 561hw1.cpp : Defines the entry point for the console application.
//

#include<iostream>
#include<fstream>
#include<stdio.h>
#include<string>
#include<vector>
#include<queue>
#include<unordered_map>
#include<utility>

using namespace std;

// Global Variables

// Width & height of the square nursery
int n = 0;

// Number of lizzards
int p = 0;

//typedef unordered_map <vector<int>, int> stateTable;

class node {
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
    void delete_mm(){
        if(state!=NULL) for(int i=0;i<n;i++) if(state[i]!=NULL)delete [] state[i];
        state= NULL;
    }
};

void PrintResult(char ** state,int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout<<state[i][j];
        }
        cout<<endl;
    }
}

//bool CheckRedundancy(int n, char** state, stateTable myStateTable){
//    vector<int> myVector;
//    for(int i = 0; i < n; i++){
//        for(int j = 0; j < n; j++){
//            if(state[i][j] == '1'){
//                myVector.push_back(n*i+j);
//            }
//        }
//    }
//    //    pair<stateTable::iterator, bool> result = myStateTable.insert(make_pair(myVector, 1));
//    //    if (result.second)
//    //    {
//    //        // the insertion worked and result.first points to the newly
//    //        // inserted pair
//    //        return true;
//    //    }
//    //    else
//    //    {
//    //        // the insertion failed and result.first points to the pair that
//    //        // was already in the map
//    //        return false;
//    //    }
//    if(myStateTable.insert({myVector , 1}).second)
//        return true;
//    else
//        return false;
//}

void BFS(char** state, int numOfLizzard){
    queue<node> myQueue;
    int numOfInsertedLizzard = 0;
    
    // Initialize a new node:
    node newNode;
    newNode.UpdateState(n, state);
    myQueue.push(newNode);
    
    
    while(!myQueue.empty()){
        unsigned long length = myQueue.size();
        int rStart = 0;
        int cStart = 0;
        for(int i = 0; i < length; i++){
            // Insert every child of the front node to the queue:
    
            node& firstNode= myQueue.front();
            bool myFlag = false;
            for(int r = n-1; r >= 0; r --){
                for(int c = n-1; c >= 0 ; c--){
                    if(myQueue.front().state[r][c] == '1'){
                        rStart = r;
                        cStart = c;
                        myFlag = true;
                        break;
                    }
                }
                if(myFlag == true)
                    break;
            }
            bool firstTime = true;
            for(int x= rStart; x < n ; x++){
                for(int y = 0; y < n; y++){
                    if(firstTime == true) {
                        y = cStart;
                        firstTime = false;
                    }
                    
                    if(firstNode.CheckLizzardSafety(x, y)){
                        // Passing the check, insert the node into the queue
                        node temp;
                        char ** tempState = new char*[n];
                        for(int i = 0; i < n; i++){
                            tempState[i] = new char[n];
                            memset(tempState[i], '\0', sizeof(char));
                        }
                        for(int i = 0; i < n; i++){
                            for(int j = 0; j < n; j++){
                                tempState[i][j] = firstNode.state[i][j];
                            }
                        }
                        temp.UpdateState(n, tempState);
                        temp.state[x][y] = '1';
                        
                        cout<<endl;
                        myQueue.push(temp);
                    }
                }
            }

            // Pop out the front node
            myQueue.pop();
        }
        
        // to the next level of the tree, number of lizzard increased.
        numOfInsertedLizzard++;
        
        if(numOfInsertedLizzard >= numOfLizzard && (!myQueue.empty()) ){
            // Output result
//            cout<<"myq.size = "<<myQueue.size()<<endl;
            cout<<"OK"<<endl;
            PrintResult(myQueue.front().state,n);
            break;
        }
        //if FAIL
        if(numOfInsertedLizzard >= numOfLizzard && (myQueue.empty()) ){
            cout<<"FAIL"<<endl;
            break;
        }
        
        
        length = myQueue.size();
    }
}
bool DFS(node currentNode, int numOfLizzard, int numOfInsertedLizzard){
//    cout<<"numOfInsertedLizzard = "<< numOfInsertedLizzard << endl;
    if(numOfInsertedLizzard >= numOfLizzard){
        cout<<"OK"<<endl;
        PrintResult(currentNode.state, n);
        exit(0);
        return true;
    }
    else{
        
        int rStart = 0;
        int cStart = 0;
        bool myFlag = false;
        
        for(int i = n-1 ; i >= 0; i-- ){
            for(int j = n-1 ; j >= 0; j--){
                if(currentNode.state[i][j] == '1'){
                    rStart = i;
                    cStart = j;
                    myFlag = true;
                    break;
                }
            }
            if(myFlag == true)
                break;
        }
        bool firstTime = true;
        
        for(int x = rStart ; x < n; x++){
            for(int y = 0; y < n; y++){
                if(firstTime){
                    y = cStart;
                    firstTime = false;
                }
                if(currentNode.CheckLizzardSafety(x, y)){
                    node newNode;
                    char ** tempState = new char*[n];
                    for(int i = 0; i < n; i++){
                        tempState[i] = new char[n];
                        memset(tempState[i],'\0',sizeof(char));
                    }
                    for(int i = 0; i < n; i++){
                        for(int j = 0; j < n; j++){
                            tempState[i][j] = currentNode.state[i][j];
                        }
                    }
                    newNode.UpdateState(n, tempState);
                    newNode.state[x][y] = '1';
                    if (DFS(newNode, numOfLizzard, numOfInsertedLizzard + 1)){
                        return true;
                    }
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
            memset(nursery[i], '\0', sizeof(char));
        }
        for(int i = 0;i < n; i++){
            getline(inputFile, buffer);
            for (int j = 0; j < n; j++) {
                nursery[i][j] = buffer[j];
            }
        }
        cout<<"input = "<<endl;
        PrintResult(nursery, n);
        // BFS
        if(algorithms == "BFS"){
            BFS(nursery, p);
        }
        
        // DFS
        else if(algorithms == "DFS"){
            node temp;
            temp.UpdateState(n, nursery);
            if(!DFS(temp, p, 0))
                cout<<"FAIL"<<endl;
        }
        
        //SA
        else if(algorithms == "SA"){
            //do SA stuff
        }
        else{
            cout<<"plz specify BFS, DFS or SA."<<endl;
        }
	}
	else {
		perror ("Error opening the file.");
	}

    return 0;
}


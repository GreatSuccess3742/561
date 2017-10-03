//
//  homework11.cpp
//  csci561 - hw2
//
//  Created by Eric Hsieh on 2017/9/30.
//  Copyright © 2017年 Eric Hsieh. All rights reserved.
//

#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<utility>
#include<time.h>
#include<math.h>
#include<queue>

using namespace std;

class Coordinate{
    public:
        int x;
        int y;
};
char ** Create2DArray(int n){
    char** myArray = new char*[n];
    for(int i = 0; i < n; i++){
        myArray[i] = new char[n];
        memset(myArray[i],'?',n * sizeof(char));
    }
    return myArray;
}

void Delete2DArray(int n, char** myArray){
    for(int i = 0; i < n; i++){
        delete[] myArray[i];
    }
    delete[] myArray;
}

void Reset2DArray(int n, char** myArray){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            myArray[i][j] = '\0';
        }
    }
}
void PrintBoard(char** board, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout<<board[i][j];
        }
        cout<<endl;
    }
}
bool CompleteDecisionMap(char** decisionMap, int n){
    for(int i =0 ; i < n; i++){
        for(int j = 0; j < n; j++){
            if(decisionMap[i][j] == '?')
                return false;
        }
    }
    return true;
}
bool CheckConverge(char** cur, char** prev, int n){
    for(int i = 0; i < n ; i++){
        for(int j = 0; j < n; j++){
            if(cur[i][j] != prev[i][j])
                return false;
        }
    }
    return true;
}

Coordinate NewRoot(char** decisionMap, int n){
    Coordinate root;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(decisionMap[i][j] == '?'){
                root.x = i;
                root.y = j;
                return root;
            }
        }
    }
    // decesion map is complete
    root.x = -1;
    root.y = -1;
    return root;
}

char** CheckConnectivity(char** board,int n){
    char** decisionMap = Create2DArray(n);
    char** previousDecisionMap = Create2DArray(n);
    
    queue<Coordinate> myQueue;
    Coordinate temp;
    char decision = '0';
    
    // Mark * on the decision map
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(board[i][j] == '*')
                decisionMap[i][j] = board[i][j];
        }
    }
    
    Reset2DArray(n, previousDecisionMap);
    while(!CompleteDecisionMap(decisionMap, n)){
        Coordinate root;
        root = NewRoot(decisionMap, n);
        myQueue.push(root);
        decisionMap[root.x][root.y] = decision;
        while(!CheckConverge(decisionMap, previousDecisionMap, n)){
            
            root.x = myQueue.front().x;
            root.y = myQueue.front().y;
            cout<<"rootx = "<<root.x<<endl;
            cout<<"rooty = "<<root.y<<endl;
            
            // Update previousDecision Map
            for(int i = 0; i < n; i++){
                for(int j = 0; j < n; j++){
                    previousDecisionMap[i][j] = decisionMap[i][j];
                }
            }
            
            if(decisionMap[root.x][root.y] == '*')
                continue;
            
            // Check neighbor's value
            
            // Down
            if((root.x+1) < n){
                if(board[root.x+1][root.y] == board[root.x][root.y]){
                    decisionMap[root.x+1][root.y] = decision;
                    temp.x = root.x+1;
                    temp.y = root.y;
                    myQueue.push(temp);
                }
            }
        
            // Up
            if((root.x-1) >= 0){
                if(board[root.x-1][root.y] == board[root.x][root.y]){
                    decisionMap[root.x-1][root.y] = decision;
                    temp.x = root.x-1;
                    temp.y = root.y;
                    myQueue.push(temp);
                }
            }
            
            // Right
            if((root.y+1) < n){
                if(board[root.x][root.y+1] == board[root.x][root.y]){
                    decisionMap[root.x][root.y+1] = decision;
                    temp.x = root.x;
                    temp.y = root.y+1;
                    myQueue.push(temp);
                }
            }
            // Left
            if((root.y-1) >= 0){
                if(board[root.x][root.y-1] == board[root.x][root.y]){
                    decisionMap[root.x][root.y-1] = decision;
                    temp.x = root.x;
                    temp.y = root.y-1;
                    myQueue.push(temp);
                }
                
            }
            
            PrintBoard(decisionMap,n);
            myQueue.pop();
            cout<<"root.x = "<<root.x<<endl;
            cout<<"root.y = "<<root.y<<endl;
        }
        decision++;
    }
    return decisionMap;
}


int main(int argc, const char * argv[]) {
    // insert code here...
    string buffer;
    
    fstream inputFile;
    fstream outputFile;
    
    int n = 0;
    int p = 0;
    float timeRemained = 0;
    
//    inputFile.open("input.txt", fstream::in);
    inputFile.open("/Users/erichsieh/GoogleDrive/561/hw/561/hw2/hw2/hw2/input.txt", fstream::in);
    outputFile.open("output.txt", fstream::out);
    
    if (!outputFile) {
        cout << "can't open outputFile." << endl;
    }
    
    if (inputFile.is_open()) {
        getline(inputFile, buffer);
        n = stoi(buffer);
        buffer.clear();
        getline(inputFile, buffer);
        p = stoi(buffer);
        buffer.clear();
        getline(inputFile, buffer);
        timeRemained = stof(buffer);
        buffer.clear();
        
        // Declare board
        char** board = new char*[n];
        
        for (int i = 0; i < n; i++) {
            board[i] = new char[n];
            memset(board[i], '\0', n * sizeof(char));
        }
        for (int i = 0; i < n; i++) {
            getline(inputFile, buffer);
            for (int j = 0; j < n; j++) {
                board[i][j] = buffer[j];
            }
            buffer.clear();
        }
        CheckConnectivity(board, n);
    }
    else{
        cout<<"failed to open input file"<<endl;
    }
    
    
    
    inputFile.close();
    outputFile.close();
    return 0;
}

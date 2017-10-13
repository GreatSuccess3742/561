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
#include<set>
#include<map>
#include<string> // for VS2017

using namespace std;

int maxDepth = 0;
int tempGlobalCount = 0;

class MaxValue {
public:
    int max;
    int index;
};

class MinValue {
public:
    int min;
    int index;
};

class TreeNode {
public:
    int depth;
    int alpha;
    int beta;
    bool MaxOrNot;
    TreeNode *parent;
    vector<TreeNode> child;
    char** board;
    int** decisionMap;
    int decision;
    // evaluation value = my score - opponent's score
    int evaluationValue;
    bool TerminalNode(int** decisionMap, int n) {
        set<char> mySet;
        for (int i = 0; i<n; i++) {
            for (int j = 0; j < n; j++) {
                if (decisionMap[i][j] != -2) {
                    mySet.insert(decisionMap[i][j]);
                    if (mySet.size() > 0)
                        return false;
                }
            }
        }
        return true;
    }
    // Constructor
    TreeNode();
    TreeNode(int, int, int, bool, char**, int**, int, int);
};

TreeNode::TreeNode() {
    
}

TreeNode::TreeNode(int myDepth, int myAlpha, int myBeta, bool myMaxOrNot, char** myBoard, int** myDecisionMap, int myDecision, int myEvaluationValue) {
    depth = myDepth;
    alpha = myAlpha;
    beta = myBeta;
    MaxOrNot = myMaxOrNot;
    board = myBoard;
    decisionMap = myDecisionMap;
    decision = myDecision;
    evaluationValue = myEvaluationValue;
}
class Coordinate {
public:
    int x;
    int y;
};

class ReturnInfo {
public:
    int alpha;
    int beta;
    int decision;
    int evaluationValue;
    bool winLose;
};
char ** Create2DArray(int n) {
    char** myArray = new char*[n];
    for (int i = 0; i < n; i++) {
        myArray[i] = new char[n];
        memset(myArray[i], '?', n * sizeof(char));
    }
    return myArray;
}

void Delete2DArray(int n, char** myArray) {
    for (int i = 0; i < n; i++) {
        delete[] myArray[i];
    }
    delete[] myArray;
}

int ** Create2DIntArray(int n) {
    int** myArray = new int*[n];
    for (int i = 0; i < n; i++) {
        myArray[i] = new int[n];
        memset(myArray[i], -1, n * sizeof(int));
    }
    return myArray;
}

void Delete2DIntArray(int n, int** myArray) {
    for (int i = 0; i < n; i++) {
        delete[] myArray[i];
    }
    delete[] myArray;
}

void Reset2DArray(int n, int** myArray) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            myArray[i][j] = '\0';
        }
    }
}
void PrintBoard(char** board, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << board[i][j];
        }
        cout << endl;
    }
}

void CopyBoard(char** dstBoard, char** srcBoard, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dstBoard[i][j] = srcBoard[i][j];
        }
    }
}

void PrintDecisionMap(int** decisionMap, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << decisionMap[i][j];
        }
        cout << endl;
    }
}
bool CompleteDecisionMap(int** decisionMap, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (decisionMap[i][j] == -1)
                return false;
        }
    }
    return true;
}
bool CheckConverge(int** cur, int** prev, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (cur[i][j] != prev[i][j])
                return false;
        }
    }
    return true;
}

Coordinate NewRoot(int** decisionMap, int n) {
    Coordinate root;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (decisionMap[i][j] == -1) {
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

int** CheckConnectivity(char** board, int n) {
    // Check connectivity of a board, returns a decision map
    // Decision map will indicate how many choices remains
    // -1 means unchecked coordinates, -2 means '*' on the board
    
    int** decisionMap = Create2DIntArray(n);
    int** previousDecisionMap = Create2DIntArray(n);
    
    queue<Coordinate> myQueue;
    int decision = 0;
    
    // Mark * on the decision map
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == '*')
                decisionMap[i][j] = -2;
        }
    }
    
    Reset2DArray(n, previousDecisionMap);
    while (!CompleteDecisionMap(decisionMap, n)) {
        Coordinate root;
        Coordinate temp;
        root = NewRoot(decisionMap, n);
        myQueue.push(root);
        decisionMap[root.x][root.y] = decision;
        while (! myQueue.empty() ) {
            root.x = myQueue.front().x;
            root.y = myQueue.front().y;
            
            myQueue.pop();
            // Update previousDecision Map
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    previousDecisionMap[i][j] = decisionMap[i][j];
                }
            }
            
            if (decisionMap[root.x][root.y] == -2)
                continue;
            
            // Check neighbor's value
            
            // Down
            if ((root.x + 1) < n) {
                if (board[root.x + 1][root.y] == board[root.x][root.y] && decisionMap[root.x + 1][root.y] == -1) {
                    decisionMap[root.x + 1][root.y] = decision;
                    temp.x = root.x + 1;
                    temp.y = root.y;
                    myQueue.push(temp);
                }
            }
            
            // Up
            if ((root.x - 1) >= 0) {
                if (board[root.x - 1][root.y] == board[root.x][root.y] && decisionMap[root.x - 1][root.y] == -1) {
                    decisionMap[root.x - 1][root.y] = decision;
                    temp.x = root.x - 1;
                    temp.y = root.y;
                    myQueue.push(temp);
                }
            }
            
            // Right
            if ((root.y + 1) < n) {
                if (board[root.x][root.y + 1] == board[root.x][root.y] && decisionMap[root.x][root.y + 1] == -1) {
                    decisionMap[root.x][root.y + 1] = decision;
                    temp.x = root.x;
                    temp.y = root.y + 1;
                    myQueue.push(temp);
                }
            }
            // Left
            if ((root.y - 1) >= 0) {
                if (board[root.x][root.y - 1] == board[root.x][root.y] && decisionMap[root.x][root.y - 1] == -1) {
                    decisionMap[root.x][root.y - 1] = decision;
                    temp.x = root.x;
                    temp.y = root.y - 1;
                    myQueue.push(temp);
                }
                
            }
        }
        decision++;
    }
    return decisionMap;
}

void ApplyGravity(char** board, int n) {
    // Fetching column information
    
    for (int colIndex = 0; colIndex < n; colIndex++) {
        int emptyGridCount = 0;
        queue<char> remainingFruit;
        
        for (int rowIndex = 0; rowIndex < n; rowIndex++) {
            if (board[rowIndex][colIndex] == '*') {
                emptyGridCount++;
            }
            // Storing all fruits in remainingFruit
            else {
                PrintBoard(board, n);
                cout<<board[rowIndex][colIndex]<<endl;
                remainingFruit.push(board[rowIndex][colIndex]);
            }
        }
        
        // Place all empty space to the top and put all fruits to the bottom
        for (int i = 0; i < n; i++) {
            if (emptyGridCount > 0) {
                emptyGridCount--;
                board[i][colIndex] = '*';
            }
            else {
                board[i][colIndex] = remainingFruit.front();
                remainingFruit.pop();
            }
        }
    }
}
void PrintNodeInfo(ReturnInfo nodeInfo){
    cout<<"--------------- Node Info: ---------------- "<<endl;
    cout<<"alpha = "<<nodeInfo.alpha<<endl;
    cout<<"beta = "<<nodeInfo.beta<<endl;
    cout<<"decision = "<<nodeInfo.decision<<endl;
    cout<<"evaluationValue = "<<nodeInfo.evaluationValue<<endl;
    cout<<"Win or Lose = "<<nodeInfo.winLose<<endl;
    cout<<endl;
}


ReturnInfo AlphaBeta(TreeNode currentNode, int depth, int alpha, int beta, bool maximizingPlayer, int decision, int n) {
    ReturnInfo nodeInfo;
    nodeInfo.alpha = currentNode.alpha;
    nodeInfo.beta = currentNode.beta;
    nodeInfo.decision = currentNode.decision;
    nodeInfo.evaluationValue = currentNode.evaluationValue;
    nodeInfo.winLose = false;
    
//    PrintNodeInfo(nodeInfo);
//    cout<<"board = "<<endl;
//    PrintBoard(currentNode.board, n);
//    
//    cout<<"decisionmap = "<<endl;
//    PrintDecisionMap(currentNode.decisionMap, n);
    
    // Reaches the bottom of the tree, where we can determine win or lose
    if (currentNode.TerminalNode(currentNode.decisionMap, n)) {
        cout<<"reach terminal node"<<endl;
        // Return the heuristic value of node
        if (currentNode.evaluationValue > 0){
            cout<<"win"<<endl;
            if(currentNode.MaxOrNot == true){
                nodeInfo.alpha = currentNode.evaluationValue;
                cout<<"win nodeInfo.alpha = "<<nodeInfo.alpha<<endl;
            }
            else{
                nodeInfo.beta = currentNode.evaluationValue;
                cout<<"win nodeInfo.beta"<<nodeInfo.beta<<endl;
            }
            
            nodeInfo.winLose = true;
            return nodeInfo;
        }
        else{
            cout<<"lose"<<endl;
            if(currentNode.MaxOrNot)
                nodeInfo.alpha = currentNode.evaluationValue;
            else
                nodeInfo.beta = currentNode.evaluationValue;
            return nodeInfo;
        }
    }
    // reaches the max depth, return current evaluationValue
    if (currentNode.depth >= maxDepth) {
        cout<<"reaches max depth"<<endl;
        return nodeInfo;
    }
    
    
    
    // Max node
    if (maximizingPlayer) {
        // constructing all the child node of current max node
        set<int> choiceCounter;
        
        // found out how many choices on the board
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (currentNode.decisionMap[i][j] >= 0) {
                    choiceCounter.insert(currentNode.decisionMap[i][j]);
                }
            }
        }
        // Push all possible state into parent node
        for (int i = 0; i < choiceCounter.size(); i++) {
            
            
            char ** tempBoard = Create2DArray(n);
            CopyBoard(tempBoard, currentNode.board, n);
            
            int ** tempDecisionMap = CheckConnectivity(currentNode.board, n);

            TreeNode choices(currentNode.depth + 1, currentNode.alpha, currentNode.beta, false, tempBoard, tempDecisionMap, -1, currentNode.evaluationValue);

            currentNode.child.push_back(choices);
        }
        
        // find the size of every decision
        int* connectivityTable = new int[choiceCounter.size()];
        memset(connectivityTable, 0, choiceCounter.size() * sizeof(int));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (currentNode.decisionMap[i][j] >= 0)
                    connectivityTable[currentNode.decisionMap[i][j]]++;
            }
        }
        
        // Sort the decsion in descending order
        MaxValue myMax;
        myMax.index = -1;
        myMax.max = -1;
        
        vector<int> orderOfDecision;
        for (int i = 0; i < choiceCounter.size(); i++) {
            for (int j = 0; j < choiceCounter.size(); j++) {
                if (connectivityTable[j] > myMax.max) {
                    myMax.max = connectivityTable[j];
                    myMax.index = j;
                }
            }
            orderOfDecision.push_back(myMax.index);
            connectivityTable[myMax.index] = -1;
            myMax.index = -1;
            myMax.max = -1;
        }
        
        // Choose the largest connectivity option according to the orderOfDecision
        int eliminateCount = 0;
        int descisionCount = 0;
        descisionCount = orderOfDecision.size();
        for (int i = 0; i < descisionCount; i++) {
            
            // update decision of the child node
            currentNode.child[i].decision = orderOfDecision[i];
            
            for (int rowIndex = 0; rowIndex < n; rowIndex++) {
                for (int colIndex = 0; colIndex < n; colIndex++) {
                    if (currentNode.child[i].decisionMap[rowIndex][colIndex] == orderOfDecision[i]) {
                        currentNode.child[i].board[rowIndex][colIndex] = '*';
                        eliminateCount++;
                    }
                }
            }
            int score = pow(eliminateCount, 2);
            
            // MaxNode: Calculating the evaluation value after elimination
            
            currentNode.child[i].evaluationValue = currentNode.evaluationValue + score;
            eliminateCount = 0;
            
            ApplyGravity(currentNode.child[i].board, n);
            currentNode.child[i].decisionMap = CheckConnectivity(currentNode.child[i].board, n);
            
        }
        
        // A-B pruning
        for (int i = 0; i < currentNode.child.size(); i++) {
            ReturnInfo tempInfo;
            
            
            cout<<"123"<<endl;
            
            tempInfo = AlphaBeta(currentNode.child[i], currentNode.child[i].depth + 1, currentNode.child[i].alpha, currentNode.child[i].beta, currentNode.child[i].MaxOrNot, currentNode.child[i].decision, n);
            
            
            
            int previousAlpha = currentNode.alpha;
            cout<<"currentNode.alpha = "<<currentNode.alpha<<" tempInfo.evaluationValue = "<<tempInfo.evaluationValue<<endl;
            currentNode.alpha = max(currentNode.alpha, tempInfo.beta);
            // Check whether currentNode.alpha had been updated or not.
            // If updated, node.decision should also be updated.
            if(previousAlpha!=currentNode.alpha){
                nodeInfo.decision = tempInfo.decision;
                nodeInfo.winLose = tempInfo.winLose;
            }
            if (currentNode.beta <= currentNode.alpha){
                cout<<"Max node prune."<<endl;
                break;
            }
            
        }
        
        
        nodeInfo.alpha = currentNode.alpha;

        cout<<"Max Node info"<<endl;
        PrintNodeInfo(nodeInfo);
        return nodeInfo;
    }
    
    // Min node
    else {
        // constructing all the child node of current max node
        set<int> choiceCounter;
        
        // found out how many choices on the board
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (currentNode.decisionMap[i][j] >= 0) {
                    choiceCounter.insert(currentNode.decisionMap[i][j]);
                }
            }
        }
        
        // Push all posible state into parent node
        for (int i = 0; i < choiceCounter.size(); i++) {
            char ** tempBoard = Create2DArray(n);
            CopyBoard(tempBoard, currentNode.board, n);
            int ** tempDecisionMap = CheckConnectivity(currentNode.board, n);
            TreeNode choices(currentNode.depth + 1, INT_MIN, INT_MAX, true, tempBoard, tempDecisionMap, -1, currentNode.evaluationValue);
            currentNode.child.push_back(choices);
        }
        
        // find the size of every decision
        int* connectivityTable = new int[choiceCounter.size()];
        memset(connectivityTable, 0, choiceCounter.size() * sizeof(int));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (currentNode.decisionMap[i][j] >= 0)
                    connectivityTable[currentNode.decisionMap[i][j]]++;
            }
        }
        
        // Sort the decsion in ascending order
        MinValue myMin;
        myMin.index = INT_MAX;
        myMin.min = INT_MAX;
        
        vector<int> orderOfDecision;
        for (int i = 0; i < choiceCounter.size(); i++) {
            for (int j = 0; j < choiceCounter.size(); j++) {
                if (connectivityTable[j] < myMin.min) {
                    myMin.min = connectivityTable[j];
                    myMin.index = j;
                }
            }
            orderOfDecision.push_back(myMin.index);
            connectivityTable[myMin.index] = -1;
            myMin.index = -1;
            myMin.min = -1;
        }
        
        // Choose the smallest connectivity option according to the orderOfDecision
        int eliminateCount = 0;
        int descisionCount = 0;
        descisionCount = orderOfDecision.size();
        for (int i = 0; i < descisionCount; i++) {
            currentNode.child[i].decision = orderOfDecision[i];
            for (int rowIndex = 0; rowIndex < n; rowIndex++) {
                for (int colIndex = 0; colIndex < n; colIndex++) {
                    if (currentNode.child[i].decisionMap[rowIndex][colIndex] == orderOfDecision[i]) {
                        currentNode.child[i].board[rowIndex][colIndex] = '*';
                        eliminateCount++;
                    }
                }
            }
            int score = pow(eliminateCount, 2);
            
            // MinNode: Calculating the evaluation value after elimination
            
            currentNode.child[i].evaluationValue = currentNode.evaluationValue - score;
            score = 0;
            ApplyGravity(currentNode.child[i].board, n);
            currentNode.child[i].decisionMap = CheckConnectivity(currentNode.child[i].board, n);
            
            /*cout << "updated decision map:" << endl;
             PrintDecisionMap(currentNode.child[i].decisionMap, n);*/
        }
        // A-B pruning
        for (int i = 0; i < currentNode.child.size(); i++) {
            ReturnInfo tempInfo;
            tempInfo = AlphaBeta(currentNode.child[i], currentNode.child[i].depth + 1, currentNode.child[i].alpha, currentNode.child[i].beta, currentNode.child[i].MaxOrNot, currentNode.child[i].decision, n);
            
            int previousBeta = currentNode.beta;
            currentNode.beta = min(currentNode.beta, tempInfo.alpha);
            if(previousBeta != currentNode.beta){
                nodeInfo.decision = tempInfo.decision;
                nodeInfo.winLose = tempInfo.winLose;
            }
            if (currentNode.beta <= currentNode.alpha){
                cout<<"Min node prune."<<endl;
                break;
            }
        }
        nodeInfo.beta = currentNode.beta;
        cout<<"Min Node"<<endl;
        PrintNodeInfo(nodeInfo);
        return nodeInfo;
    }
    
    
}

int main(int argc, const char * argv[]) {
    // insert code here...
    string buffer;
    
    fstream inputFile;
    fstream outputFile;
    
    int n = 0;
    int p = 0;
    float timeRemained = 0;
    
    //inputFile.open("input.txt", fstream::in);
    
    // OSX file path
    inputFile.open("/Users/erichsieh/GoogleDrive/561/hw/561/hw2/hw2/hw2/input.txt", fstream::in);
    
    // Windows file path
    // inputFile.open("F:\\Google Drive\\561\\hw\\561\\hw2\\hw2\\hw2\\input.txt", fstream::in);
    
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
        
        // Max depth's value and remaining time should be in inverse proportion
        maxDepth = 5;
        
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
        
        char ** tempBoard = Create2DArray(n);
        CopyBoard(tempBoard, board, n);
        int ** tempDecisionMap = CheckConnectivity(board, n);
        
        TreeNode root(0, INT_MIN, INT_MAX, true, tempBoard, tempDecisionMap, -1, 0);
        
        ReturnInfo ans;
        ans = AlphaBeta(root, 0, INT_MIN, INT_MAX, true, -1, n);
        cout<<"-----ans-----"<<endl;
        PrintNodeInfo(ans);
    }
    else {
        cout << "failed to open input file" << endl;
    }
    
    
    
    inputFile.close();
    outputFile.close();
    
    //	system("pause");
    system("read -n 1 -s -p \"Press any key to continue...\"");
    return 0;
}

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
#include<limits.h> // INT_MAX and INT_MIN

using namespace std;

int maxDepth = 0;
int tempGlobalCount = 0;
// Time
time_t startTime;
time_t currentTime;
double timeUsed = 0;
double timeLimit = 0;
float timeRemained = 0;
char** ansBoard;


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
    vector<TreeNode*> child;
    char** board;
    int** decisionMap;
    int decision; // Best Decision
    int score;
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

class Answer{
public:
    int col;
    int row;
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


void PrintBoardToFile(fstream& outputFile, char** board, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            outputFile << board[i][j];
        }
        outputFile << endl;
    }
}

void EliminateFruit(char** board, int** decisionMap,int decision, int n){
    // update decision of the child node
//    currentNode->child[i]->decision = orderOfDecision[i];
    for (int rowIndex = 0; rowIndex < n; rowIndex++) {
        for (int colIndex = 0; colIndex < n; colIndex++) {
            if (decisionMap[rowIndex][colIndex] == decision) {
                board[rowIndex][colIndex] = '*';
            }
        }
    }
    ApplyGravity(board, n);
}


TreeNode* MaxNodeLevel(TreeNode *myNode, int n);
TreeNode* MinNodeLevel(TreeNode *myNode, int n);
Answer FindAnswer(int** decisionMap, int decision, int n);
void CopyBoard(char** dstBoard, char** srcBoard, int n);
int** CheckConnectivity(char** board, int n);
vector<int> FindOrderOfDecision(TreeNode* currentNode, int n);
void BranchingCurrentState(TreeNode* currentNode, vector<int> orderOfDecision, int n);
int AlphaBetaPruning(char** board, int n, int evaluationValue, int depth, bool MaxOrNot, int alpha, int beta);
bool TerminalState(char** board, int n);
TreeNode* EatFruit(char** board, int ** decisionMap, int n);

int main(int argc, const char * argv[]) {
    // insert code here...
    string buffer;
    
    fstream inputFile;
    fstream outputFile;
    
    int n = 0;
    int p = 0;
    
    
    //inputFile.open("input.txt", fstream::in);
    
    // OSX file path
    inputFile.open("/Users/erichsieh/GoogleDrive/561/hw/561/hw2/hw2/hw2/input.txt", fstream::in);
    
    // Windows file path
    //inputFile.open("F:\\Google Drive\\561\\hw\\561\\hw2\\hw2\\hw2\\input.txt", fstream::in);
    
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
        
//        timeLimit = timeRemained/2.0;
//        if(timeRemained < 2){
//            maxDepth = 1;
//            cout<<"depth = 1"<<endl;
//        }
//        else{
//            maxDepth = 5;
//        }
        maxDepth = 3;
        
        
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
        
        ansBoard = Create2DArray(n);
        CopyBoard(ansBoard, board, n);
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                ansBoard[i][j] = board[i][j];
            }
        }
        
        AlphaBetaPruning(tempBoard, n, 0, maxDepth, true, INT_MIN, INT_MAX);
        cout<<"ans board = "<<endl;
        PrintBoard(ansBoard, n);
        exit(0);
//        TreeNode *root = new TreeNode(0, INT_MIN, INT_MAX, true, tempBoard, tempDecisionMap, -1, 0);
//        root = MaxNodeLevel(root, n);
//        
//        cout<<"root->alpha = "<<root->alpha<<endl;
//        cout<<"root->beta = "<<root->beta<<endl;
//        cout<<"root->evaluationValue = "<<root->evaluationValue<<endl;
//        cout<<"root->decision = "<<root->decision<<endl;
//        cout<<"root decisionmap"<<endl;
//        PrintDecisionMap(root->decisionMap, n);
        
        

        Answer ans;
//        ans = FindAnswer(tempDecisionMap, root->decision, n);
//        EliminateFruit(tempBoard, tempDecisionMap, root->decision, n);
        
        char col = 'A';
        col = col + ans.col;
        outputFile<<col;
        outputFile<<ans.row+1<<endl;
        
        
        PrintBoardToFile(outputFile, tempBoard, n);
        
        
    }
    else {
        cout << "failed to open input file" << endl;
    }
    
    inputFile.close();
    outputFile.close();
    
    //	system("pause");
    
    return 0;
}

Answer FindAnswer(int** decisionMap, int decision, int n){
    Answer myAns;
    if(decision == -1)
        decision = 0;
    myAns.row = n-1;
    myAns.col = n-1;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(decisionMap[i][j] == decision){
                myAns.row = i;
                myAns.col = j;
                return myAns;
            }
        }
    }
    return myAns;
}

void CopyBoard(char** dstBoard, char** srcBoard, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dstBoard[i][j] = srcBoard[i][j];
        }
    }
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

vector<int> FindOrderOfDecision(TreeNode* currentNode, int n){
    // constructing all the child node of current max node
    set<int> choiceCounter;
    
    // found out how many choices on the board
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (currentNode->decisionMap[i][j] >= 0) {
                choiceCounter.insert(currentNode->decisionMap[i][j]);
            }
        }
    }
    // Push all possible state into parent node
    for (int i = 0; i < choiceCounter.size(); i++) {
        
        
        char ** tempBoard = Create2DArray(n);
        CopyBoard(tempBoard, currentNode->board, n);

        
        int ** tempDecisionMap = CheckConnectivity(currentNode->board, n);
        
        TreeNode *choices;
        choices = new TreeNode(currentNode->depth + 1, currentNode->alpha, currentNode->beta, !(currentNode->MaxOrNot), tempBoard, tempDecisionMap, -1, currentNode->evaluationValue);
        
        currentNode->child.push_back(choices);
    }
    
    // find the size of every decision
    int* connectivityTable = new int[choiceCounter.size()];
    memset(connectivityTable, 0, choiceCounter.size() * sizeof(int));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (currentNode->decisionMap[i][j] >= 0)
                connectivityTable[currentNode->decisionMap[i][j]]++;
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
    
    return orderOfDecision;
}
void BranchingCurrentState(TreeNode* currentNode, vector<int> orderOfDecision, int n){
    int eliminateCount = 0;
    int decisionCount = 0;
    decisionCount = orderOfDecision.size();
    
    for (int i = 0; i < decisionCount; i++) {
        // update decision of the child node
        currentNode->child[i]->decision = orderOfDecision[i];
        for (int rowIndex = 0; rowIndex < n; rowIndex++) {
            for (int colIndex = 0; colIndex < n; colIndex++) {
                if (currentNode->child[i]->decisionMap[rowIndex][colIndex] == orderOfDecision[i]) {
                    currentNode->child[i]->board[rowIndex][colIndex] = '*';
                    eliminateCount++;
                }
            }
        }
        int score = pow(eliminateCount, 2);
        currentNode->child[i]->score = score;
        
        // Calculating the evaluation value after elimination
        
//        if(currentNode->child[i]->MaxOrNot)
//            currentNode->child[i]->evaluationValue = currentNode->evaluationValue - score;
//        else
//            currentNode->child[i]->evaluationValue = currentNode->evaluationValue + score;
        eliminateCount = 0;
        
//        ApplyGravity(currentNode->child[i]->board, n);
        currentNode->child[i]->decisionMap = CheckConnectivity(currentNode->child[i]->board, n);
    }
}
bool TerminalState(char** board, int n) {
    set<char> mySet;
    for (int i = 0; i<n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] != '*') {
                return false;
            }
        }
    }
    return true;
}
TreeNode* EatFruit(char** board, int ** decisionMap, int n){
    int score = 0;
    
    char ** tempBoard = Create2DArray(n);
    CopyBoard(tempBoard, board, n);
    int ** tempDecisionMap = CheckConnectivity(board, n);
    
    TreeNode *tempNode = new TreeNode(0, 0, 0, false, tempBoard, tempDecisionMap, -1, 0);
    vector<int> tempOrderOfDecision;
    tempOrderOfDecision = FindOrderOfDecision(tempNode, n);
    BranchingCurrentState(tempNode, tempOrderOfDecision, n);
//    cout<<"tempNode->child.size() = "<<tempNode->child.size()<<endl;
//    for(int i = 0; i < tempNode->child.size(); i++){
//        cout<<"score = "<<tempNode->child[i]->score<<endl;
//        PrintBoard(tempNode->child[i]->board, n);
//        cout<<endl;
//    }
    
    
    
    
    return tempNode;
}

int AlphaBetaPruning(char** board, int n, int evaluationValue, int depth, bool MaxOrNot, int alpha, int beta){
    
    if(TerminalState(board, n) || depth == 0){
        return evaluationValue;
    }
    int** decisionMap = Create2DIntArray(n);
    decisionMap = CheckConnectivity(board, n);
    TreeNode *child;
    child = EatFruit(board, decisionMap, n);
    
    for(int i = 0; i < child->child.size(); i++){
        if(MaxOrNot){
            child->child[i]->evaluationValue = evaluationValue + child->child[i]->score;
        }
        else{
            child->child[i]->evaluationValue = evaluationValue - child->child[i]->score;
        }
    }
    
    
    if(MaxOrNot){
        
        int v = INT_MIN;
        for(int i = 0 ; i < child->child.size(); i++){
            cout<<"maxnod depth = "<<depth<<endl;
            cout<<"max evaluation value  = "<<child->child[i]->evaluationValue<<endl;
            PrintBoard(child->child[i]->board, n);
            cout<<endl;
            
            ApplyGravity(child->child[i]->board, n);
            int childValue = 0;
            char** tempBoard = Create2DArray(n);
            CopyBoard(tempBoard, child->child[i]->board, n);
            childValue =  AlphaBetaPruning(tempBoard, n, child->child[i]->evaluationValue, depth - 1, !(MaxOrNot), alpha, beta);
            if(childValue > v && depth == maxDepth){
                CopyBoard(ansBoard, tempBoard, n);
            }
            v = max(v,childValue);
            alpha = max(alpha, v);
            if(beta <= alpha)
                break;
        }
        return v;
    }
    else{
        int v = INT_MAX;
        for(int i = 0; i< child->child.size(); i++){
            cout<<"minNode depth = "<<depth<<endl;
            cout<<"min evaluation value  = "<<child->child[i]->evaluationValue<<endl;
            PrintBoard(child->child[i]->board, n);
            cout<<endl;
            
            ApplyGravity(child->child[i]->board, n);
            int childValue = 0;
            char** tempBoard = Create2DArray(n);
            CopyBoard(tempBoard, child->child[i]->board, n);
            childValue =  AlphaBetaPruning(tempBoard, n, child->child[i]->evaluationValue, depth - 1, !(MaxOrNot), alpha, beta);
            v = min(v,childValue);
            beta = min(beta, v);
            if(beta <= alpha)
                break;
        }
        return v;
    }
    
}

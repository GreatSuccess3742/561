// 561hw1.cpp : Defines the entry point for the console application.
//

#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<vector>
#include<queue>
#include<utility>
#include<time.h>
#include<map>
#include<math.h>
#include<string>


using namespace std;

// Global Variables

// Width & height of the square nursery
int n = 0;

// Number of lizzards
int p = 0;

// Time
time_t startTime;
time_t currentTime;
double timeUsed = 0;
double timeLimit = 270;

//File
fstream outputFile;




class node {
public:
    char** state;
    bool CheckLizzardSafety(int x, int y) {
        if (state[x][y] != '0') return false;
        for (int i = y; i >= 0; i--) {
            if (state[x][i] == '2') break;
            if (state[x][i] == '1') return false;
        }
        for (int i = y; i < n; i++) {
            if (state[x][i] == '2') break;
            if (state[x][i] == '1') return false;
        }
        for (int i = x; i >= 0; i--) {
            if (state[i][y] == '2') break;
            if (state[i][y] == '1') return false;
        }
        for (int i = x; i < n; i++) {
            if (state[i][y] == '2') break;
            if (state[i][y] == '1') return false;
        }
        for (int i = x, j = y; i >= 0 && j >= 0; i--, j--) {
            if (state[i][j] == '2') break;
            if (state[i][j] == '1') return false;
        }
        for (int i = x, j = y; i < n && j < n; i++, j++) {
            if (state[i][j] == '2') break;
            if (state[i][j] == '1') return false;
        }
        for (int i = x, j = y; i >= 0 && j < n; i--, j++) {
            if (state[i][j] == '2') break;
            if (state[i][j] == '1') return false;
        }
        for (int i = x, j = y; i < n && j >= 0; i++, j--) {
            if (state[i][j] == '2') break;
            if (state[i][j] == '1') return false;
        }
        return true;
    }
    void UpdateState(int n, char** newState) {
        state = new char*[n];
        for (int i = 0; i < n; i++) {
            state[i] = new char[n];
            memcpy(state[i], newState[i], n * sizeof(char));
        }
    }
    void delete_mm() {
        if (state != NULL) for (int i = 0; i<n; i++) if (state[i] != NULL)delete[] state[i];
        state = NULL;
    }
    
    bool CheckConflictLizzard(int x, int y) {
        if (state[x][y] != '1') {
            cout << "current location is not a lizzard" << endl;
            return true;
        }
        // left
        for (int i = y - 1; i >= 0; i--) {
            if (state[x][i] == '2') break;
            if (state[x][i] == '1') return false;
        }
        // right
        for (int i = y + 1; i < n; i++) {
            if (state[x][i] == '2') break;
            if (state[x][i] == '1') return false;
        }
        // up
        for (int i = x - 1; i >= 0; i--) {
            if (state[i][y] == '2') break;
            if (state[i][y] == '1') return false;
        }
        // down
        for (int i = x + 1; i < n; i++) {
            if (state[i][y] == '2') break;
            if (state[i][y] == '1') return false;
        }
        // left up
        for (int i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--) {
            if (state[i][j] == '2') break;
            if (state[i][j] == '1') return false;
        }
        // right down
        for (int i = x + 1, j = y + 1; i < n && j < n; i++, j++) {
            if (state[i][j] == '2') break;
            if (state[i][j] == '1') return false;
        }
        // right up
        for (int i = x - 1, j = y + 1; i >= 0 && j < n; i--, j++) {
            if (state[i][j] == '2') break;
            if (state[i][j] == '1') return false;
        }
        // left down
        for (int i = x + 1, j = y - 1; i < n && j >= 0; i++, j--) {
            if (state[i][j] == '2') break;
            if (state[i][j] == '1') return false;
        }
        return true;
    }
};

void PrintResult(char ** state, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            outputFile << state[i][j];
        }
        outputFile << endl;
    }
}

void BFS(char** state, int numOfLizzard) {
    queue<node> myQueue;
    int numOfInsertedLizzard = 0;
    
    // Initialize a new node:
    node newNode;
    newNode.UpdateState(n, state);
    myQueue.push(newNode);
    
    while (!myQueue.empty()) {
        
        // Time control
        time(&currentTime);
        timeUsed = difftime(currentTime, startTime);
        
        if (timeUsed >= timeLimit) {
            
            outputFile << "FAIL" << endl;
            break;
        }
        
        unsigned long length = myQueue.size();
        int rStart = 0;
        int cStart = 0;
        for (int i = 0; i < length; i++) {
            // Insert every child of the front node to the queue:
            
            node& firstNode = myQueue.front();
            bool myFlag = false;
            for (int r = n - 1; r >= 0; r--) {
                for (int c = n - 1; c >= 0; c--) {
                    if (myQueue.front().state[r][c] == '1') {
                        rStart = r;
                        cStart = c;
                        myFlag = true;
                        break;
                    }
                }
                if (myFlag == true)
                    break;
            }
            bool firstTime = true;
            for (int x = rStart; x < n; x++) {
                for (int y = 0; y < n; y++) {
                    if (firstTime == true) {
                        y = cStart;
                        firstTime = false;
                    }
                    
                    if (firstNode.CheckLizzardSafety(x, y)) {
                        // Passing the check, insert the node into the queue
                        node temp;
                        char ** tempState = new char*[n];
                        for (int i = 0; i < n; i++) {
                            tempState[i] = new char[n];
                            memset(tempState[i], '\0', sizeof(char));
                        }
                        for (int i = 0; i < n; i++) {
                            for (int j = 0; j < n; j++) {
                                tempState[i][j] = firstNode.state[i][j];
                            }
                        }
                        
                        temp.UpdateState(n, tempState);
                        temp.state[x][y] = '1';
                        
                        
                        myQueue.push(temp);
                        
                        for (int i = 0; i < n; i++) {
                            if (tempState[i] != NULL)
                                delete[] tempState[i];
                        }
                        if (tempState != NULL) {
                            delete[] tempState;
                            
                        }
                        
                    }
                }
            }
            
            // Pop out the front node
            for (int i = 0; i < n; i++) {
                delete[] myQueue.front().state[i];
            }
            delete[] myQueue.front().state;
            myQueue.pop();
        }
        
        // to the next level of the tree, number of lizzard increased.
        numOfInsertedLizzard++;
        
        if (numOfInsertedLizzard >= numOfLizzard && (!myQueue.empty())) {
            // Output result
            //            cout<<"myq.size = "<<myQueue.size()<<endl;
            outputFile << "OK" << endl;
            PrintResult(myQueue.front().state, n);
            break;
        }
        //if FAIL
        if (numOfInsertedLizzard >= numOfLizzard && (myQueue.empty())) {
            outputFile << "FAIL" << endl;
            break;
        }
        
        
        length = myQueue.size();
    }
}
bool DFS(node currentNode, int numOfLizzard, int numOfInsertedLizzard) {
    time(&currentTime);
    timeUsed = difftime(currentTime, startTime);
    
    if (timeUsed >= timeLimit) {
        cout << "FAIL" << endl;
        outputFile << "FAIL" << endl;
        return false;
    }
    if (numOfInsertedLizzard >= numOfLizzard) {
        cout << "OK" << endl;
        outputFile << "OK" << endl;
        PrintResult(currentNode.state, n);
        return true;
    }
    else {
        
        int rStart = 0;
        int cStart = 0;
        bool myFlag = false;
        
        for (int i = n - 1; i >= 0; i--) {
            for (int j = n - 1; j >= 0; j--) {
                if (currentNode.state[i][j] == '1') {
                    rStart = i;
                    cStart = j;
                    myFlag = true;
                    break;
                }
            }
            if (myFlag == true)
                break;
        }
        bool firstTime = true;
        
        for (int x = rStart; x < n; x++) {
            for (int y = 0; y < n; y++) {
                if (firstTime) {
                    y = cStart;
                    firstTime = false;
                }
                if (currentNode.CheckLizzardSafety(x, y)) {
                    node newNode;
                    char ** tempState = new char*[n];
                    for (int i = 0; i < n; i++) {
                        tempState[i] = new char[n];
                        memset(tempState[i], '\0', sizeof(char));
                    }
                    for (int i = 0; i < n; i++) {
                        for (int j = 0; j < n; j++) {
                            tempState[i][j] = currentNode.state[i][j];
                        }
                    }
                    newNode.UpdateState(n, tempState);
                    newNode.state[x][y] = '1';
                    
                    for (int i = 0; i < n; i++) {
                        delete[] tempState[i];
                    }
                    delete[] tempState;
                    
                    if (DFS(newNode, numOfLizzard, numOfInsertedLizzard + 1)) {
                        return true;
                    }
                }
            }
        }
        /*dfsIterationCount++;
         cout << "DFS count = " << dfsIterationCount << endl;
         PrintResult(currentNode.state, n);*/
        
        
        // free currentNode.state before return false
        for (int i = 0; i < n; i++) {
            if (currentNode.state[i] != NULL) {
                delete[] currentNode.state[i];
            }
        }
        if (currentNode.state != NULL)
            delete[] currentNode.state;
        
        return false;
    }
}
int CheckCost(node currentNode, int n) {
    int cost = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (currentNode.state[i][j] == '1' && !currentNode.CheckConflictLizzard(i, j))
                cost++;
        }
    }
    return cost;
}
void ZoneOut(int x, int y, node myNode) {
    // left
    for (int i = y - 1; i >= 0; i--) {
        if (myNode.state[x][i] == '2') break;
        if (myNode.state[x][i] == '1' || myNode.state[x][i] == '0') myNode.state[x][i] = '3';
    }
    // right
    for (int i = y + 1; i < n; i++) {
        if (myNode.state[x][i] == '2') break;
        if (myNode.state[x][i] == '1' || myNode.state[x][i] == '0') myNode.state[x][i] = '3';
    }
    // up
    for (int i = x - 1; i >= 0; i--) {
        if (myNode.state[i][y] == '2') break;
        if (myNode.state[i][y] == '1' || myNode.state[i][y] == '0') myNode.state[i][y] = '3';
    }
    // down
    for (int i = x + 1; i < n; i++) {
        if (myNode.state[i][y] == '2') break;
        if (myNode.state[i][y] == '1' || myNode.state[i][y] == '0') myNode.state[i][y] = '3';
    }
    // left up
    for (int i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--) {
        if (myNode.state[i][j] == '2') break;
        if (myNode.state[i][j] == '1' || myNode.state[i][j] == '0') myNode.state[i][j] = '3';
    }
    // right down
    for (int i = x + 1, j = y + 1; i < n && j < n; i++, j++) {
        if (myNode.state[i][j] == '2') break;
        if (myNode.state[i][j] == '1' || myNode.state[i][j] == '0') myNode.state[i][j] = '3';
    }
    // right up
    for (int i = x - 1, j = y + 1; i >= 0 && j < n; i--, j++) {
        if (myNode.state[i][j] == '2') break;
        if (myNode.state[i][j] == '1' || myNode.state[i][j] == '0') myNode.state[i][j] = '3';
    }
    // left down
    for (int i = x + 1, j = y - 1; i < n && j >= 0; i++, j--) {
        if (myNode.state[i][j] == '2') break;
        if (myNode.state[i][j] == '1' || myNode.state[i][j] == '0') myNode.state[i][j] = '3';
    }
}
void InitInsert(node myNode, int numberOfLizzard, char** originalMap) {
    char** temp = new char*[n];
    for (int i = 0; i < n; i++) {
        temp[i] = new char[n];
        memset(temp[i], '\0', sizeof(char));
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            temp[i][j] = originalMap[i][j];
            myNode.state[i][j] = temp[i][j];
        }
    }
    
    for (int i = 0; i < n; i++) {
        delete[] temp[i];
    }
    delete[] temp;
    
    for (int i = 0; i < numberOfLizzard; i++) {
        int randomRowIdx = (rand() % n);
        int randomColIdx = (rand() % n);
        while (myNode.state[randomRowIdx][randomColIdx] != '0') {
            randomRowIdx = (rand() % n);
            randomColIdx = (rand() % n);
        }
        myNode.state[randomRowIdx][randomColIdx] = '1';
    }
}
int CountLizzard(node myNode) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (myNode.state[i][j] == '1') {
                count++;
            }
        }
    }
    return count;
}

vector<int> AvailableGrid(node myNode) {
    vector<int> availableGrid;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (myNode.state[i][j] == '0') {
                availableGrid.push_back(i * n + j);
            }
        }
    }
    return availableGrid;
}

void RandomInsert(node myNode, int numOfLizzard) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (myNode.state[i][j] == '1') {
                ZoneOut(i, j, myNode);
            }
        }
    }
    int remainingLizard = numOfLizzard - CountLizzard(myNode);
    
    while (remainingLizard > 0) {
        int randomRowIdx = (rand() % n);
        int randomColIdx = (rand() % n);
        vector<int> availableGrid = AvailableGrid(myNode);
        int availableGridNum = availableGrid.size();
        
        for (int i = 0; i < availableGridNum; i++) {
            int grid = availableGrid[i];
            int r = grid / n;
            int c = grid%n;
            myNode.state[r][c] = '1';
            remainingLizard--;
            if (remainingLizard <= 0)
                break;
        }
        if (myNode.state[randomRowIdx][randomColIdx] == '3' && remainingLizard > 0) {
            myNode.state[randomRowIdx][randomColIdx] = '1';
            remainingLizard--;
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (myNode.state[i][j] == '3')
                myNode.state[i][j] = '0';
        }
    }
}

node CreateNextNode(node currentNode) {
    node nextNode;
    char ** nextState = new char*[n];
    for (int i = 0; i < n; i++) {
        nextState[i] = new char[n];
        memset(nextState[i], '0', n * sizeof(char));
    }
    nextNode.UpdateState(n, nextState);
    for (int i = 0; i<n; i++) {
        for (int j = 0; j < n; j++) {
            nextState[i][j] = currentNode.state[i][j];
        }
    }
    nextNode.UpdateState(n, nextState);
    RandomInsert(nextNode, p);
    
    return nextNode;
}

node CreateTempNode(void) {
    node nextNode;
    char ** nextState = new char*[n];
    for (int i = 0; i < n; i++) {
        nextState[i] = new char[n];
        memset(nextState[i], '0', n * sizeof(char));
    }
    nextNode.UpdateState(n, nextState);
    return nextNode;
}

bool YorN(float p) {
    int n = rand() % 100;
    p *= 100;
    //  cout <<"p = "<< p << endl;
    if (n <= p)
        return true;
    else
        return false;
}


void SA(node currentNode, int numOfLizzard, char** originalMap) {
    
    node nextNode;
    
    InitInsert(currentNode, numOfLizzard, originalMap);
    
    map< vector<vector<int> >, int> record;
    
    int currentE = 0;
    int nextE = 0;
    
    // Temperature
    float T = 100;
    
    // Suffle Count
    int shuffleCount = 0;
    
    // Time
    time_t startTime;
    time_t currentTime;
    double timeUsed = 0;
    
    time(&startTime);
    
    
    
    while (true)
    {
        time(&currentTime);
        timeUsed = difftime(currentTime, startTime);
        
        if (timeUsed >= timeLimit) {
            outputFile << "FAIL" << endl;
            break;
        }
        
        shuffleCount++;
        
        if (T == 0 && currentE != 0) {
            outputFile << "FAIL" << endl;
            //PrintResult(currentNode.state, n);
            break;
        }
        
        nextNode = CreateNextNode(currentNode);
        
        vector<vector<int> > nextNodeState;
        
        for (int i = 0; i < n; i++) {
            nextNodeState.push_back(vector<int>());
            for (int j = 0; j<n; j++) {
                nextNodeState[i].push_back((int)(nextNode.state[i][j] - '0'));
            }
        }
        
        map<vector<vector<int> >, int>::iterator it;
        
        it = record.find(nextNodeState);
        
        while (it != record.end()) {
            nextNode = CreateNextNode(currentNode);
            for (int i = 0; i < n; i++) {
                nextNodeState.push_back(vector<int>());
                for (int j = 0; j<n; j++) {
                    nextNodeState[i].push_back((int)(nextNode.state[i][j] - '0'));
                }
            }
            it = record.find(nextNodeState);
        }
        
        currentE = CheckCost(currentNode, n);
        nextE = CheckCost(nextNode, n);
        
        if (nextE == 0) {
            outputFile << "OK" << endl;
            PrintResult(nextNode.state, n);
            break;
        }
        
        
        
        float deltaE = (float)nextE - (float)currentE;
        
        if (deltaE < 0) {
            char** tempState = new char*[n];
            for (int i = 0; i < n; i++) {
                tempState[i] = new char[n];
                memset(tempState[i], '\0', n * sizeof(char));
            }
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    tempState[i][j] = nextNode.state[i][j];
                }
            }
            currentNode.UpdateState(n, tempState);
        }
        else {
            if (YorN(exp(-(deltaE) / T))) {
                //                cout<<"CurrentE = "<<currentE<<endl;
                //                cout<<"CurrentE = "<<currentE<<endl;
                //                cout<<"CurrentE = "<<currentE<<endl;
                char** tempState = new char*[n];
                for (int i = 0; i < n; i++) {
                    tempState[i] = new char[n];
                    memset(tempState[i], '\0', n * sizeof(char));
                }
                for (int i = 0; i < n; i++) {
                    for (int j = 0; j < n; j++) {
                        tempState[i][j] = nextNode.state[i][j];
                    }
                }
                currentNode.UpdateState(n, tempState);
            }
        }
        // Temperature decrease ratio
        T *= 0.9;
        
        // Shuffle after certain iteration:
        if (shuffleCount >= 20) {
            shuffleCount = 0;
            vector<vector<int> > failCase;
            
            //put the fail case into the record
            for (int i = 0; i < n; i++) {
                failCase.push_back(vector<int>());
                for (int j = 0; j<n; j++) {
                    failCase[i].push_back((int)(currentNode.state[i][j] - '0'));
                }
            }
            
            //            cout<<"fail case"<<endl;
            //            for(int i = 0; i <n;i++){
            //                for(int j = 0; j < n; j++){
            //                    cout<<failCase[i][j];
            //                }
            //                cout<<endl;
            //            }
            //            exit(0);
            //            cout<<"current T = "<<T<<endl;
            record[failCase] = 1;
            /*cout << record.size() << endl;
             cout << endl;*/
            InitInsert(currentNode, numOfLizzard, originalMap);
        }
    }
    
    
    
}


int main()
{
    string buffer;
    string algorithms;
    
    fstream inputFile;
    
//    inputFile.open("/Users/erichsieh/GoogleDrive/561/hw/561/hw1/input.txt", fstream::in);
    inputFile.open("input.txt", fstream::in);
    outputFile.open("output.txt", fstream::out);
    
    if (!outputFile) {
        cout << "can't open outputFile." << endl;
    }
    srand((unsigned)time(NULL));
    
    if (inputFile.is_open()) {
        getline(inputFile, algorithms);
        getline(inputFile, buffer);
        n = stoi(buffer);
        buffer.clear();
        getline(inputFile, buffer);
        p = stoi(buffer);
        buffer.clear();
        // Map of the nursery
        char** nursery = new char*[n];
        
        for (int i = 0; i < n; i++) {
            nursery[i] = new char[n];
            memset(nursery[i], '\0', n * sizeof(char));
        }
        for (int i = 0; i < n; i++) {
            getline(inputFile, buffer);
            for (int j = 0; j < n; j++) {
                nursery[i][j] = buffer[j];
            }
        }
        
        //check if the number of available grid is greater than the number of lizzards or not
        int emptyGrid = 0;
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                if(nursery[i][j] == '0')
                    emptyGrid++;
            }
        }
        
        if(p > emptyGrid){
            outputFile<<"FAIL"<<endl;
            exit(0);
        }
            
        // BFS
        if (algorithms == "BFS") {
            time(&startTime);
            BFS(nursery, p);
        }
        
        // DFS
        else if (algorithms == "DFS") {
            time(&startTime);
            node temp;
            temp.UpdateState(n, nursery);
            if (!DFS(temp, p, 0))
                outputFile << "FAIL" << endl;
        }
        
        //SA
        else if (algorithms == "SA") {
            //do SA stuff
            node temp;
            temp.UpdateState(n, nursery);
            SA(temp, p, nursery);
        }
        else {
            cout << "plz specify BFS, DFS or SA." << endl;
        }
    }
    else {
        perror("Error opening the file.");
    }
    
    inputFile.close();
    outputFile.close();
    
    return 0;
}


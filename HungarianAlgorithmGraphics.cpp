#include<bits/stdc++.h>
#include <graphics.h>
// g++ -std=c++17 -Wl,-stack_size -Wl,0x10000000 main.cpp
#define mt make_tuple
#define mp make_pair
#define pu push_back
#define INF 1000000001
#define MOD 1000000007
#define EPS 1e-6
#define ll long long int
#define ld long double
#define fi first
#define se second
#define all(v) v.begin(),v.end()
#define pr(v) { for(int i=0;i<v.size();i++) { v[i]==INF? cout<<"INF " : cout<<v[i]<<" "; } cout<<endl;}
#define t1(x)                cerr<<#x<<" : "<<x<<endl
#define t2(x, y)             cerr<<#x<<" : "<<x<<" "<<#y<<" : "<<y<<endl
#define t3(x, y, z)          cerr<<#x<<" : " <<x<<" "<<#y<<" : "<<y<<" "<<#z<<" : "<<z<<endl
#define t4(a,b,c,d)          cerr<<#a<<" : "<<a<<" "<<#b<<" : "<<b<<" "<<#c<<" : "<<c<<" "<<#d<<" : "<<d<<endl
#define t5(a,b,c,d,e)          cerr<<#a<<" : "<<a<<" "<<#b<<" : "<<b<<" "<<#c<<" : "<<c<<" "<<#d<<" : "<<d<<" "<<#e<<" : "<<e<<endl
#define t6(a,b,c,d,e,f)          cerr<<#a<<" : "<<a<<" "<<#b<<" : "<<b<<" "<<#c<<" : "<<c<<" "<<#d<<" : "<<d<<" "<<#e<<" : "<<e<<" "<<#f<<" : "<<f<<endl
#define GET_MACRO(_1,_2,_3,_4,_5,_6,NAME,...) NAME
#define t(...) GET_MACRO(__VA_ARGS__,t6,t5, t4, t3, t2, t1)(__VA_ARGS__)
#define _ cerr<<"here"<<endl;
#define __ {ios::sync_with_stdio(false);cin.tie(NULL);cout.tie(NULL);}

using namespace std;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template<class A, class B> ostream& operator<<(ostream& out, const pair<A, B> &a){ return out<<"("<<a.first<<", "<<a.second<<")";}
template <int> ostream& operator<<(ostream& os, const vector<int>& v) { os << "["; for (int i = 0; i < v.size(); ++i) { if(v[i]!=INF) os << v[i]; else os << "INF";if (i != v.size() - 1) os << ", "; } os << "]"; return os; } 
template <typename T> ostream& operator<<(ostream& os, const vector<T>& v) { os << "["; for (int i = 0; i < v.size(); ++i) { os << v[i]; ;if (i != v.size() - 1) os << ", "; } os << "]"; return os; } 

void draw_graph(vector<int> labelX, vector<int>labelY, vector<vector<int>> mat, int n, int m, vector<bool> S, vector<bool> T, vector<int> matchY){
    pair<int,int> Xcoord[n];
    pair<int,int> Ycoord[m];
    // drawing nodes
    for(int i=0;i<n;i++) {
        Xcoord[i] = {50,80*i+50};
        circle(Xcoord[i].first,Xcoord[i].second,30);
        char label[10];
        sprintf(label,"%d",labelX[i]);
        outtextxy(Xcoord[i].first, Xcoord[i].second, label);
    }
    for(int i=0;i<m;i++) {
        Ycoord[i] = {150,80*i+50};
        circle(Ycoord[i].first,Ycoord[i].second,30);
        char label[10];
        sprintf(label,"%d",labelY[i]);
        outtextxy(Ycoord[i].first, Ycoord[i].second, label);
    }
    // drawing equality graph
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if(labelX[i] + labelY[j] == mat[i][j]){
                if(matchY[j]==i){
                    setcolor(4);
                }
                line(Xcoord[i].first, Xcoord[i].second, Ycoord[j].first, Ycoord[j].second);
                setcolor(15);
            }
        }
    }
    // label for S and T
    for(int i=0;i<n;i++){
        if(S[i]){
            char label[2] = "S";
            outtextxy(Xcoord[i].first-40, Xcoord[i].second, label);
        }
    }
    for(int i=0;i<m;i++){
        if(T[i]){
            char label[2] = "T";
            outtextxy(Ycoord[i].first+40, Ycoord[i].second, label);
        }
    }
    // int in = 0;
    // while (in == 0) {
    //     in = getchar();
    // }
    delay(10000);
}

// Initialise the matching to the maximum outgoing edge and the other vertex is free
// Set the labels to those edges
// Return the number of matched vertices
int initLabels(vector<int> &labelX, vector<int> &matchX, vector<int> &matchY, vector<vector<int> > &mat, int n, int m) {
    int matchedCounter = 0;
    for(int i = 0; i < n; i++) {
        int maxWeight = -INF, maxJ = -1;
        for(int j = 0; j < m; j++) {
            if(mat[i][j] > maxWeight) {
                maxWeight = mat[i][j];
                maxJ = j;
            }
        }

        labelX[i] = maxWeight;
        if(matchY[maxJ] == -1) {
            matchedCounter++;
            matchX[i] = maxJ;
            matchY[maxJ] = i;
        }
    }

    return matchedCounter;
}

// Check if N(S) == T
// Note that the check is only happening for the equality graph
bool checkNeighbours(vector<bool> &setT,  vector<int> &slack,  int &neighbour, int m) {
    for(int j = 0; j < m; j++) {
        if(!setT[j] && slack[j] == 0) {
            neighbour = j;
            return false;
        }
    }

    return true;
}

// Update the labels to make N(s) != T
void changeLabel(vector<int> &labelX, vector<int> &labelY, vector<int> &slack, vector<bool> &setS, vector<bool> &setT) {
    int n = labelX.size(), m = labelY.size();
    int changeAmt = INF;
    for(int j = 0; j < m; j++) {
        if(!setT[j]) {
            changeAmt = min(changeAmt, slack[j]);
        }
    }

    for(int i = 0; i < n; i++) {
        if(setS[i]) {
            labelX[i] -= changeAmt;
        }
    }

    for(int j = 0; j < m; j++) {
        if(setT[j]) {
            labelY[j] += changeAmt;
        } else {
            slack[j] -= changeAmt;
        }
    }
}

// Find the augmenting path from root (computed) and target
// Update the matching accordingly
void augmentPath(vector<int> &labelX, vector<int> &labelY, vector<vector<int> > &mat, vector<int> &matchX, vector<int> &matchY, vector<bool> &setS, int target) {
    int n = labelX.size(), m = labelY.size();
    // Get root
    int root = -1;
    for(int i = 0; i < n; i++) {
        if(setS[i] && matchX[i] == -1) root = i;
    }

    // Use the BFS to go from X -> Y. Y -> X can be done from the matching
    queue<int> bfsQueue;
    bfsQueue.push(root);
    vector<int> parent(m, -1), vis(n);
    vis[root] = 1;
    while(!bfsQueue.empty()) {
        int curr = bfsQueue.front();
        bfsQueue.pop();

        for(int j = 0; j < m; j++) {
            if(labelX[curr] + labelY[j] == mat[curr][j] && parent[j] == -1) {
                parent[j] = curr;
                if(target == j) { // Build path using parent and matching 
                    int currX = parent[target];
                    int currY = target;
                    // t(currX, currY, root);
                    matchY[currY] = currX;
                    while(currX != root) {
                        int prevY = matchX[currX];
                        int prevX = parent[prevY];
                        // t(prevX, prevY);
                        matchX[currX] = currY;
                        matchY[prevY] = prevX;
                        currX = prevX;
                        currY = prevY;
                    }
                    matchX[currX] = currY;
                    return;
                } else {
                    if(matchY[j] != -1 && !vis[matchY[j]]) {
                        vis[matchY[j]] = 1;
                        bfsQueue.push(matchY[j]);
                    }
                }
            }
        }
    }
}

void HungarianAlgorithm(vector<vector<int> > &mat, int n, int m) {
    vector<int> labelX(n), labelY(m);
    vector<int> matchX(n, -1), matchY(m, -1);

    int matchedCounter = initLabels(labelX, matchX, matchY, mat, n, m);

    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);
    
    for(; matchedCounter < min(n, m); matchedCounter++) {
        vector<bool> setS(n), setT(m);
        int startI = -1;
        for(int i = 0; i < n; i++) {
            if(matchX[i] == -1) {
                startI = i;
                setS[startI] = true;
                break;
            }
        }

        // Initialise slack
        // Note that setS contains one element at the moment, so no need for minimum
        vector<int> slack(m, INF);
        for(int j = 0; j < m; j++) {
            slack[j] = labelX[startI] + labelY[j] - mat[startI][j];
        }


        int setSCounter = 1;
        while(true) {
            draw_graph(labelX,labelY,mat,n,m,setS,setT,matchY);
            int neighbour = -1;
            // t(setS, setT, labelX, labelY, matchX, matchY);
            bool neighbourSetEqual = checkNeighbours(setT, slack, neighbour, m);
            // t(setS, labelX, labelY, setT, slack);
            if(!neighbourSetEqual) {
                // t(matchY[neighbour]);

                // Find target vertex and augment path
                if(matchY[neighbour] != -1) {
                    // The target vertex isn't free
                    // Add the target vertex to T and it's matched couterpart to S

                    int eleX = matchY[neighbour];
                    setS[eleX] = true;
                    setT[neighbour] = true;
                    setSCounter++;

                    // Since we add one element at a time, only update slack[j] once
                    for(int j = 0; j < m; j++) {
                        slack[j] = min(slack[j], labelX[eleX] + labelY[j] - mat[eleX][j]);
                    }
                } else {
                    // This happens only once, so O(n^2) at once
                    // Once this happens, the matching is guaranteed to increase
                    // t(setSCounter);
                    augmentPath(labelX, labelY, mat, matchX, matchY, setS, neighbour);
                    break;
                }
            } else {
                changeLabel(labelX, labelY, slack, setS, setT);
            }
        }
        draw_graph(labelX,labelY,mat,n,m,setS,setT,matchY);
    }

    // t(labelX, labelY);
    // t(matchX, matchY);

    int maximumWeightMatching = 0;
    for(auto it : labelX) {
        maximumWeightMatching += it;
    }

    for(auto it : labelY) {
        maximumWeightMatching += it;
    }

    closegraph();

    // t(maximumWeightMatching);
    cout << maximumWeightMatching << endl;
}

int main() {
    __;
    int n, m;
    cin >> n >> m;
    vector<vector<int> > v(n, vector<int>(m));
    for(int i = 0; i < n; i++) for(int j = 0; j < m; j++) cin >> v[i][j];

    HungarianAlgorithm(v, n, m);
    return 0;
}

/*
3 3
3 2 3
1 2 0
3 2 1

3 3
1 6 0
0 8 6
4 0 1

3 3
1 4 5
5 7 6
5 8 8

*/

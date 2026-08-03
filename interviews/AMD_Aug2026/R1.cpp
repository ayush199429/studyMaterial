#include<bits/stdc++.h>
using namespace std;

class MedianFinder{
    priority_queue<int> maxHeap;
    priority_queue<int,vector<int>,greater<>> minHeap;
    public:
    void insert(int num){
        if(maxHeap.size() != 0 and num < maxHeap.top()){
            maxHeap.push(num);
            if(maxHeap.size() > minHeap.size() + 1){
                int poppedNum = maxHeap.top();
                maxHeap.pop();
                minHeap.push(poppedNum);
            }
        }
        else{
            minHeap.push(num);
            if(minHeap.size() > maxHeap.size() + 1){
                int poppedNum = minHeap.top();
                minHeap.pop();
                maxHeap.push(poppedNum);
            }
        }
    }
    double getMedian(){
        if(maxHeap.size() == minHeap.size())
            return (maxHeap.top() + minHeap.top()) / 2;
        if(maxHeap.size() > minHeap.size())
            return maxHeap.top();
        return minHeap.top();
    }
};

int main() {
    MedianFinder obj;
    vector<int> v = {5,15,1,3};
    int i = 0;
    while(i < 4){
        int num = v[i++];
        obj.insert(num);
        cout << obj.getMedian() << endl;
    }
}

void printVector(vector<int>& v){
    for(int e : v)
        cout << e << " ";
    cout << endl;
}

void dfs(int cur, int dst, unordered_set<int>& inPath, vector<int>& res, vector<vector<int>>& graph){
    if(cur == dst){
        printVector(res);
        return;
    }
    //cout << "starting \n";
    for(int child : graph[cur]){
        if(inPath.find(child) != inPath.end())
            continue;
        inPath.insert(child);
        //cout << "pushing " << child << endl;
        res.push_back(child);
        dfs(child, dst, inPath, res, graph);
        //cout << "popping " << child << endl;
        res.pop_back();
        inPath.erase(child);
    }
    return;
}

void printPaths(int src, int dst, vector<vector<int>>& graph){
    unordered_set<int> inPath;
    vector<int> res;
    inPath.insert(src);
    res.push_back(src);
    //cout << "starting dfs\n";
    dfs(src, dst, inPath, res, graph);
}

int main(){
    vector<pair<int,int>> edges = {{0,1}, {0,2}, {1,3}, {2,3}, {3,4}};
    vector<vector<int>> graph = vector<vector<int>>(5, vector<int>());
    for(auto& edge : edges){
        int src = edge.first, dst = edge.second;
        graph[src].push_back(dst);
    }
    int src = 0, dst = 4;
    printPaths(src, dst, graph);
    return 0;
}
/*
A -> B
A -> C
B -> D
C -> D
D -> E
*/


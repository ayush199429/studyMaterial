#include<bits/stdc++.h>
using namespace std;
class BoundedBlockingQueue{
    queue<int> q;
    mutex mtx;
    condition_variable cv;
    int n;
    public:
    BoundedBlockingQueue(int n){
        this->n = n;
    }
    void enqueue(int e){
        unique_lock<mutex> lck(mtx);
        cv.wait(lck, [this](){return q.size() < n;});
        q.push(e);
        lck.unlock();
        cv.notify_one();
    }
    int size(){
        int ret;
        unique_lock<mutex> lck(mtx);
        ret = q.size();
        return ret;
    }
    int dequeue(){
        unique_lock<mutex> lck(mtx);
        cv.wait(lck, [this](){return !q.empty();});
        int ret = q.front();
        q.pop();
        lck.unlock();
        cv.notify_one();     
        return ret;
    }
};

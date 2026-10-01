#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

class TaskProcessor{
    map<int,int> mp;
    std::mutex mtx;
    void computeA(){
        {
            std::unique_lock<mutex> lck(mtx);
            //process mp;
        }
        //other stuff...
    }
     void computeB(){
        {
            std::unique_lock<mutex> lck(mtx);
            //process mp;
        }
        //other stuff...
    }   
    void readInput(){
    }
    void parse(){}
    void compute(){
        std::thread t1(computeA, input);
        std::thread t2(copmuteB, input);
        t1.join();
        t2.join();
    }
    void combineResults(){
    }
    void writeOutput(){}
};

int main() {
    int num1, num2;
    int sum;
    cin>>num1>>num2;

    sum = addNumbers(num1,num2);
    cout<< "The sum is " << sum;

    return 0;
}

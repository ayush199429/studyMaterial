#include <cmath>
#include <cstdio>
#include <utility>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

template<typename T>
class Array{
    std::unique_ptr<T[]> uptr;
    T* ptr;
    size_t curSize, totalSize;
    Array(){
        
    }
    Array(size_t size){
        
    }
    Array(const Array& other){
        //delete[] ptr;
        ptr = new T[other.size()];
        uptr = make_unique<T[]>(other.curSize);
        for(int i=0;i<other.size();i++)
            ptr[i] = other[i];
    }
    Array(Array&& other) noexcept{
        ptr = other.ptr;
        curSize = other.size();
        other.ptr = NULL;
        other.curSize = 0;
    }
    size_t size(){
        return curSize;
    }
    Array& operator=(const Array& other){
        if(this == &other)
            return *this;
        delete[] ptr;
        ptr = new T[other.size()];
        curSize = other.size();
        for(int i=0;i<other.size();i++)
            ptr[i] = other[i];
        return *this; 
    }
    Array& operator=(Array&& other){
        delete[] ptr;
        ptr = std::move(other.ptr);
        curSize = other.size();
        other.ptr = NULL;
        other.curSize = 0;
        return *this;
    }
    ~Array(){
        delete[] ptr;
        ptr = NULL;
    }
    void push_back(const T& element){
        if(curSize == totalSize){
            reserve(2 * curSize);
        }
        ptr[++curSize] = element;
    }
    void push_back(T&& element){
        if(curSize == totalSize){
            reserve(2 * curSize);
        }
        ptr[++curSize] = std::move(element);      
    }
    private:
    void reserve(size_t newSize){
        T* newPtr = new T[newSize];
        for(int i=0;i<curSize;i++)
            newPtr[i] = std::move_if_noexcept(ptr[i]);
        delete[] ptr;
        ptr = newPtr;
        totalSize = newSize;
    }
};

int main() {
    T obj;
    T obj1 = std::move(obj);
    obj.push_back(std::move(element));
}

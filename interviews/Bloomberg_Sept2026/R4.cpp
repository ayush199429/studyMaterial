//Data Structures you are aware of. How hashmap works.
//Types of Memory in C++ program. How GC works in Java. 
//Implement SharedPtr.
//Discuss Thread safety. What all type of locks are you aware of in c++.

#include <cstddef>

template <typename T>
class SharedPtr {
    T* ptr;
    size_t* refCount;

    SharedPtr(){
        ptr = nullptr;
        refCount = nullptr;
    }

    SharedPtr(const T& obj){
        ptr = new T(obj); 
        refCount = new size_t(1));
    }

    SharedPtr(const SharedPtr& other){
        ptr = other.ptr;
        refCount = other.refCount;
        (*refCount)++;
    }

    ~SharedPtr() {
        (*refCount)--;
        if (refCount == 0) {
            delete ptr;
            delete refCount;
        }
    }
};

int main(){
  Sharedptr p1;
  auto p2 = p1;
}


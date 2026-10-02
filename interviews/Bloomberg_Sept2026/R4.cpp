//Q. Implement SharedPtr and verbally discuss how to make it thread safe.

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


//Q. Implement SharedPtr and verbally discuss how to make it thread safe.

#include <cstddef>

template <typename T>
class SharedPtr {
    T* ptr;
    size_t* refCount;

public:
    // 1. Default constructor
    SharedPtr()
        : ptr(nullptr), refCount(nullptr) {}

    // 2. Constructor from raw pointer
    explicit SharedPtr(T* p)
        : ptr(p), refCount(new size_t(1)) {}

    // 3. Copy constructor
    SharedPtr(const SharedPtr& other)
        : ptr(other.ptr), refCount(other.refCount) {
        if (refCount)
            ++(*refCount);
    }

    // Destructor
    ~SharedPtr() {
        if (refCount && --(*refCount) == 0) {
            delete ptr;
            delete refCount;
        }
    }
};

int main(){
  Sharedptr p1;
  auto p2 = p1;
}


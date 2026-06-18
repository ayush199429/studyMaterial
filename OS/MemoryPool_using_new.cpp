class MemPool{
    char* pool = nullptr;
    int cap;
    size_t sz;
    int currFree = 0, tailFree;
    int* freePtr;
    public:
    void init(size_t n, size_t size){
        pool = new char[n * size + 4*n];
        cap = n;
        sz = size;
        freePtr = reinterpret_cast<int*>(pool + size*n);
        for(int i=0;i<n;i++)
            freePtr[i] = i+1;
        freePtr[n-1] = -1;
        tailFree = n-1;
    }
    void* getMem(){
        if(currFree == -1)
            return nullptr;
        void* ret = pool + sz * currFree;
        int lastFree = currFree;
        currFree = freePtr[currFree];
        freePtr[lastFree] = -1;
        return ret;
    }
    void free(void* ptr){
        int idx = (static_cast<char*>(ptr) - pool) / sz;
        if(currFree == -1)
            currFree = idx;
        else
            freePtr[tailFree] = idx;
        tailFree = idx;
        freePtr[tailFree] = -1;
    }
};

class Abc{
    int data;
    static MemPool pool;
    public:
    static void* operator new(size_t sz){
        static bool isPoolInitDone = false;
        if(isPoolInitDone == false){
            pool.init(200, sz);
            isPoolInitDone = true;
        }
        return pool.getMem();
    }
    static void operator delete(void* ptr){
        pool.free(ptr);
    }
};
MemPool Abc::pool;

int main(){
    Abc* obj = new Abc();
}

class Threadpool{
    vector<std::thread> threads;
    std::mutex mtx;
    std::condition_variable cv;
    queue<std::function<void()>> tasks;
    bool stop = false;
  public:
    Threadpool(int n){
        for(int i=0;i<n;i++)
            threads.emplace_back(&Threadpool::process, this);
    } 
    void process(){
        while(true){
            unique_lock<std::mutex> lck(mtx);
            cv.wait(lck, [this](){return stop or !tasks.empty();});
            if(stop and tasks.empty())
                return;
            auto task = std::move(tasks.front());
            tasks.pop();
            lck.unlock();
            task();
        }
    }
    void submit(std::function<void()> task){
        {
            unique_lock<mutex> lck(mtx);
            tasks.push(std::move(task));
        }
        cv.notify_one();
    }
    ~Threadpool(){
        {
            unique_lock<mutex> lck(mtx);
            stop = true;
        }
        cv.notify_all();
        for(auto& t : threads)
            t.join();
    }
};

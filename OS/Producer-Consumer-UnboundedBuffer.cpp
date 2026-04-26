template<typename T>
class ThreadSafeQueue {
    queue<T> q;
    mutex mtx;
    condition_variable cv;
    int hasContent = 0;

public:
    void Producer(T obj) {
        unique_lock<mutex> lock(mtx);
        q.push(obj);
        hasContent++;
        cv.notify_one();
    }

    T Consumer() {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this]{ return hasContent > 0; });
        T obj = q.front();
        q.pop();
        hasContent--;
        return obj;
    }
};

int main() {
    BoundedBuffer buffer(5);

    vector<thread> producers;
    vector<thread> consumers;

    int numProducers = 3;
    int numConsumers = 3;

    // Create producers
    for (int i = 0; i < numProducers; i++) {
        producers.emplace_back([&, i]() {
            for (int j = 0; j < 5; j++) {
                buffer.produce(i * 100 + j);
            }
        });
    }

    // Create consumers
    for (int i = 0; i < numConsumers; i++) {
        consumers.emplace_back([&]() {
            for (int j = 0; j < 5; j++) {
                buffer.consume();
            }
        });
    }

    // Join all threads
    for (auto &t : producers) t.join();
    for (auto &t : consumers) t.join();

    return 0;
}

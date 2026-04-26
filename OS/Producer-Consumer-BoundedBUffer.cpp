#include <bits/stdc++.h>
using namespace std;

class BoundedBuffer {
private:
    queue<int> buffer;
    int capacity;
    mutex mtx;
    condition_variable not_full, not_empty;

public:
    BoundedBuffer(int cap) : capacity(cap) {}

    void produce(int item) {
        unique_lock<mutex> lock(mtx);
        not_full.wait(lock, [&]() {
            return buffer.size() < capacity;
        });

        buffer.push(item);
        cout << "Produced: " << item << endl;

        not_empty.notify_one();
    }

    int consume() {
        unique_lock<mutex> lock(mtx);
        not_empty.wait(lock, [&]() {
            return !buffer.empty();
        });

        int item = buffer.front();
        buffer.pop();

        cout << "Consumed: " << item << endl;

        not_full.notify_one();
        return item;
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

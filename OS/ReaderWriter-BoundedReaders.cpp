#include <bits/stdc++.h>
#include <shared_mutex>
#include <semaphore>
using namespace std;

class ReaderWriter {
private:
    shared_mutex rw_mtx;
    counting_semaphore<> readerSlots; // limits readers

public:
    ReaderWriter(int maxReaders)
        : readerSlots(maxReaders) {}

    void Read() {
        readerSlots.acquire();              // limit N readers
        shared_lock<shared_mutex> lock(rw_mtx);

        cout << "Reading by thread " << this_thread::get_id() << endl;

        this_thread::sleep_for(chrono::milliseconds(100));

        readerSlots.release();
    }

    void Write() {
        unique_lock<shared_mutex> lock(rw_mtx);

        cout << "Writing by thread " << this_thread::get_id() << endl;

        this_thread::sleep_for(chrono::milliseconds(100));
    }
};

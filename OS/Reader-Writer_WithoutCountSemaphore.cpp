#include <bits/stdc++.h>
#include <semaphore>
using namespace std;

// ─────────────────────────────────────────────
// Solution 1: Semaphore-based
// ─────────────────────────────────────────────
class ReaderWriter_Semaphore {
private:
    mutex            mtx;       // protects readcnt
    binary_semaphore wrt{1};    // resource access (cross-thread → semaphore)
    int              readcnt = 0;

public:
    void Read() {
        {
            lock_guard<mutex> lock(mtx);
            if (++readcnt == 1) wrt.acquire();  // first reader locks writers out
        }

        cout << "[R] " << this_thread::get_id() << "\n";
        this_thread::sleep_for(chrono::milliseconds(100));

        {
            lock_guard<mutex> lock(mtx);
            if (--readcnt == 0) wrt.release();  // last reader lets writers in
        }
    }

    void Write() {
        wrt.acquire();

        cout << "[W] " << this_thread::get_id() << "\n";
        this_thread::sleep_for(chrono::milliseconds(100));

        wrt.release();
    }
};


// ─────────────────────────────────────────────
// Solution 2: CV-based (two CVs → notify_one)
// ─────────────────────────────────────────────
class ReaderWriter_CV {
private:
    mutex              mtx;
    condition_variable readers_cv;   // readers wait here  (!writing)
    condition_variable writers_cv;   // writers wait here  (!writing && readcnt==0)
    int                readcnt = 0;
    bool               writing = false;

public:
    void Read() {
        // Entry
        unique_lock<mutex> lock(mtx);
        readers_cv.wait(lock, [&]{ return !writing; });
        readcnt++;
        lock.unlock();

        cout << "[R] " << this_thread::get_id() << "\n";
        this_thread::sleep_for(chrono::milliseconds(100));

        // Exit
        lock.lock();
        if (--readcnt == 0) writers_cv.notify_one();  // ✅ only writers wait here
    }

    void Write() {
        // Entry
        unique_lock<mutex> lock(mtx);
        writers_cv.wait(lock, [&]{ return !writing && readcnt == 0; });
        writing = true;
        lock.unlock();

        cout << "[W] " << this_thread::get_id() << "\n";
        this_thread::sleep_for(chrono::milliseconds(100));

        // Exit
        lock.lock();
        writing = false;
        readers_cv.notify_all();    // ✅ wake all waiting readers
        writers_cv.notify_one();    // ✅ wake one waiting writer
    }
};


// ─────────────────────────────────────────────
// Driver
// ─────────────────────────────────────────────
int main() {
    ReaderWriter_Semaphore rw;

    vector<thread> threads;
    for (int i = 0; i < 5; i++)
        threads.emplace_back([&]{ rw.Read(); });
    for (int i = 0; i < 2; i++)
        threads.emplace_back([&]{ rw.Write(); });

    for (auto& t : threads) t.join();
}

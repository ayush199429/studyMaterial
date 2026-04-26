#include <bits/stdc++.h>
#include <shared_mutex>
using namespace std;

class ReaderWriter {
private:
    fstream file;
    shared_mutex rw_mtx;

public:
    ReaderWriter(const string& filename) {
        file.open(filename, ios::in | ios::out | ios::app);
    }

    void Write(const string& data) {
        unique_lock<shared_mutex> lock(rw_mtx);
        file << data << endl;
    }

    void Read() {
        shared_lock<shared_mutex> lock(rw_mtx);

        file.clear();        // reset EOF
        file.seekg(0);       // go to start

        string line;
        while (getline(file, line)) {
            cout << "Read: " << line << endl;
        }
    }
};

void writerTask(ReaderWriter* obj, string data) {
    for (int i = 0; i < 3; i++) {
        obj->Write(data);
    }
}

void readerTask(ReaderWriter* obj) {
    for (int i = 0; i < 5; i++) {
        obj->Read();
    }
}

int main() {
    ReaderWriter obj("file.txt");

    vector<thread> reads, writes;

    for (int i = 0; i < 2; i++) {
        writes.emplace_back(writerTask, &obj, "Hello from writer");
    }

    for (int i = 0; i < 3; i++) {
        reads.emplace_back(readerTask, &obj);
    }

    for (auto &t : writes) t.join();
    for (auto &t : reads) t.join();

    return 0;
}

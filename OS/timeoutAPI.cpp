#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

using namespace std;

class DataFetcher {
private:
    int data;

public:
    DataFetcher(int x) {
        data = x;
    }

    int get() {

        // Seed random using current time
        srand(time(nullptr));

        // Random delay between 1 to 5 seconds
        int delay = 2;//rand() % 5 + 1;

        cout << "Sleeping for "
             << delay
             << " seconds\n";

        this_thread::sleep_for(
            chrono::seconds(delay)
        );

        return data;
    }
};

int fetch(DataFetcher& obj, int timeoutSec) {

    future<int> fut = async(
        launch::async,
        &DataFetcher::get,
        &obj
    );

    future_status status =
        fut.wait_for(
            chrono::seconds(timeoutSec)
        );

    if(status == future_status::ready)
        return fut.get();

    return -1;
}

int main() {

    DataFetcher obj(10);

    cout << fetch(obj, 3) << endl;

    return 0;
}

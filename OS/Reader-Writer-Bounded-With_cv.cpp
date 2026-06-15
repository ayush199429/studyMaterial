class BoundedReaderWriterLock {
private:
    mutex mtx;
    condition_variable cv;

    int activeReaders = 0;
    bool writerActive = false;

    const int MAX_READERS;

public:
    BoundedReaderWriterLock(int k)
        : MAX_READERS(k) {}

    void readerLock() {
        {
          unique_lock<mutex> lock(mtx);
  
          cv.wait(lock, [&] {
              return !writerActive &&
                     activeReaders < MAX_READERS;
          });
  
          activeReaders++;
       }

      //Read............
      {
        unique_lock<mutex> lock(mtx);

        activeReaders--;

        cv.notify_all();
      }
    }

    void writer() {
        unique_lock<mutex> lock(mtx);

        cv.wait(lock, [&] {
            return !writerActive &&
                   activeReaders == 0;
        });

        writerActive = true;

      //Write...............

        writerActive = false;

        cv.notify_all();
    }
};

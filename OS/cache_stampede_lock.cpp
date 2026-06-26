class Cache {
    unordered_map<int, int> cache;
    unordered_map<int, mutex> mtxs;
    unordered_map<int, condition_variable> cvs;
    unordered_map<int, bool> loading;

public:

    int get(int id) {

        // Fast path
        if (cache.contains(id))
            return cache[id];

        unique_lock<mutex> lk(mtxs[id]);

        // Double check
        if (cache.contains(id))
            return cache[id];

        if (loading[id]) {

            cvs[id].wait(lk, [&] {
                return !loading[id];
            });

            return cache[id];
        }

        // I become the loader
        loading[id] = true;

        lk.unlock();

        //-----------------
        // Slow DB call
        //-----------------
        int val = getFromDB(id);
        cache[id] = val;
        
        lk.lock();
        loading[id] = false;
        lk.unlock();

        cvs[id].notify_all();

        return val;
    }
};

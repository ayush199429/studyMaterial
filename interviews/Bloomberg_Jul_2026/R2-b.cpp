/*
design a system that is receiving trades continously. A trade has a security name, ex, IBM and the size, ex 500. Users can subscribe 
and unsubscribe to trades, ex a user can subscribe to {HDFC, 200} and the system has to notify all users who have their 
size < currentTrade's size, ex, if curr size is 250 then notify all users who set alert for HDFC and size <=250. It should be thread-safe.
you have to return a handle to user when they subscribe and they will use to call the unsubscribe api, it should be basically a unique 
identifier for that alert.
*/

using AlertId = uint64_t;

struct Trade {
    string security;
    int size;
};

struct Alert {
    AlertId id;
    string security;
    int threshold;
    function<void(const Trade&)> callback;
};

using AlertList = list<Alert>;
using AlertIterator = AlertList::iterator;

struct AlertBucket {
    mutable shared_mutex mtx;
    AlertList alerts;
};

struct AlertLocation {
    string security;
    int threshold;
    AlertIterator it;
};

class TradeAlertService {

    // security -> threshold -> alerts
    unordered_map<string, map<int, AlertBucket>> buckets;

    // handle -> exact location of alert
    unordered_map<AlertId, AlertLocation> handleMap;

    // Protects buckets/handleMap themselves.
    mutex globalMutex;

    atomic<AlertId> nextId{1};

public:

    AlertId subscribe(
        const string& security,
        int threshold,
        function<void(const Trade&)> callback)
    {
        AlertId id = nextId.fetch_add(1);

        lock_guard<mutex> globalLock(globalMutex);

        auto& bucket = buckets[security][threshold];

        unique_lock<shared_mutex> bucketLock(bucket.mtx);

        bucket.alerts.push_back({
            id,
            security,
            threshold,
            callback
        });

        auto it = prev(bucket.alerts.end());

        handleMap[id] = {
            security,
            threshold,
            it
        };

        return id;
    }

    void unsubscribe(AlertId id)
    {
        lock_guard<mutex> globalLock(globalMutex);

        auto it = handleMap.find(id);

        if (it == handleMap.end())
            return;

        AlertLocation location = it->second;

        auto& bucket =
            buckets[location.security][location.threshold];

        unique_lock<shared_mutex> bucketLock(bucket.mtx);

        bucket.alerts.erase(location.it);

        handleMap.erase(it);
    }

    void notify(const Trade& trade)
    {
        vector<function<void(const Trade&)>> callbacks;

        lock_guard<mutex> globalLock(globalMutex);

        auto securityIt = buckets.find(trade.security);

        if (securityIt == buckets.end())
            return;

        auto& thresholdMap = securityIt->second;

        for (auto it = thresholdMap.begin();
             it != thresholdMap.end() &&
             it->first <= trade.size;
             ++it)
        {
            auto& bucket = it->second;

            shared_lock<shared_mutex> bucketLock(bucket.mtx);

            for (const auto& alert : bucket.alerts)
            {
                callbacks.push_back(alert.callback);
            }
        }

        // bucket locks released here
        // global lock also released after this scope
    }

    // Call callbacks outside all locks.
};

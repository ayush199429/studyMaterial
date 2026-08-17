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

struct AlertBucket {
    shared_mutex mtx;
    vector<Alert> alerts;
};

class TradeAlertService {

    // security -> threshold -> alert bucket
    unordered_map<string,
        map<int, AlertBucket>> alerts;

    // alertId -> (security, threshold)
    unordered_map<AlertId,
        pair<string, int>> alertIndex;

    // Protects creation/access of the outer structures.
    mutex mapMutex;

    atomic<AlertId> nextId{1};

public:

    AlertId subscribe(
        const string& security,
        int threshold,
        function<void(const Trade&)> callback)
    {
        AlertId id = nextId.fetch_add(1);

        {
            lock_guard<mutex> lock(mapMutex);

            alerts[security][threshold].alerts.push_back(
                {id, security, threshold, callback}
            );

            alertIndex[id] = {security, threshold};
        }

        return id;
    }

    void unsubscribe(AlertId id)
    {
        pair<string, int> location;

        {
            lock_guard<mutex> lock(mapMutex);

            auto it = alertIndex.find(id);

            if (it == alertIndex.end())
                return;

            location = it->second;
            alertIndex.erase(it);
        }

        auto& [security, threshold] = location;

        auto& bucket = alerts[security][threshold];

        unique_lock<shared_mutex> lock(bucket.mtx);

        auto& vec = bucket.alerts;

        vec.erase(
            remove_if(vec.begin(), vec.end(),
                [&](const Alert& alert) {
                    return alert.id == id;
                }),
            vec.end()
        );
    }

    void notify(const Trade& trade)
    {
        vector<Alert> alertsToNotify;

        // Find all thresholds <= trade.size.
        // For every relevant bucket, take shared lock,
        // read/copy its alerts and release the lock.

        for (auto& [threshold, bucket] :
             alerts[trade.security])
        {
            if (threshold > trade.size)
                break;

            shared_lock<shared_mutex> lock(bucket.mtx);

            for (const auto& alert : bucket.alerts)
            {
                alertsToNotify.push_back(alert);
            }
        }

        // Do NOT call user callbacks while holding locks.
        for (const auto& alert : alertsToNotify)
        {
            alert.callback(trade);
        }
    }
};

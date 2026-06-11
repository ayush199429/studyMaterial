enum MODE{
    EMAIL,
    SMS,
    IN_APP,
    WHATSAPP
};

enum PRIORITY{
    LOW,
    MEDIUM,
    HIGH
};

class Notifier{
    bool notify(string msg, string address, NotifResponse* que) = 0;
};

class EMAILNotifier : public Notifier{
    bool notify(string msg, string address, NotifResponse* que){}
};
class SMSNotifier : public Notifier{};
class CompositeNotifier : public Notifier{
    unordered_map<MODE,string> mp;
    void addPref(Preflist list){
        for(auto e : list)
            mp[e.first] = e.second;
    }
    bool notify(Notification notif, uitn id, NotifResponse* que){
        for(auto e : mp){
            Notifier obj = Factory::getObject(e.first);
            obj.notify(notif, e.second, que);
        }
    }
};

class UserNotifService{
    unordered_map<uint,CompositeNotifier> mp;
    bool process(Notification& notif, uint id, NotifResponse* que){
        mp[notif.usrId].notify(notif, id, que);
    }
    void addUser(uint id, PrefList list){
        mp[id].addPref(list);
    }
}
    
class NotifResponse{
    uint id;
    timestamp sent_at;
    MODE mode;
    string recepient address;
};

class NotificationManager{
    priority_queue<pair<uint,Priority>, vector<pair<uint,Priority>>, greater<pair<uint,Priority>>, Comp> pq;
    vector<std::thread> threadPool;
    std::mutex mtx;
    condition_variable cv;
    queue<pair<uint,NotifResponse>> responseQue;
    class NotifAck{
        uint id;
        time collected_at;
    };
    void consume(){
        while(true){
            Notification notif;
            uint id;
            {
                unique_lock<mutex> lck(mtx);
                cv.wait(lck, [&](){return !pq.empty();});
                notif = pq.top().second;
                id = pq.top().first;
                pq.pop();
            }
            prcoessNotification(notif, id);
        }
    }
    bool prcoessNotification(Notification& notif, uint id, NotifResponse* responseQue){
        usrNotifSrv.process(notif, id, responseQue);
    }
    NotificationManager(NotifResponse* queue){
        responseQue = queue;
        for(int i=0;i<nThreads;i++)
            threadPool.emplace_back({&this->consume});
    }
    NotifAck notify(Notification notif, uint id){
        unique_lock<mutex> lck(mtx);
        pq.push({notif, id});
        cv.notify_one();
        return {id, currenttime()};
    }
};

struct Notification{
    string msg;
    User usrID;
    NotificationPriority priority;
};

class INotifier{
    virtual void send(string msg, User usr) = 0;
};

class EmailNotifier : public INotifChannel{
};
class SMSNotifier : public INotifChannel{
};
class InAppNotifier : public INotifChannel{
};

enum NotificationPriority{
    HIGH,
    MEDIUM,
    LOW
};

enum NotifChannel{
    EMail,
    SMS,
    InApp
};

class SafeQueue{
    struct NotifPacket{
        string msg;
        uint usrID;
        NotifChannel channel;
    };
    std::mutex mtx;
    condition_variable cv;
    vector<thread> threadVec;
    int threadLimit = 5;
    SafeQueue(){
         for(int i=0;i<threadLimit;i++)
             threadVec.push_back({&SafeQueue::consume, this});
    }
    queue<NotifPacket> qHigh, qMed, qLow;
    void push(Notification notif, NotifChannel channel){
        auto priority = notif.priority;
        auto& q = getQueueFromPriority(priority);
        {
            auto lock_guard<mutex> lck(mtx);
            q.push({notif.msg, notif.usrID, channel});
        }
        cv.notify_one();
    }
    NotifPacket dequeue() {
        std::unique_lock lock(mtx);
        cv.wait(lock, [&]{ return stop || !qHigh.empty() || !qMed.empty() || !qLow.empty(); });
        auto& q = !qHigh.empty() ? qHigh : !qMed.empty() ? qMed : qLow;
        auto pkt = q.front(); q.pop();
        return pkt;
    }

    void consume() {
        while (true) {
            auto pkt = dequeue();
            auto notifier = getNotifier(pkt.channel);
            notifier->send(pkt.notif.msg, pkt.notif.usrID);
        }
    }
    ~SafeQueue(){
        for(auto& thread : threadVec)
            thread.join();
    }
};

class NotifService{
    SafeQueue sq;
    map<int,set<NotifChannel>> mp;
    void push(Notification& notif){
        uint usrID = notif.usrID;
        for(auto channel : mp[usrID])
            sq.push(notif, channel);
    }
};

int main(){
    NotifService srv;
    for(int i=0;i<100;i++){
        Notification notif(msg, user, NotificationPriority::HIGH)
        srv.push(notif);
    }
}


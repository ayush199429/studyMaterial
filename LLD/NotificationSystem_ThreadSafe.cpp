#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <memory>
#include <atomic>

// ── (Alert, IChannel, UserPrefs, ChannelFactory same as before) ──


// ── Priority Queue (thread-safe) ─────────────────────────────────

struct NotifTask {
    Alert       alert;
    std::string userId;
};

class PriorityDispatchQueue {
    std::queue<NotifTask> high_, medium_, low_;
    std::mutex            mu_;
    std::condition_variable cv_;
    std::atomic<bool>     stop_{ false };

public:
    // called by producer threads
    void enqueue(NotifTask task) {
        {
            std::unique_lock lock(mu_);          // lock only for the push
            switch (task.alert.priority) {
                case Priority::HIGH:   high_.push(std::move(task));   break;
                case Priority::MEDIUM: medium_.push(std::move(task)); break;
                case Priority::LOW:    low_.push(std::move(task));    break;
            }
        }                                        // unlock BEFORE notify
        cv_.notify_one();                        // woken worker acquires lock immediately
    }

    // called by worker threads — blocks until a task is available
    bool dequeue(NotifTask& out) {
        std::unique_lock lock(mu_);              // cv_.wait needs unique_lock
        cv_.wait(lock, [&] {                     // atomically releases lock while sleeping
            return stop_ || !high_.empty() || !medium_.empty() || !low_.empty();
        });                                      // re-acquires lock on wake

        if (stop_ && high_.empty() && medium_.empty() && low_.empty())
            return false;                        // signal worker to exit

        // drain highest priority first
        auto& q = !high_.empty() ? high_ : !medium_.empty() ? medium_ : low_;
        out = std::move(q.front());
        q.pop();
        return true;
    }                                            // lock released here on scope exit

    void shutdown() { stop_ = true; cv_.notify_all(); }
};


// ── Worker Thread Pool ────────────────────────────────────────────

class NotificationWorkerPool {
    PriorityDispatchQueue&  queue_;
    std::vector<std::thread> workers_;

    void workerLoop() {
        NotifTask task;
        while (queue_.dequeue(task)) {
            // fetch user prefs, create channel, send
            for (ChannelType ct : getPrefs(task.userId).channels) {
                auto ch = ChannelFactory::create(ct);
                ch->send(task.alert, task.userId);
            }
        }
    }

public:
    NotificationWorkerPool(PriorityDispatchQueue& q, int numWorkers)
        : queue_(q) {
        for (int i = 0; i < numWorkers; ++i)
            workers_.emplace_back([this] { workerLoop(); });
    }

    ~NotificationWorkerPool() {
        queue_.shutdown();
        for (auto& t : workers_) t.join();
    }
};


// ── NotificationService (producer-facing API) ─────────────────────

class NotificationService {
    PriorityDispatchQueue& queue_;
    UserPreferenceStore&   prefs_;

public:
    NotificationService(PriorityDispatchQueue& q, UserPreferenceStore& p)
        : queue_(q), prefs_(p) {}

    // producer threads call this — just enqueues, never blocks
    void dispatch(const Alert& alert, const std::string& userId) {
        auto* p = prefs_.get(userId);
        if (!p) return;
        if (alert.priority > p->minPriority) return;   // priority gate

        queue_.enqueue({ alert, userId });
    }
};


// ── Main: producer threads + worker pool ─────────────────────────

int main() {
    UserPreferenceStore store;
    store.save({ "alice", { ChannelType::EMAIL, ChannelType::SMS }, Priority::LOW });
    store.save({ "bob",   { ChannelType::IN_APP },                  Priority::MEDIUM });

    PriorityDispatchQueue queue;
    NotificationWorkerPool pool(queue, /*numWorkers=*/3);
    NotificationService    svc(queue, store);

    // simulate multiple producer threads
    auto producer = [&](Priority p, std::string msg, std::string user) {
        svc.dispatch({ "id", msg, p }, user);
    };

    std::thread t1(producer, Priority::LOW,    "Weekly digest",  "alice");
    std::thread t2(producer, Priority::HIGH,   "Disk critical!", "alice");
    std::thread t3(producer, Priority::MEDIUM, "CPU at 80%",     "bob");

    t1.join(); t2.join(); t3.join();
    // ~NotificationWorkerPool() shuts down cleanly
}

enum class RateLimiterAlgo
{
    FIXED_WINDOW,
    SLIDING_WINDOW,
    TOKEN_BUCKET
};

class Request
{
public:
    string service;
    string endpoint;
    string key;            // userId / apiKey / IP
};

class RateLimitConfig
{
public:
    RateLimiterAlgo algo;

    int maxRequests;

    int windowSize;        // seconds

    int bucketSize;

    int refillRate;        // tokens/sec
};

class IRateLimiter
{
public:
    virtual bool allow(const string& key) = 0;

    virtual ~IRateLimiter() = default;
};

class FixedWindowLimiter : public IRateLimiter
{
private:
    mutex mtx;

    // key -> {windowStart, requestCount}
    unordered_map<
        string,
        pair<uint64_t,int>
    > requests;

    RateLimitConfig config;

public:
    FixedWindowLimiter(const RateLimitConfig& cfg);

    bool allow(const string& key) override;
};

class SlidingWindowLimiter : public IRateLimiter
{
private:
    mutex mtx;

    // key -> timestamps
    unordered_map<
        string,
        deque<uint64_t>
    > requests;

    RateLimitConfig config;

public:
    SlidingWindowLimiter(const RateLimitConfig& cfg);

    bool allow(const string& key) override;
};

struct Bucket
{
    int tokens;

    uint64_t lastRefillTime;
};

class TokenBucketLimiter : public IRateLimiter
{
private:
    mutex mtx;

    unordered_map<
        string,
        Bucket
    > buckets;

    RateLimitConfig config;

public:
    TokenBucketLimiter(const RateLimitConfig& cfg);

    bool allow(const string& key) override;
};

class RateLimiterFactory
{
public:
    static unique_ptr<IRateLimiter>
    create(const RateLimitConfig& cfg)
    {
        switch(cfg.algo)
        {
            case RateLimiterAlgo::FIXED_WINDOW:
                return make_unique<FixedWindowLimiter>(cfg);

            case RateLimiterAlgo::SLIDING_WINDOW:
                return make_unique<SlidingWindowLimiter>(cfg);

            case RateLimiterAlgo::TOKEN_BUCKET:
                return make_unique<TokenBucketLimiter>(cfg);
        }

        return nullptr;
    }
};

class RateLimiterManager
{
private:

    // "Auth:/login" -> limiter
    unordered_map<
        string,
        unique_ptr<IRateLimiter>
    > limiters;

public:

    void addLimiter(
        const string& service,
        const string& endpoint,
        const RateLimitConfig& config)
    {
        string id = service + ":" + endpoint;

        limiters[id] =
            RateLimiterFactory::create(config);
    }

    bool allow(const Request& req)
    {
        string id =
            req.service + ":" + req.endpoint;

        auto it = limiters.find(id);

        if(it == limiters.end())
            return true;

        return it->second->allow(req.key);
    }
};

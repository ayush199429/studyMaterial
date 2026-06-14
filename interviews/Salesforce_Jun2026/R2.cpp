Rate Limiter HLD + LLD:

Rate Limiter LLD

class RateLimiter {
map<UserId, vector<LimitingAlgo*>> userToAlgos;

public:
Response validate(UserId userID, Timestamp timestamp) {

```
    vector<Response> responses;

    for(auto algo : userToAlgos[userID]) {
        responses.push_back(
            algo->validate(userID, timestamp)
        );
    }

    return mergeAllResponses(responses);
}
```

};

class LimitingAlgo {
public:
virtual Response validate(
UserId userID,
Timestamp timestamp
) = 0;
};

class SlidingWindow : public LimitingAlgo {
public:
Response validate(
UserId userID,
Timestamp timestamp
) override {
...
}
};

class TokenBucket : public LimitingAlgo {

```
map<UserId, pair<uint, Timestamp>> userState;
map<UserId, uint> limitValue;
```

public:

```
Response validate(
    UserId userID,
    Timestamp timestamp
) override {

    if(userState[userID].first >= limitValue[userID])
        return {false, refreshTimestamp, 0};

    mtx.lock();

    userState[userID].first++;

    mtx.unlock();

    return {true, refreshTimestamp,
            limitValue[userID] -
            userState[userID].first};
}
```

};

class Response {
public:
bool isAllowed;
Timestamp nextRefreshTime;
uint tokensRemaining;
};

Design discussion:

* Strategy Pattern:
  LimitingAlgo is the base interface.
  SlidingWindow / TokenBucket derive from it.

* Factory Pattern:
  Can be added later to create algorithm instances.

* RateLimiter contains:
  userID -> list of limiting algorithms.

* For every request:
  call validate() on all configured algorithms
  and merge the responses.

* Thread safety discussion:
  Initially lock was only around increment.
  Interviewer pointed out race condition.
  Correct approach is to lock both:
  check limit
  increment counter

* Concurrency discussion:
  Mentioned mutexes and lock_guard.
  Not much experience with atomics.

Note:
The implementation that was called Token Bucket during the interview
was actually closer to a Fixed Window implementation since it maintained:

```
counter
refreshTimestamp
```

and reset counts periodically rather than refilling tokens continuously.

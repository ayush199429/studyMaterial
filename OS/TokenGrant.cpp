A bucket holds a limited number of tokens
Users request tokens — they are either granted or rejected based on current availability
If granted, the user holds those n tokens for exactly 1 hour, after which they are automatically returned to the bucket
Need to support: requestTokens(userId, n), releaseTokens(userId), and a background expiry mechanism
Key discussion points:

Using a min-heap / priority queue ordered by expiry time to efficiently handle auto-release
Handling concurrent requests — locking strategies vs. atomic operations
Edge cases: partial grants, user requesting tokens they already hold, token expiry during an active request
Time complexity of grant and release operations
---------------------------------------------------------------------------------------------------------

class TokenManager{
    int tokenCount = 0;
    queue<pair<time,val,userId>> q;
    unordered_map<int,int>> userTokens;
    std::mutex tokenMutex, qMtx;
    
    bool doGrant(int n, int userId){
        if(tokenCount >= n){
            userTokens[userId] += n;
            q.push({currentTime()+60,userId,n});
            tokenCount -= n;
        }
    }
    bool grant(int n, int userId){
        bool b = false;
        if(tokenCount >= n){
            auto unique_lock<std::mutex> lck(tokenMutex);
            b = doGrant(n, userId);
        }
        if(!b){
            auto unique_lock<std::mutex> lck(qMtx);
            int count = 0;
            while(!q.empty() and q.front().first <= currentTime())
                auto pr = q.front();
                if(userTokens[pr.second] > pr.third){
                    count += pr.third;
                    userTokens[pr.second] -= pr.third;
                }
                q.pop();
            }
            {
                auto unique_lock<std::mutex> lck2(tokenMutex);
                tokenCount += count;
                return doGrant(n, userId);
                
            }
        }
        return b;
    }
    void release(int userId, int n){
        if(userTokens[userId]){
            auto unique_lock<std::mutex> lck(tokenMutex);
            tokenCount += n;
            userTokens[userId] -= n;
        }
    }
};

int main() {
    
}

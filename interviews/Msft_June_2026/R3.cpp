/*

Unified Search Provider in an Operating System similar to Windows / iPhone / Android

Design a Search Service for an Operating System that allows users to search across multiple sources — such as local files, system settings, installed apps, apps available in the app store, contacts, and the web.
The system should query these providers in parallel, aggregate the results, remove duplicates, and rank them before presenting to the user.
*/

#include <utility>
map<string,string> mp;
[
    "invoice" : [path1, path2, path3];
    "cycle" : [path1]
    "resolution" : []
]
static_map
invoice_cycle.pdf
readFile() : fetch keywords from file (if its a text file) and populate the map.

Entities:
SearchManager - Uses the individual components like LocalFilesManager, SystemSettingManager, etc
LocalFilesManager - LocalFiles search/add
WebManager, AppManager

class BaseSearchManager{
    virtual vector<string> search(string query) = 0;
};

class SearchManager : public BaseSearchManager{
    vector<BaseSearchManager> searchManagersList;
    vector<string> search(string query){
       priority_queue<pair<int,string>> pq;
       vector<vector<string>> managerResults(searchManagersList.size(),{});
       int i = 0;
       for(auto& manager : searchManagersList){
            t1.launch(manager.search(query, managerResults[i++]));
       }
       for(auto threads : threadList)
            thread.join();
        for(auto v : managerResults){
            for(string s : v)
                pq.push(getRank(s));
        }
       return createVectorFromPQ(pq); //vector of search results arranged in rank order.
    }
};
class LocalFilesManager : public SearchManager {
    unordered_map<> mp;
    mutex mtx;
};

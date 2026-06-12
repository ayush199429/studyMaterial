#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;
/**
Example:

Input: [4,8,4,3,2,6,9], k=3
Output: 21 (9+8+4)

Input: [4,8,14,3,2,6,9], k=3
Output: 26 (4+8+14)
*/

//Write function.
int rec(int i, int j, vector<int>& v, int k){
    if(k == 0)
        return 0;
    int a = v[i] + rec(i+1, j, v, k-1);
    int b = v[j] + rec(i, j-1, v, k-1);
    return max(a, b);
}

int main() {
    vector<int> v{4,8,14,3,2,6,9};
    int k = 3;
    cout << rec(0, v.size()-1, v, k);
    return 0;
}

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include<bits/stdc++.h>
using namespace std;

/*
For example, consider the sequence of customer actions: "abccdcdaab"
For k = 2 (e.g., browsing and adding to cart), the longest sequence is "ccdcd"
For k = 3 (e.g., browsing, adding to cart, and purchasing), the longest sequence is "ccdcdaa"

d : 1
c : 2


input: aabc, k=2


*/

string findLongest(string s, int k){
    unordered_map<char,int> mp;
    int startIdx = -1, len = 0;
    int left = 0;
    for(int right=0;right<s.size();right++){
        mp[s[right]]++;
        while(mp.size() > k and left < right){
            mp[s[left]]--;
            if(mp[s[left]] == 0)
                mp.erase(s[left]);
            left++;
        }
        if(mp.size() == k and right-left+1 >= len){
            len = right-left+1;
            startIdx = left;
        }
    }
    if(len >= k)
        return s.substr(startIdx, len);
    return "";
} 

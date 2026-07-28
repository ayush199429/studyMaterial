/*
They also asked few c++ ques like how hashmap is implemented, how doubly linked list is implemented.
Then they asked many ques on gRPC project like how sequencing of events is maintained, what is grpc, why used it, why is it faster, 
does it use json, does it use http?
i said grpc is faster because it uses protobufs and provides streaming ability. But i fucked on whether grpc uses http or not and whether
it uses json or not. this disappointed them.
*/

using namespace std;

/*
A A B B B A
a b b b c c c b b = 
0 7  
A B C C C B B
(A,1)
a b c c c b b a a c
c,1
*/ 

string candyCrush(const string& s, int k){
    stack<pair<char,int>> recordStack;
    for(char c : s){
        if(!recordStack.empty() and recordStack.top().first != c){
            if(recordStack.top().second >= k)
                recordStack.pop();
        }
        if(!recordStack.empty() and recordStack.top().first == c)
            recordStack.top().second++;
        else
            recordStack.push({c,1});
    }
    string result;
    while(!recordStack.empty()){
        auto element = recordStack.top();
        char c = element.first;
        int count = element.second;
        recordStack.pop();
        for(int i=0;i<count;i++)
            result+= c;
    }
    reverse(result.begin(), result.end());
    return result;
}

int main() {
    string s = "abcccbbaac";
    cout << candyCrush(s, 3);
    return 0;
}




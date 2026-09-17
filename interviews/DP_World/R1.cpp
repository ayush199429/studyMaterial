struct node{
    int val;
    node* left = NULL, *right = NULL;
    node(int v){
        val = v;
    }
};

int findMax(node* root, int& maxx){
    if(root == nullptr)
        return 0;
    int curr = root->val;
    int left = findMax(root->left, maxx);
    int right = findMax(root->right, maxx);
    int currSum = curr + left + right;
    maxx = max(maxx, currSum);
    return max(left+curr, right+curr);
}

int main() {
    node* root = new node(-10);
    root->left = new node(19);
    root->right = new node(20);
    root->right->left = new node(15);
    root->right->right = new node(7);
    int ans = INT_MIN;
    findMax(root, ans);
    cout << ans << endl;
}

void merge(vector<int>& v, int l1, int h1, int l2, int h2, vector<int>& temp){
    int i = l1, j = l2, ptr = 0;
    while(i <= h1 and j <= h2){
        if(v[i] <= v[j])
            temp[ptr++] = v[i++];
        else
            temp[ptr++] = v[j++];
    }
    while(i <= h1){
        temp[ptr++] = v[i++];
    }
    while(j <= h2){
        temp[ptr++] = v[j++];
    }
}

void copy(vector<int>& src, vector<int>& dst, int start){
    for(int i=0;i<src.size();i++)
        dst[start++] = src[i];
}

void mergeSort(vector<int>& v, int l, int h){
    if(l == h)
        return;
    int mid = (l+h)/2;
    mergeSort(v, l, mid);
    mergeSort(v, mid+1, h);
    vector<int> temp(h-l+1);
    merge(v, l, mid, mid+1, h, temp);
    copy(temp, v, l);
}

int main(){
    vector<int> v = {5,3,8,2,1,7,4,6};
    mergeSort(v, 0, 7);
    for(int x : v)
        cout << x;
}


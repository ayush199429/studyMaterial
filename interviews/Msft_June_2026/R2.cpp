#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

/*
Req:
Create path if not present
If root itself is wrong - Invalid, more than 2 dots invalid, special chars, double dots going beyong root.
Use .ext to diff between file/folder
We can use . and ..

Entities:
Files
Folders
Paths


path: HOME/Doc/user/folder1

Representation:
              HOME
    D1.  D2.   F1.   D3     D4 ......
  D5 F2. F3.        
*/

class BaseDir{
    string path;
    
};
class Dir : BaseDir{
    unordered_map<string,BaseDir> contents;
    void addContent(BaseDir* obj){
        contents[obj].insert(obj);
    }
};
class File : public BaseDir{
    ofstream file;
    void addContent(string content){
        file.open(path);
        file << content;
        file.close();
    }
};

class DirManager{
    BaseDir* root = new Dir("HOME");
    bool mkDirInt(string path, BaseDir* obj){
        int i = 0;
        while(path[i] != '/' and i < path.size())
            i++;
        string curPath = path.substr(0,i);
        if(obj.isFile() and curPath.size() != 0)
            return false;
        if(curPath.size() == 0)
            return true;
        if(obj.contents.find(curPath) == contents.end()){
            if(isFile(curPath))
                obj.contents[curPath] = new File(curPath);
            else
                obj.contents[curPath] = new Dir(curPath);
            return mkDirInt(path.substr(i), contents[curPath]);
        }
        else{
            return mkDirInt(path.substr(i), contents[curPath]);
        }
    }
    bool mkdir(string path){
        int i = 0;
        while(path[i] != '/' and i < path.size())
            i++;
        string curPath = path.substr(0,i);
        if(root.path == curPath)
            return mkDirInt(path.substr(i), root);
        return false;
    }
    
    vector<BaseDir> dir(string path){
        if(path.isFile(path))
            return {};
        int i = 0;
        BaseDir* ptr = root;
        while(i < path.size()){
            while(path[i] != '/' and i < path.size())
                i++;
            string curPath = path.substr(0,i);
            path = path.substr(i);
            BaseDir* ptr = ptr->contents[curPath];
        }
        vector<BaseDir> res;
        for(auto content : ptr->contents)
            res.push_back(*content);
        return res;
    }
    
}





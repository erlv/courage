#include <string>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

template<typename T>
void outputVector(vector<T> val) {
    stringstream ss;
    
    int i=0;
    for (; i < val.size(); i++) {
        ss << val[i] << " ";
    }
    ss << endl;
    cout << ss.str();
}

class Solution {
public:
    void updateStringMask(string& s, vector<int>& strMask) {
        int i = 0;
        for (; i < s.length(); i++) {
            int curStrMaskIdx = (int)(s[i] - 'a');
            strMask[curStrMaskIdx] = strMask[curStrMaskIdx]+1;
        }
    }

    bool CheckPermutation(string s1, string s2) {
        vector<int> s1mask(128,0);
        vector<int> s2mask(128,0);
        
    //   updateStringMask(s1, s1mask);
    //   updateStringMask(s2, s2mask);

        outputVector<int>(s1mask);
        outputVector<int>(s2mask);

        if (s1mask == s2mask) {
            return true;
        } else {
            return false;
        }
    }
};


int main() {
    class Solution cursol;
    string s1 {"abc"};
    string s2{"bad"};
    bool res = cursol.CheckPermutation(s1, s2);
    if (res) {
        cout << "true";
     } else {
        cout << "false";
    }
} 
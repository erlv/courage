class Solution {
public:
    vector<vector<int>> generate(int numRows) {
        vector<vector<int>> res;
        int i = 0;
        for (; i < numRows; i++) {
            vector<int> currow;
            if (res.size() > 0 ) {
                vector<int> prevrow = res[res.size()-1];
                currow.push_back(1);
                int j = 1;
                for (; j < i; j++) {
                    currow.push_back(prevrow[j-1] + prevrow[j]);
                }
            }
            currow.push_back(1);
            res.push_back(currow);
        }
        return res;
    }
};
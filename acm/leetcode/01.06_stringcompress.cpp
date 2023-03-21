class Solution {
public:
    string compressString(string S) {
        stringstream compSstream;
        int i = 0;
        int j = 0;
        while (i < S.length()) {
            char curi = S[i];
            int j =0;
            while (curi == S[i+j]) {
                j++;
            }
            compSstream << curi << j;
            i = i+j;
        }
        string compS = compSstream.str();
        return compS.length() < S.length() ? compS : S;
    }
};
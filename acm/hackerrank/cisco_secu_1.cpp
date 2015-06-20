/*
 * =====================================================================================
 *
 *       Filename:  cisco_secu_1.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/09/2015 09:17:35 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kun Ling (), kunling@lingcc.com
 *   Organization:  Lingcc.com
 *
 * =====================================================================================
 */

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
using namespace std;


int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    string key("hacker");
    const char* key_ptr = key.c_str();
    int key_len = key.length();
    string code;
    cin >> code;
    const char* code_ptr = code.c_str();
    vector<char> code_char_vec;
    int code_len = code.length();
    
    int i =0;
    for(; i < code_len; i+=2) {
        char low_4 = code_ptr[i] >= 'a'? (code_ptr[i] - 'a' + 10) : (code_ptr[i] - '0');
        char high_4 = code_ptr[i+1] >= 'a'? (code_ptr[i+1] - 'a' + 10) : (code_ptr[i+1] - '0');
        char tmp = (high_4 &0xf) << 4 | (low_4 & 0xf);
        cout << "low_4:" << (int)low_4 << ", high_4:" << (int)high_4 << ", tmp:" << (int)tmp << endl;
        code_char_vec.push_back(tmp);
    }
    
    string res;     
    for(i=0; i < code_len/2 +1; i++) {
       int j = i % key_len; 
        cout << "key[i] " << key[j] << ", code_char_vec[i] " << code_char_vec[i] << endl;
       res.push_back(key[j] ^ code_char_vec[i]);
    }
    cout << res << endl;

    return 0;
}


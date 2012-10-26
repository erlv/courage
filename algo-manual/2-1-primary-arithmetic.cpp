#include <iostream>
#include <string>

using namespace std;

int get_carry_op(int a, int b) {
    int res= 0;

    int a_mod, b_mod;
    int a_div, b_div;
    a_div = a/10;
    b_div = b/10;
    a_mod = a%10;
    b_mod = b%10;
    int last_carry = 0;
    while( !(a_mod <= 0 || b_mod <= 0)) {
        a = a_div;
        b = b_div;
        if(a_mod + b_mod + last_carry > 9) {
            res ++;
            last_carry = 1;
        } else {
            last_carry = 0;
        }
        a_div = a/10;
        b_div = b/10;
        a_mod = a%10;
        b_mod = b%10;
    }

    return res;
}

int main() {
    int a,b;
    cin >> a >> b;
    while( !(a == 0 && b == 0) ) {
        int carry_op=get_carry_op(a,b);
        if(carry_op>1)
            cout << carry_op << " carry operations"<<endl;
        else if(carry_op == 1)
            cout << "1 carry operation" << endl;
        else if(carry_op == 0)
            cout << "No carry operation" << endl;
        cin >> a >> b;
    }
}

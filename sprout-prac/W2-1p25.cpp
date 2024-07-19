#include <iostream>
using namespace std;
int main () {
    int month;
    cin>>month;
    if(1<=month and month<=3) {
        cout<<"spring\n";
    }
    if(4<=month and month<=6) {
        cout<<"summer\n";
    }
    if(7<=month and month<=9) {
        cout<<"fall\n";
    }
    if(10<=month and month<=12) {
        cout<<"winter\n";
    }
    return 0;
}



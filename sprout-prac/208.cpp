#include<iostream>
using namespace std;

int main() {
    int month;
    cin>>month;
    if(1<=month && month<=3) {
        cout<<"spring\n";
    }
    if(4<=month && month<=6) {
        cout<<"summer\n";
    }
    if(7<=month && month<=9) {
        cout<<"fall\n";
    }
    if(10<=month && month<=12) {
        cout<<"winter\n";
    }
}

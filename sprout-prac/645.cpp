#include<iostream>
using namespace std;

int main() {
    int x, y, z;
    cin>>x>>y>>z;
    if(x==y && y==z)
        cout<<"Equilateral Triangle\n";

    else if(x!=y && x!=z && y!=z)
        cout<<"Scalene Triangle\n";

    else
        cout<<"Scalene Triangle\n";

}

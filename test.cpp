#include<iostream>
using namespace std;

int main() {
    for (int i = 10, j = 1; i -= j;)
    cout << (i -= j++);
}
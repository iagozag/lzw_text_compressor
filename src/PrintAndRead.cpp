#include "../include/PrintAndRead.h"

Print::Print() {}

Print::~Print() {}

void Print::add_bits(char c, int n) {
    while(n--) buf.push_back(c&1), c>>=1;
}

void Print::add_bits(string s,int z) {
    while(z--) buf.push_back(false);
    for(auto c:s) buf.push_back(c=='1');
}

void Print::print(ofstream &f, int n) {
    f.write((char*)&n,4);

    char c=0;
    int i=8;
    for(auto b: buf) {
        i--;
        c|=(b<<i);
        if(not i) {
            f << c;
            i = 8;
            c = 0;
        }
    }
    if(i!=8) f.write(&c,1);
    buf.clear();
}


void Read(ifstream &f, vector<string> &codes) {
    int n;
    f.read((char*)&n,4);
    int sz = (1<<n)-256;
    char c;
    string s;
    while((int)codes.size()<sz and f.read(&c,1)) {
        int i=8;
        while(i--) {
            if((int)s.size()==n) {
                codes.push_back(s);
                s.clear();
            }
            s.push_back('0'+((c&(1<<i))!=0));
        }
    }
}

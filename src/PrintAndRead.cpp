#include "../include/PrintAndRead.h"
#include <algorithm>
#include <fstream>

Print::Print() {}

Print::~Print() {}

void Print::add_bits(string s,int z) {
    while(z>0) buf.push_back(false),z--;
    for(auto c:s) buf.push_back(c=='1');
}

void Print::print(ofstream &f, int n) {
    if(not buf.size()) return;

    f.write((char*)&n,4);
    n = buf.size()/8;
    if(buf.size()%8) n++;
    f.write((char*)&n,4);
    
    string s;
    char c=0;
    int i=8;
    for(auto b: buf) {
        i--;
        c|=(b<<i);
        if(not i) {
            s.push_back(c);
            i = 8;
            c = 0;
        }
    }
    if(i!=8) s.push_back(c);
    f.write(&s[0],s.size());
    buf.clear();
}

void Print::print(ofstream &f) {
    string s;
    reverse(buf.begin(),buf.end());
    char c;
    while(buf.size()) {
        for(int i=0;i<8 and buf.size();i++) {
            c<<=1;
            c |= buf.back();
            buf.pop_back();
        }
        s.push_back(c);
    }

    f.write(&s[0],s.size());
}
void Read(ifstream &f, vector<string> &codes) {
    codes.clear();
    int n=0;
    f.read((char*)&n,4);
    if(not n) return;
    int sz=0;
    f.read((char*)&sz,4);
    char str[sz];
    char c;
    string s;
    f.read(str,sz);
    for(int q = 0;q<sz;q++) {
        c = str[q];
        int i=8;
        while(i--) {
            if((int)s.size()==n) {
                codes.push_back(s);
                s.clear();
            }
            s.push_back('0'+((c&(1<<i))!=0));
        }
    }
    if((int)s.size()==n) {
        codes.push_back(s);
        s.clear();
    }
}

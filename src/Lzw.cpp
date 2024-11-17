#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <set>

#include "../include/Lzw.h"
#include "../include/PrintAndRead.h"

Lzw::Lzw(int _max_bits, bool _stats, bool _fixed): max_bits(_max_bits), min_bits(9), stats(_stats), fixed(_fixed) {}

Lzw::~Lzw() {}

void Lzw::reset_dict() {
    num_bits=min_bits;
    t.clear();
    for(int i=0;i<=255;i++) {
        t.insert(char_to_bin(i),int_to_bin(t.size()));
    }
}

string Lzw::char_to_bin(char c) {
    string r(8,'0');
    for(int i=0;i<8;i++) r[i] = '0' + (((c>>i)&1)==1);
    return r;
}

string Lzw::int_to_bin(int n) {
    string r(num_bits,'0');
    for(int i=0;i<num_bits;i++) r[i] += ((n>>i)&1)==1;
    reverse(r.begin(),r.end());
    return r;
}

void Lzw::compress(string file) {
    ifstream in(file, ios::binary);
    if(not in.good()) throw invalid_argument("Error: file not found");

    reset_dict();
    string s,sb;
    Print p;
    
    char c;
    vector<string> buf;
    ofstream out(file+".lzw", ios::binary);

    while(in.read(&c,1)) {
        sb=char_to_bin(c);
        s=s+sb;

        string v = t.find(s);
       	if(v.size()) continue;

        if((int)t.size()>=(1<<num_bits)) {
            if(num_bits==max_bits) {
                for(int j=0;j<8;j++) s.pop_back();
                buf.push_back(t.find(s));

                for(auto &ss: buf) 
                    p.add_bits(ss,num_bits-((int)ss.size()));

                p.print(out,num_bits);
                buf.clear();
                reset_dict();
                t.insert(s+sb,int_to_bin(t.size()));
                s = sb;
                continue;
            } else num_bits++;
        }

        t.insert(s,int_to_bin(t.size()));

        for(int j=0;j<8;j++) s.pop_back();
        buf.push_back(t.find(s));
        s = sb;
    }
    for(auto ss:buf) p.add_bits(ss,num_bits-ss.size());
    p.add_bits(t.find(s),num_bits-t.find(s).size());
    p.print(out, num_bits);

    in.close();
    out.close();
}


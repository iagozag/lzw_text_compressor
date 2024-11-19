#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "../include/Lzw.h"
#include "../include/PrintAndRead.h"

Lzw::Lzw(int _max_bits, bool _stats, bool _fixed): max_bits(_max_bits), min_bits(9), stats(_stats), fixed(_fixed) {}

Lzw::~Lzw() {}

void Lzw::reset_dict() {
    num_bits=min_bits;
    t.clear();
    char i=0;
    do {
        t.insert(char_to_bin(i),int_to_bin(t.size()));
        i++;
    } while (i);
}

void Lzw::reset_dict2() {
    t.clear();
    char i=0;
    do {
        string s = char_to_bin(i);
        t.insert(int_to_bin(t.size()),s);
        i++;
    } while (i);
}

string Lzw::char_to_bin(char c) {
    string r(8,'0');
    for(int i=0;i<8;i++) r[i] = '0' + (((c>>i)&1)==1);
    reverse(r.begin(),r.end());
    return r;
}

string Lzw::int_to_bin(int n) {
    string r(num_bits,'0');
    for(int i=0;i<num_bits;i++) r[r.size()-i-1] += ((n>>i)&1)==1;
    return r;
}

void Lzw::compress(string file) {
    ifstream in("inputs/"+file, ios::binary);
    if(not in.good()) throw invalid_argument("Error: file not found");
    ofstream out("outputs/"+file+".lzw", ios::binary);

    reset_dict();
    string s,sb;
    Print p;
    char c;
    vector<string> buf;
    char str[1025];
    long n=0,i=0;

    while(1) {
        if(n==i) {
            in.read(str,1024);
            n = in.gcount();
            if(not n) break;
            i=0;
        }

        c = str[i++];

        sb=char_to_bin(c);
        s=s+sb;

        string v = t.find(s);
        if(v.size()) continue;

        if((int)t.size()>=(1<<num_bits)) {
            if(num_bits>=max_bits) {
                for(int j=0;j<8;j++) s.pop_back();
                buf.push_back(v);
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
        buf.push_back(v);
        s = sb;
    }

    for(auto ss:buf) p.add_bits(ss,num_bits-ss.size());
    p.add_bits(t.find(s),num_bits-t.find(s).size());
    p.print(out, num_bits);

    in.close();
    out.close();
}

void Lzw::decompress(string file) {
    ifstream in("inputs/"+file+".lzw", ios::binary);
    ofstream out("inputs/teste"+file, ios::binary);
    if(not in.good()) throw invalid_argument("Error: file not found");

    vector<string> buf;
    Print p;
    Read(in,buf);
    if(not buf.size()) return;
    num_bits = buf.back().size();
    reverse(buf.begin(),buf.end());

    string code = buf.back();
    buf.pop_back();

    reset_dict2();
    string str = t.find(code);
    string s = buf.back(), entry;
    p.add_bits(str);
    
    while(1) {
        if(not buf.size()) {
            Read(in,buf);
            if(not buf.size()) break;
            
            num_bits = buf.back().size();
            reverse(buf.begin(),buf.end());
        }

        code = buf.back();
        buf.pop_back();
        

        if(t.find(code).size()) entry = t.find(code);
        else entry = str+str.substr(0,8);
        p.add_bits(entry);
        if(t.size()>=(1<<num_bits)) reset_dict2();
        t.insert(int_to_bin(t.size()), str+entry.substr(0,8));
        str = entry;
    }

    p.print(out);

    in.close();
    out.close();
}   

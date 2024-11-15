#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>

#include "../include/Lzw.h"

Lzw::Lzw(string _file,int _max_bits, bool _stats, bool _fixed):
         file(_file), max_bits(_max_bits), stats(_stats), fixed(_fixed) {
    
    ifstream f(file);
    if (not f.good()) {
        cerr << "Erro ao encontrar o arquivo.\n";
        exit(1);
    } else f.close();

    char c=0;
    do {
        string s = char_to_bin(c);
        t.insert(s,int_to_bin(t.get_size()));
        c++;
    } while (c!=0);
}

Lzw::~Lzw() {}

string Lzw::char_to_bin(char c) {
    string r(8,',');
    for(int i=0;i<8;i++) r[i] = '0' + (((c>>i)&1)==1);
    return r;
}

string Lzw::int_to_bin(int n) {
    string r;
    do {
        r.push_back('0'+((n&1)==1));
        n>>=1;
    } while (n);
    reverse(r.begin(),r.end());
    return r;
}

void Lzw::compress() {
    string s,sb;
    char c;
    cout << t.get_size() << endl;
    ifstream in(file, ios::binary);    
    // ofstream out(file+".lzw",ios::out);
    while(in.get(c)) {
        sb=char_to_bin(c);
        s=s+sb;
        cout << s << endl;
        string v = t.find(s);
        if(v.size()) continue; // já tá no dicionário

        // não tá no dicionário
        t.insert(s,int_to_bin(t.get_size()));

        for(int j=0;j<8;j++) s.pop_back();
        cout << "s: " << t.find(s) << endl;
        s = sb;
    }
    cout << t.find(s) << endl;

    in.close();
        // cout << ' ' << s << ' ' <<  "testeeee\n";
    // out.close();
}


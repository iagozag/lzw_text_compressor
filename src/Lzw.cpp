#include <algorithm>
#include <fstream>
#include <stdexcept>

#include "../include/Lzw.h"
#include "../include/PrintAndRead.h"

Lzw::Lzw(int _max_bits, bool _stats, bool _fixed): max_bits(_max_bits), min_bits(9), input_file_size(0), output_file_size(0), 
                                                   dictionary_resets(0), stats(_stats), fixed(_fixed) {}

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
    in.seekg(0, ios::end);
    input_file_size = in.tellg();
    in.seekg(0, ios::beg);

    ofstream out("outputs/"+file+".lzw", ios::binary);

    reset_dict();
    Print p;
    char c;
    string s,sb;
    vector<string> buf;
    char str[1024];
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
        if(v.size()>0) continue;

        if((int)t.size()>=(1<<num_bits)) {
            if(num_bits>=max_bits) {
                dictionary_resets++;
                for(int j=0;j<8;j++) s.pop_back();
                buf.push_back(t.find(s));
                for(auto &ss: buf) 
                    p.add_bits(ss,num_bits-((int)ss.size()));

                p.print(out,num_bits);
                buf.clear();
                reset_dict();
                s+=sb;
                t.insert(s,int_to_bin(t.size()));
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


    output_file_size = out.tellp();

    in.close();
    out.close();
}

void Lzw::decompress(string file) {
    ifstream in("outputs/"+file, ios::binary);
    ofstream out("inputs/"+file.substr(0, file.size()-4), ios::binary);
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
        if(t.size()>=(1<<num_bits)) reset_dict2(), dictionary_resets++;
        t.insert(int_to_bin(t.size()), str+entry.substr(0,8));
        str = entry;
    }

    p.print(out);

    in.close();
    out.close();
}   

void Lzw::print_stats(bool compress, long long time_taken, string stats_file_path) {
    std::ofstream stats_file("stats/"+stats_file_path+".stats");

    if (compress) {
        stats_file << "COMPRESSION STATISTICS: " << endl;
        stats_file << "Time taken: " << time_taken << " ms" << endl;
        stats_file << "Input File Size: " << input_file_size << " bytes" << endl;
        stats_file << "Compressed File Size: " << output_file_size << " bytes" << endl;
        stats_file << "Compression Ratio: " << 100.0 - (100.0 * output_file_size / input_file_size) << "%" << endl;
        stats_file << "Number of Dictionary Resets: " << dictionary_resets << endl;
    } else {
        stats_file << "DECOMPRESSION STATISTICS: " << endl;
        stats_file << "Time taken: " << time_taken << " ms" << endl;
        stats_file << "Number of Dictionary Resets: " << dictionary_resets << endl;
    }

    stats_file.close();
}

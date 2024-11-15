#ifndef LZW_H
#define LZW_H

#include "Trie.h"

class Lzw {
    private:
        Trie t;
        string file;
        int max_bits;
        bool stats;
        bool fixed;

    public:
        Lzw(string flie, int _max_bits=12, bool _stats = false, bool _fixed = false);
        ~Lzw();
        void compress();
        string char_to_bin(char c);
        string int_to_bin(int n);
        
};



#endif
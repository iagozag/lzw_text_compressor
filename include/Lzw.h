#ifndef LZW_H
#define LZW_H

#include "Trie.h"

class Lzw {
    private:
        Trie t;
        // map<string,string> m;
        int max_bits;
        int min_bits;
        int num_bits;
		bool stats;
        bool fixed;

        /*
         * @brief Resets the dictionary
         */
        void reset_dict();

    public:
        Lzw(int _max_bits=12, bool stats = false, bool _fixed = false);
        ~Lzw();
        /*
         * @brief Returns a binary string that represents the binary representation of c
         */
        string char_to_bin(char c);
        
        /*
         * @brief Returns a binary string that represents the binary representation of n (with a size limit)
         */
        string int_to_bin(int n);


        /*
         * @brief Runs the LZW algorithm on the file with the name given as argument
         */
        void compress(string file);
        
};



#endif

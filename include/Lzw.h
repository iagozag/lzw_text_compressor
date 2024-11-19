#ifndef LZW_H
#define LZW_H

#include <iostream>
#include <chrono>

#include "Trie.h"

class Lzw {
    private:
        Trie t;
        int max_bits;
        int min_bits;
        int num_bits;
        long input_file_size;
        long output_file_size;
        int dictionary_resets;
		bool stats;
        bool fixed;

        /*
         * @brief Resets the dictionary
         */
        void reset_dict();

        void reset_dict2();

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
        
        /*
         * @brief Runs the LZW algorithm on the compressed file to get back the original file
         */
        void decompress(string file);

        void print_stats(bool, long long, string);
};



#endif

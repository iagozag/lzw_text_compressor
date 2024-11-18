#ifndef PRINT_H
#define PRINT_H

#include <fstream>
#include <vector>

using namespace std;

class Print {
    private:
        vector<bool> buf;

    public:
        Print();
        ~Print();
        /*
         * @brief Adds n bits of char to the buffer to be written to the output
         */
        void add_bits(char c, int n=8);

        /*
         * @brief Adds all the digits of the binary string to the buffer to be written to the output
         */
        void add_bits(string s, int z=0);

        /*
         * @brief Prints an int, and then all the bits of the buffer
         */
        void print(ofstream &f, int n);

        /*
         * @brief Prints all the bits of the buffer
         */
        void print(ofstream &f);
};


void Read(ifstream &f, vector<string> &codes);

#endif

#ifndef PRINT_H
#define PRINT_H

#include <fstream>
#include <vector>

using namespace std;

class Print {
    private:
        vector<bool> buf;

    public:
        /*
         * @brief Constructs Print class
         */
        Print();
    
        /*
         * @brief Destroys Print class
         */
        ~Print();

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

/*
* @brief Reads the compressed file as written by the print method
*/
void Read(ifstream &f, vector<string> &codes);

#endif

#include <iostream>
#include "../include/Trie.h"
#include "../include/Lzw.h"

using namespace std;

#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#define endl '\n'

int main(int argc, char* argv[]){ _
	if (argc < 2) {
		cout << "How to use: ./main <file> [--max-bits <value> (12 by default)] [--stats (0 by default)]" << endl;
		exit(0);
	}

	string file = "inputs/";
	file += argv[1];
	int max_bits = 12; 
	bool stats = 0;

	for(int i = 2; i < argc; i++){
		string arg = argv[i];
        if(arg == "--max-bits" and i+1 < argc) max_bits = atoi(argv[i+1]), i++;
        else if(arg == "--stats") stats = 1;
    }

	Lzw l(max_bits, stats);
	l.compress(file);

    exit(0);
}

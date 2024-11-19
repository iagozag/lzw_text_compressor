#include <iostream>
#include "../include/Lzw.h"

using namespace std;

#define endl '\n'

int main(int argc, char* argv[]){
	if (argc < 3) {
		cout << "How to use: ./main <file> (--compress / --decompress) [--max-bits <value> (12 by default)] [--stats (false by default)] [--fixed (false by default)]" << endl;
		exit(0);
	}

	string file = "";
	file += argv[1];
	int max_bits = 12; 
	bool comp = 1, stats = 0, fixed = 0;

	for(int i = 2; i < argc; i++){
		string arg = argv[i];
        if(arg == "--max-bits" and i+1 < argc) max_bits = atoi(argv[i+1]), i++;
        else if(arg == "--decompress") comp = 0;
        else if(arg == "--stats") stats = 1;
        else if(arg == "--fixed") fixed = 1;
    }

	Lzw l(max_bits, stats, fixed);

	auto start = chrono::high_resolution_clock::now();

	if(comp) l.compress(file);
	else l.decompress(file);

    auto end = chrono::high_resolution_clock::now();

	if(stats) l.print_stats(comp, chrono::duration_cast<chrono::milliseconds>(end - start).count(), file);
    exit(0);
}

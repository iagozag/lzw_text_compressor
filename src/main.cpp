#include <iostream>
#include "../include/Trie.h"

using namespace std;

#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#define endl '\n'

int main(int argc, char* argv[]){ 
	if (argc < 2) {
		cout << "How to use: ./main <file> [--max-bits <value> (12 by default)] [--stats (0 by default)]" << endl;
		exit(0);
	}

	string file = "../inputs/";
	file += argv[1];
	int max_bits = 12; 
	bool stats = 0;

	for(int i = 2; i < argc; i++){
		string arg = argv[i];
        if(arg == "--max-bits" and i+1 < argc) max_bits = atoi(argv[i+1]), i++;
        else if(arg == "--stats") stats = 1;
    }

	cout << file << " " << max_bits << " " << stats << endl;

	/*
	Trie t;	

	t.insert("001", "1");
	t.insert("011", "2");
	t.insert("1", "3");
	t.insert("11", "4");

	cout << t.find("001") << endl << 
			t.find("011") << endl << 
			t.find("0") << endl <<
			t.find("000") << endl <<
			t.find("1") << endl <<
			t.find("10") << endl <<
			t.find("11") << endl;

	t.insert("11", "5");
	cout << t.find("11") << endl;

	t.erase("11");

	t.insert("11", "6");
	cout << t.find("11") << endl;
	*/

    exit(0);
}

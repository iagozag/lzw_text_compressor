#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>

using namespace std;

class Trie {
	private:
		struct node{
			string t;
			string v;
			int l;
			int r;
			int par;
			bool end;

			node(string _t = "", string _v = ""): t(_t), v(_v), l(-1), r(-1), end(false) {
				if(_v != "") end = true;
			}
		};

		vector<node> nodes;
		int sz;
		int idx;

		int create(string, string);

	public:
		Trie();
		~Trie();
		int size();
		void insert(string, string);
		void erase(string);
		string find(string);
		void clear();
};

#endif

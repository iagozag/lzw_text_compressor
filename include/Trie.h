#ifndef TRIE_H
#define TRIE_H

#include <string>

using namespace std;

class Trie {
	private:
		struct node{
			string t;
			node* l;
			node* r;
			node* par;
			node* end;

			node(string _t = ""): t(_t), l(nullptr), r(nullptr), end(nullptr) {}
		};

		node* root;
		int size;

	public:
		Trie();
		~Trie();
		void insert(string, string);
		void erase(string);
		string find(string);
		void clear(node*);
		void clear();
};

#endif

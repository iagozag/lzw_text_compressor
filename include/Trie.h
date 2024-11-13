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
			node* end;

			node(string _t = ""): t(_t), l(nullptr), r(nullptr), end(nullptr){}
		};

		node* root;

	public:
		Trie();
		~Trie();
		void insert(string s, string);
		void erase(string s);
		string find(string s);
};

#endif

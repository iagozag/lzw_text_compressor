#ifndef TRIE_H
#define TRIE_H

#include <string>

using namespace std;

class Trie {
	private:
		struct node{
			string t, value;
			node* l;
			node* r;
			node* end;

			node(string _t = "", string _value = ""): t(_t), value(_value), l(nullptr), r(nullptr), end(nullptr){}
		};

		node* root;

	public:
		Trie();
		~Trie();
		void insert(string s, string);
		void erase(string s);
		void find(string s);
};

#endif

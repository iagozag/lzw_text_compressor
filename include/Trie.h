#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>

using namespace std;

class Trie {
	private:
	    /*
         * @brief Structure representing a node in the Trie
         */
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

        /*
         * @brief Creates a new node in the Trie with text and value empty by default
         */
		int create(string t, string v);

	public:
        /*
         * @brief Constructs the Trie
         */
        Trie();

        /*
         * @brief Destroys the Trie
         */
        ~Trie();

        /*
         * @brief Returns the size of the Trie
         */
        int size();

        /*
         * @brief Inserts a key-value pair into the Trie
         */
        void insert(string key, string value);

        /*
         * @brief Removes a key from the Trie
         */
        void erase(string key);

        /*
         * @brief Finds the value associated with a key
         */
        string find(string key);

        /*
         * @brief Clears all nodes in the Trie
         */
        void clear();
};

#endif

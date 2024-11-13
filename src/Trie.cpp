#include "../include/Trie.h"

Trie::Trie(): root(new node()), size(0) {};

Trie::~Trie(){
	clear();
	delete root;
}

void Trie::insert(string s, string val){
	int i = 0, n = s.size();
	node* cur = root;
	while(i < n){
		int j = 0, m = cur->t.size();
		while(j < m and i < n and cur->t[j] == s[i])
			i++, j++;

		if(j == m){
			if(i == n){
				if(cur->end == nullptr) cur->end = new node(val), cur->end->par = cur, size++;
				else cur->end->t = val;
			}
			else if(s[i] == '0'){
				if(cur->l == nullptr) cur->l = new node(s.substr(i, n-i)), cur->l->par = cur;
				cur = cur->l;
			} else{
				if(cur->r == nullptr) cur->r = new node(s.substr(i, n-i)), cur->r->par = cur;
				cur = cur->r;
			}
		} else{
			node* new_par = new node(cur->t.substr(0, j));
			new_par->par = cur->par;

			if(cur->t[0] == '0') cur->par->l = new_par;
			else cur->par->r = new_par;

			cur->t = cur->t.substr(j, m-j);

			if(s[i] == '0'){
				new_par->l = new node(s.substr(i, n-i));
				new_par->l->par = new_par;
				new_par->r = cur;
				cur = new_par->l;
			} else{
				new_par->r = new node(s.substr(i, n-i));
				new_par->r->par = new_par;
				new_par->l = cur;
				cur = new_par->r;
			}

			cur->par = new_par;
		}
	}
}

void Trie::erase(string s){
	int i = 0, n = s.size();
	node* cur = root;
	while(i < n){
		int j = 0, m = cur->t.size();
		while(j < m and i < n and cur->t[j] == s[i])
			i++, j++;

		if(j != m) return;

		if(i == n){
			if(cur->end == nullptr) return;
			cur = cur->end;
		}
		else if(s[i] == '0' and cur->l != nullptr) cur = cur->l;
		else if(s[i] == '1' and cur->r != nullptr) cur = cur->r;
		else return;
	}

	node* tmp;
	while(cur != root and cur->l == nullptr and cur->r == nullptr and cur->end == nullptr){
		tmp = cur;
		cur = cur->par;

		if(cur->l == tmp) cur->l = nullptr;
		if(cur->r == tmp) cur->r = nullptr;
		if(cur->end == tmp) cur->end = nullptr;

		delete tmp;
	}
}

string Trie::find(string s){
	int i = 0, n = s.size();
	node* cur = root;
	while(i < n){
		int j = 0, m = cur->t.size();
		while(j < m and i < n and cur->t[j] == s[i])
			i++, j++;

		if(j != m) break;

		if(i == n) return (cur->end == nullptr ? "" : cur->end->t);
		else if(s[i] == '0' and cur->l != nullptr) cur = cur->l;
		else if(s[i] == '1' and cur->r != nullptr) cur = cur->r;
		else break;
	}
	return "";
}

void Trie::clear(node* cur){
	if(cur->l != nullptr) clear(cur->l);
	if(cur->r != nullptr) clear(cur->r);
	if(cur->end != nullptr) delete cur->end, cur->end = nullptr;

	delete cur;
}

void Trie::clear(){
	clear(root);
	root = new node();
}

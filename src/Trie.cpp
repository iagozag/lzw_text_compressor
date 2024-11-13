#include "../include/Trie.h"

Trie::Trie(): root(new node()) {};

Trie::~Trie(){

}

void Trie::insert(string s, string val){
	int i = 0, n = s.size();
	node* cur = root;
	node* par = nullptr;
	while(i < n){
		int j = 0, m = cur->t.size();
		while(j < m and i < n and cur->t[j] == s[i])
			i++, j++;

		if(j == m){
			if(i == n) cur->end = new node(val);
			else if(s[i] == '0'){
				if(cur->l == nullptr) cur->l = new node(s.substr(i, n-i));
				par = cur, cur = cur->l;
			} else{
				if(cur->r == nullptr) cur->r = new node(s.substr(i, n-i));
				par = cur, cur = cur->r;
			}
		} else{
			node* new_par = new node(cur->t.substr(0, j));
			if(cur->t[0] == '0') par->l = new_par;
			else par->r = new_par;

			par = new_par;
			cur->t = cur->t.substr(j, m-j);

			if(s[i] == '0'){
				par->l = new node(s.substr(i, n-i));
				par->r = cur;
				cur = par->l;
			} else{
				par->r = new node(s.substr(i, n-i));
				par->l = cur;
				cur = par->r;
			}
		}
	}
}

void Trie::erase(string s){

}

string Trie::find(string s){
	int i = 0, n = s.size();
	node* cur = root;
	while(i < n){
		int j = 0, m = cur->t.size();
		while(j < m and i < n and cur->t[j] == s[i])
			i++, j++;

		if(j == m){
			if(i == n) return (cur->end == nullptr ? "" : cur->end->t);
			else if(s[i] == '0' and cur->l != nullptr) cur = cur->l;
			else if(cur->r != nullptr) cur = cur->r;
		}
		else break;
	}
	return "";
}

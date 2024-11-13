#include "../include/Trie.h"

Trie::Trie(): root(new node()) {};

Trie::~Trie(){

}

void Trie::insert(string s, string val){
	int i = 0, n = s.size();
	node* cur = root;
	node* par = nullptr;
	while(i < n){
		bool ok = true; int j = 0;
		for(auto& c: cur->t){
			if(c != s[i]){
				ok = false; 
				break;
			}

			i++, j++;
			if(i == n) break;
		}

		if(ok){
			if(i == n) cur->end = new node("", val);
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
			cur->t = cur->t.substr(j, cur->t.size()-j);

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

void Trie::find(string s){

}

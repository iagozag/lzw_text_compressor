#include "../include/Trie.h"

Trie::Trie(): sz(0), idx(1) { nodes.emplace_back(); }

Trie::~Trie(){
	nodes.clear();
}

int Trie::size(){
	return sz;
}

int Trie::create(string _t = "", string _v = ""){
	nodes.emplace_back(_t, _v);
	return idx++;
}

void Trie::insert(string s, string val){
	int i = 0, n = s.size();
	int cur = 0;
	while(i < n){
		int j = 0, m = nodes[cur].t.size();
		while(j < m and i < n and nodes[cur].t[j] == s[i])
			i++, j++;

		if(j == m){
			if(i == n){
				if(nodes[cur].end == false) nodes[cur].end = true, nodes[cur].v = val, sz++;
				else nodes[cur].v = val;
			}
			else if(s[i] == '0'){
				if(nodes[cur].l == -1) nodes[cur].l = create(s.substr(i, n-i)), nodes[nodes[cur].l].par = cur;
				cur = nodes[cur].l;
			} else{
				if(nodes[cur].r == -1) nodes[cur].r = create(s.substr(i, n-i)), nodes[nodes[cur].r].par = cur;
				cur = nodes[cur].r;
			}
		} else{
			if(i == n){
				if(nodes[cur].t[j] == '0'){
					nodes[cur].l = create(nodes[cur].t.substr(j, m-j), nodes[cur].v);
					nodes[nodes[cur].l].par = cur;
				}
				else{
					nodes[cur].r = create(nodes[cur].t.substr(j, m-j), nodes[cur].v);
					nodes[nodes[cur].r].par = cur;
				}

				nodes[cur].t = nodes[cur].t.substr(0, j);
				nodes[cur].v = val, sz++;
			} else{
				int new_par = create(nodes[cur].t.substr(0, j));
				nodes[new_par].par = nodes[cur].par;

				if(nodes[cur].t[0] == '0') nodes[nodes[cur].par].l = new_par;
				else nodes[nodes[cur].par].r = new_par;

				nodes[cur].t = nodes[cur].t.substr(j, m-j);
				nodes[cur].par = new_par;

				if(s[i] == '0'){
					nodes[new_par].l = create(s.substr(i, n-i));
					nodes[new_par].r = cur;
					cur = nodes[new_par].l;
				} else{
					nodes[new_par].r = create(s.substr(i, n-i));
					nodes[new_par].l = cur;
					cur = nodes[new_par].r;
				}

				nodes[cur].par = new_par;
			}
		}
	}
}

void Trie::erase(string s){
	int i = 0, n = s.size();
	int cur = 0;
	while(i < n){
		int j = 0, m = nodes[cur].t.size();
		while(j < m and i < n and nodes[cur].t[j] == s[i])
			i++, j++;

		if(j != m) return;

		if(i == n){
			if(nodes[cur].end == false) return;
		}
		else if(s[i] == '0' and nodes[cur].l != -1) cur = nodes[cur].l;
		else if(s[i] == '1' and nodes[cur].r != -1) cur = nodes[cur].r;
		else return;
	}

	nodes[cur].end = false, nodes[cur].v = "", sz--;
	while(cur and nodes[cur].l == -1 and nodes[cur].r == -1 and nodes[cur].end == false){
		int tmp = cur;
		cur = nodes[cur].par;
		if(nodes[cur].l == tmp) nodes[cur].l = -1;
		if(nodes[cur].r == tmp) nodes[cur].r = -1;
	}
}

string Trie::find(string s){
	int i = 0, n = s.size();
	int cur = 0;
	while(i < n){
		int j = 0, m = nodes[cur].t.size();
		while(j < m and i < n and nodes[cur].t[j] == s[i])
			i++, j++;

		if(j != m) break;

		if(i == n) return (nodes[cur].end == false ? "" : nodes[cur].v);
		else if(s[i] == '0' and nodes[cur].l != -1) cur = nodes[cur].l;
		else if(s[i] == '1' and nodes[cur].r != -1) cur = nodes[cur].r;
		else break;
	}
	return "";
}

void Trie::clear(){
	nodes.clear();
    nodes.emplace_back();
	sz = 0, idx = 1;
}
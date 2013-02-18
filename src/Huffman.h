
#ifndef __HUFFMAN_H
#define __HUFFMAN_H

#include <iostream>
#include <string>
#include <cmath>

using namespace std;

class HNode
{
	string str;		// sottostringa che il nodo rappresenta
	string path;	// percorso a partire dalla radice (in binario)
	HNode* left;	// riferimento a figlio sinistro
	HNode* right;	// riferimento a figlio destor
	HNode* father;	// riferimento a padre
	int grade;		// frequenza della sottostringa all'interno della stringa da codificare

public:
	HNode()								{}
	HNode(string s, int g)				{ str = s; grade = g; right = left = NULL; }
	HNode(string s, HNode *l, HNode *r)	{ str = s; grade = 0; setLeft(l); setRight(r); path = "";}
	
	HNode* getLeft()	{ return left; }
	HNode* getRight()	{ return right; }
	HNode* getFather()	{ return father; }
	int getGrade()		{ return grade; }
	int getBitsCount()	{ return grade*path.size(); }
	string getString()	{ return str; }
	string getPath()	{ return path; }
	
	void setLeft(HNode *n)		{ left = n; grade+=n->getGrade(); n->setFather(this); }
	void setRight(HNode *n)		{ right = n; grade+=n->getGrade(); n->setFather(this); }
	void setFather(HNode *n)	{ father = n; }
	void setPath(string s)		{ path = s; }
};

ostream &operator<<(ostream &out, HNode &h);
string getUniqueChars(string input);
int* getCharFrequency(string input);
void SortByGrade(HNode *nodes, int x, int y);
void encoding(HNode *n);
float Shannon(HNode *nodes, int charsCount, int N);
void HuffmanCoding(string input);

#endif

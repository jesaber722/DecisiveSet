#include<vector>

#pragma once
#ifndef DecisiveSet_H
#define DecisiveSet_H

class DecisiveSet
{
private:
public:
	int * m_combos;
	int m_numDeciders;
	unsigned int* m_minimal_set;
	int m_min_set_size;
	int m_min_set_capacity;
	unsigned int m_size;
	static int count(unsigned int n);
	static int implies(unsigned int n, unsigned int m);
public:
	int combo_order_count[32];
	DecisiveSet();
	DecisiveSet(int numDeciders);
	DecisiveSet(int combos [], int size);
	static void make_str(unsigned int combo, char* str);
	void make_set_str(char* str);
	void make_min_set_str(char* str);
	void set_minimal_set();
	//~DecisiveSet();
};
#endif


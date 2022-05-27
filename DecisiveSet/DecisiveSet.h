#include<vector>

#pragma once
#ifndef DecisiveSet_H
#define DecisiveSet_H

class DecisiveSet
{
private:
	const int * m_combos;
	int m_numDeciders;
public:
	DecisiveSet(bool combos [], int size);
};
#endif


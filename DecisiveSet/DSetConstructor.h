#pragma once

#include "DecisiveSet.h"

#ifndef D_SET_CONSTRUCTOR_H
#define D_SET_CONSTRUCTOR_H

#define INCLUD 1
#define EXCLUD 0
#define UNDET 2

namespace states{

	enum Enum {
		INCLUDED,
		EXCLUDED,
		UNDETERMINED
	};
}

class DSetConstructor
{
	
private:
public:
	int m_numDeciders;
	unsigned int m_size;
	int * m_combos;
	unsigned int invert(unsigned int combo);
	void set(unsigned int combo, bool present);
	void selfApplyFalse(unsigned int combo);
	void selfApplyTrue(unsigned int combo);

public:
	unsigned int getSize();
	int getNumDeciders();
	int get(unsigned int combo);
	DSetConstructor(int numDeciders);
	void clone_onto(DSetConstructor* constr);
	//DSetConstructor(DSetConstructor * orig);
	bool isDetermined();
	void apply(DSetConstructor* dset, unsigned int combo, bool present);
	void produce(DecisiveSet* set);
	void produce_minimum(DecisiveSet* set);
	//~DSetConstructor();
};
#endif // !D_SET_CONSTRUCTOR_H
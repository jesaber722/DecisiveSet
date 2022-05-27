#pragma once
#ifndef D_SET_CONSTRUCTOR_H
#define D_SET_CONSTRUCTOR_H


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
	int m_numDeciders;
	unsigned int m_size;
	states::Enum * m_combos;
	unsigned int invert(unsigned int combo);
	void set(unsigned int combo, bool present);
	void selfApplyFalse(unsigned int combo);

public:
	states::Enum get(unsigned int combo);
	DSetConstructor(int numDeciders);
	DSetConstructor clone();
	//DSetConstructor(DSetConstructor * orig);
	bool isDetermined();
	DSetConstructor apply(unsigned int combo, bool present);
};
#endif // !D_SET_CONSTRUCTOR_H
#include <stdlib.h>
#include <stdexcept>
#include <limits.h>
#include "DSetConstructor.h"

DSetConstructor::DSetConstructor(int numDeciders) {
	m_numDeciders = numDeciders;

	unsigned int size = 1;
	for (int i = 0; i < numDeciders; i++) {
		size *= 2;
	}
	m_size = size;
	m_combos = (states::Enum *) malloc(sizeof(states::Enum) * size);
	for (int i = 0; i < size; i++) {
		*(m_combos + i) = states::UNDETERMINED;
	}
}

void DSetConstructor::set(unsigned int combo, bool present) {
	/*
	if (*(m_combos + combo) != states::INCLUDED && *(m_combos + combo) != states::EXCLUDED) {
		throw std::exception("cannot set");
	}
	*/
	if()
	if (present) {
		*(m_combos + combo) = states::INCLUDED;
	}
	else {
		*(m_combos + combo) = states::EXCLUDED;
	}
}

states::Enum DSetConstructor::get(unsigned int combo) {
	if (combo >= m_size) {
		throw std::exception("out of bounds");
	}
	return *(m_combos + combo);
}

/*
DSetConstructor::DSetConstructor(DSetConstructor* orig) {
	m_numDeciders = orig->m_numDeciders;
	int size = 1;
	for (int i = 0; i < m_numDeciders; i++) {
		size *= 2;
	}
	m_size = size;
	m_combos = (states::Enum*)malloc(sizeof(states::Enum) * size);
	for (int i = 0; i < size; i++) {
		*(m_combos + i) = orig->get(i);
	}
}
*/

DSetConstructor DSetConstructor::clone() {
	DSetConstructor ret = DSetConstructor(this->m_numDeciders);
	
	for (int i = 0; i < ret.m_size; i++) {
		*(ret.m_combos + i) = this->get(i);
	}
	return ret;
}

bool DSetConstructor::isDetermined() {
	for (int i = 0; i < m_size; i++) {
		if (*(m_combos + i) == states::UNDETERMINED) {
			return false;
		}
		if (*(m_combos + i) != states::INCLUDED && *(m_combos + i) != states::EXCLUDED) {
			throw std::exception("corrupt data");
		}
	}
	return true;
}

unsigned int DSetConstructor::invert(unsigned int combo) {
	unsigned int s = 1;
	for (int i = 1; i < m_numDeciders; i++) {
		s = s << 1;
		s++;
	}
	return s ^ combo;
}


void DSetConstructor::selfApplyFalse(unsigned int combo) {
	unsigned int s = 1;
	int d_pres_size = 0;
	int deciders_present[40];
	unsigned int decider_value[40];
	//memset(deciders_present, 0, sizeof(deciders_present));

	for (int i = 0; i < m_numDeciders; i++) {
		if (s & combo) {
			deciders_present[d_pres_size] = i;
			decider_value[d_pres_size] = s;
			d_pres_size++;
		}
		s = s << 1;
	}
}

DSetConstructor DSetConstructor::apply(unsigned int combo, bool present) {
	DSetConstructor ret = this->clone();
	unsigned int s = UINT_MAX;
	int n;
}

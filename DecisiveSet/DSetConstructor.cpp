#include <stdlib.h>
#include <stdexcept>
#include <limits.h>
#include <iostream>
using namespace std;
#include "DSetConstructor.h"

DSetConstructor::DSetConstructor(int numDeciders) {
	m_numDeciders = numDeciders;

	unsigned int size = 1;
	for (int i = 0; i < numDeciders; i++) {
		size *= 2;
	}
	m_size = size;
	//cout << "size: " << size << "\n";
	m_combos = (int *) malloc(sizeof(int) * size);

	if (!m_combos) {
		fputs("Out of memory", stderr);
		exit(1);
	}

	for (unsigned int i = 0; i < size; i++) {
		*(m_combos + i) = UNDET;
	}

	/*
	for (unsigned int i = 0; i < size; i++) {
		cout << "Value of " << (i) << ": " << *(m_combos + i) << "\n";
	}
	*/
}

void DSetConstructor::set(unsigned int combo, bool present) {

	int v = present ? INCLUD : EXCLUD;
	if (*(m_combos + combo) != v && *(m_combos + combo) != UNDET) {
		//cout << "cannot set " << combo << "to" << present << ", current value of it is" << *(m_combos + combo) << "\n";
		fputs("Cannot set, something went wrong.", stderr);
		exit(1);
	}
	*(m_combos + combo) = v;
	return;
}

unsigned int DSetConstructor::getSize() {
	return m_size;
}

int DSetConstructor::getNumDeciders() {
	return m_numDeciders;
}

int DSetConstructor::get(unsigned int combo) {
	if (combo >= m_size) {
		fputs("out of bounds", stderr);
		exit(1);
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

void DSetConstructor::clone_onto(DSetConstructor* constr) {
	
	for (unsigned int i = 0; i < m_size; i++) {
		*(constr->m_combos + i) = this->get(i);
	}
}

bool DSetConstructor::isDetermined() {
	for (unsigned int i = 0; i < m_size; i++) {
		if (*(m_combos + i) == UNDET) {
			return false;
		}
		if (*(m_combos + i) != INCLUD && *(m_combos + i) != EXCLUD) {
			fputs("corrupt data", stderr);
			exit(1);
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
	//int deciders_present[33];
	unsigned int decider_value[33];
	//memset(deciders_present, 0, sizeof(deciders_present));

	for (int i = 0; i < m_numDeciders; i++) {
		if (s & combo) {
			//deciders_present[d_pres_size] = i;
			decider_value[d_pres_size] = s;
			d_pres_size++;
		}
		s = s << 1;
	}

	bool combo_looper[33];
	memset(combo_looper, false, (d_pres_size + 1) * (sizeof(bool)));
	while(true) {
		unsigned int combo = 0;
		for (int i = 0; i < d_pres_size; i++) {
			if (combo_looper[i]) {
				combo |= decider_value[i];
			}
		}
		//cout << "Setting " << combo << " to false\n";
		set(combo, false);

		bool exit = true;
		for (int i = 0; i < d_pres_size; i++) {
			if (combo_looper[i] == false) {
				exit = false;
				break;
			}
		}
		if (exit) {
			break;
		}

		{
			int j = 0;
			while (combo_looper[j] == true) {
				combo_looper[j] = false;
				j++;
			}
			combo_looper[j] = true;
		}
	}
}

void DSetConstructor::selfApplyTrue(unsigned int combo) {

	/*
	//DEBUG
	cout << "self applying true";

	for (unsigned int i = 0; i < m_size; i++) {
		cout << "Value of " << (i) << ": " << *(m_combos + i) << "\n";
	}
	*/
	unsigned int s = 1;
	int d_pres_size = 0;
	//int deciders_present[33];
	unsigned int decider_value[33];
	//memset(deciders_present, 0, sizeof(deciders_present));

	for (int i = 0; i < m_numDeciders; i++) {
		//cout << "andifying " << s << " and " << combo << " gives " << (s & combo) << "\n";
		if (!(s & combo)) {
			//cout << s << " is present! yay!\n";
			//deciders_present[d_pres_size] = i;
			decider_value[d_pres_size] = s;
			d_pres_size++;
		}
		s = (s << 1);
	}

	bool combo_looper[33];
	memset(combo_looper, false, (d_pres_size + 1) * (sizeof(bool)));
	while (true) {
		unsigned int comb = combo;
		for (int i = 0; i < d_pres_size; i++) {
			if (combo_looper[i]) {
				comb |= decider_value[i];
			}
		}
		//cout << "Setting " << comb << " to true\n";
		set(comb, true);

		bool exit = true;
		for (int i = 0; i < d_pres_size; i++) {
			if (combo_looper[i] == false) {
				exit = false;
				break;
			}
		}
		if (exit) {
			break;
		}

		{
			int j = 0;
			while (combo_looper[j] == true) {
				combo_looper[j] = false;
				j++;
			}
			combo_looper[j] = true;
		}
	}
	selfApplyFalse(invert(combo));
}

void DSetConstructor::DSetConstructor::apply(DSetConstructor* ret, unsigned int combo, bool present) {
	this->clone_onto(ret);
	//cout << "post clone";
	if (present) {
		ret->selfApplyTrue(combo);
	}
	else {
		ret->selfApplyFalse(combo);
	}
}

void DSetConstructor::produce(DecisiveSet* set) {
	if (!isDetermined()) {
		fputs("not determined", stderr);
	}

	memset(set->combo_order_count, 0, sizeof(int) * 32);
	set->m_size = m_size;
	set->m_numDeciders = m_numDeciders;
	set->m_minimal_set = 0;
	set->m_min_set_capacity = 0;
	set->m_min_set_size = 0;
	for (unsigned int i = 0; i < m_size; i++) {
		*(set->m_combos + i) = *(m_combos + i);
		if (*(m_combos + i)) {
			set->combo_order_count[set->count(i)]++;
		}
	}
}

void DSetConstructor::produce_minimum(DecisiveSet* set) {
	memset(set->combo_order_count, 0, sizeof(int) * 32);
	set->m_size = m_size;
	set->m_numDeciders = m_numDeciders;
	set->m_minimal_set = 0;
	set->m_min_set_capacity = 0;
	set->m_min_set_size = 0;
	for (unsigned int i = 0; i < m_size; i++) {

		if (*(m_combos + i) == 1) {
			*(set->m_combos + i) = 1;
			set->combo_order_count[set->count(i)]++;
		}
		else {
			*(set->m_combos + i) = 0;
		}
	}
}

/*
DSetConstructor::~DSetConstructor() {
	//free(m_combos);
}
*/

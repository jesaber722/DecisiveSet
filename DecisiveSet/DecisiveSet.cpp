#include <stdlib.h>
#include <stdexcept>
#include <iostream>
using namespace std;
#include "DecisiveSet.h"

DecisiveSet::DecisiveSet() {
	memset(combo_order_count, 0, sizeof(int) * 32);
	m_numDeciders = -1;
	m_combos = 0;
	m_minimal_set = 0;
	m_min_set_capacity = 0;
	m_min_set_size = 0;
}

DecisiveSet::DecisiveSet(int numDeciders) {
	m_size = 1;
	for (int i = 0; i < numDeciders; i++) {
		m_size *= 2;
	}
	memset(combo_order_count, 0, sizeof(int) * 32);
	m_numDeciders = numDeciders;
	m_combos = (int*)malloc(sizeof(int) * m_size);
	m_minimal_set = 0;
	m_min_set_capacity = 0;
	m_min_set_size = 0;
}

DecisiveSet::DecisiveSet(int combos [], int size) {
	if (size <= 0) {
		fputs("bad size value", stderr);
		exit(1);
	}
	int numDeciders = 0;
	m_size = size;
	while (size != 1) {
		if (size % 2 != 0) {
			fputs("bad size value", stderr);
			exit(1);
		}
		numDeciders++;
		size /= 2;
	}
	memset(combo_order_count, 0, sizeof(int) * 32);
	m_numDeciders = numDeciders;
	m_combos = (int *) malloc(sizeof(int) * size);

	if (!m_combos) {
		fputs("Out of memory", stderr);
		exit(1);
	}
	

	for (unsigned int i = 0; i < m_size; i++) {
		//DEBUG
		//cout << "combos[" << i << "]:" << combos[i] << "\n";
		if (combos[i]) {
			*(m_combos + i) = 1;
			combo_order_count[count(i)]++;
		}
		else {
			*(m_combos + i) = 0;
		}
	}
	
	m_minimal_set = 0;
	m_min_set_capacity = 0;
	m_min_set_size = 0;
	//set_minimal_set();
}

void DecisiveSet::set_minimal_set() {
	m_min_set_capacity = std::max(m_size / 16, (unsigned int) 16);
	m_min_set_size = 0;
	m_minimal_set = (unsigned int*)malloc(sizeof(int) * m_min_set_capacity * 1000);
	for (int i = 0; i <= m_numDeciders; i++) {
		for (unsigned int j = 0; j < m_size; j++) {
			if (count(j) != i || !*(m_combos + j)) {
				continue;
			}

			bool add = true;

			for (int k = 0; k < m_min_set_size; k++) {
				if (implies(*(m_minimal_set + k), j)) {
					add = false; // do not add this j to the min set
					break;
				}
			}

			if (add) {
				if (m_min_set_size == m_min_set_capacity) {
					m_min_set_capacity *= 2;
					m_minimal_set = (unsigned int*)realloc(m_minimal_set, m_min_set_capacity);
					if (!m_minimal_set) {
						fputs("Out of memory", stderr);
						exit(1);
					}
				}
				*(m_minimal_set + m_min_set_size) = j;
				m_min_set_size++;
			}
		}
	}
}

int DecisiveSet::count(unsigned int n) {
	int count = 0;
	while (n) {
		if (n % 2 != 0) {
			count++;
		}
		n = n / 2;
	}
	return count;
}

int DecisiveSet::implies(unsigned int n, unsigned int m) {
	return (n & m) == n;
}

void DecisiveSet::make_str(unsigned int combo, char* str) {
	char t = 65;
	//int i = 0;
	while (combo) {
		if (combo % 2 != 0) {
			*str = t;
			str++;
		}
		combo = combo / 2;
		t++;
	}
	*str = 0;
}

void DecisiveSet::make_set_str(char* str) {
	*str = 123; // curly brace
	str++;
	for(unsigned int i = 0; i < m_size; i++) {
		if (!m_combos[i]) {
			cout << "continuing on " << i << "\n";
			continue;
		}
		unsigned int combo = i;
		char t = 65;
		while (combo) {
			if (combo % 2 != 0) {
				*str = t;
				str++;
			}
			combo = combo / 2;
			t++;
		}
		*str = 44; // comma
		str++;
	}
	str--;
	*str = 125; // ending curly brace
	str++;
	*str = 0; // nul termination
}

void DecisiveSet::make_min_set_str(char* str) {
	if (!m_minimal_set) {
		fputs("min set uninitialized", stderr);
		exit(1);
	}
	*str = 123; // curly brace
	if (m_min_set_size == 0) {
		*(str + 1) = 125;
		*(str + 2) = 0;
		return;
	}
	str++;
	for (int i = 0; i < m_min_set_size; i++) {
		unsigned int combo = *(m_minimal_set + i);
		char t = 65;
		while (combo) {
			if (combo % 2 != 0) {
				*str = t;
				str++;
			}
			combo = combo / 2;
			t++;
		}
		*str = 44; // comma
		str++;
	}
	str--;
	*str = 125; // ending curly brace
	str++;
	*str = 0; // nul termination
}

/*
DecisiveSet::~DecisiveSet() {
	//if(m_combos)
		//free(m_combos);
	//if (m_minimal_set)
		//free(m_minimal_set);
}
*/
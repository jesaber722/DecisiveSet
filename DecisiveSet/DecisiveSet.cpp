#include <stdlib.h>
#include <stdexcept>
#include "DecisiveSet.h"


DecisiveSet::DecisiveSet(bool combos [], int size) {
	if (size <= 0) {
		throw std::invalid_argument("bad size value");
	}
	int numDeciders = 0;
	while (size != 1) {
		if (size % 2 != 0) {
			throw std::invalid_argument("bad size value");
		}
		numDeciders++;
		size /= 2;
	}
	m_numDeciders = numDeciders;
	m_combos = NULL;
}

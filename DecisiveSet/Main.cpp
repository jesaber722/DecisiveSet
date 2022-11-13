#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
#include "DecisiveSet.h"
#include "DSetConstructor.h"

char buf[8192];
unsigned long count_sets = 0;
int cycle_thru_perms[32][32];
unsigned long size_divisors[32];

void print_perm(int perm[32], int size) {
	for (int i = 0; i < size; i++) {
		cout << i << ":" << perm[i] << "\n";
	}
}

void multiply_perms(int b[32], int a[32], int size, int dest[32]) {
	for (int i = 0; i < size; i++) {
		dest[i] = b[a[i]];
	}
}

void init_size_divisors() {
	for (int i = 0; i < 32; i++) {
		if (i < 2) {
			size_divisors[i] = 1;
		} else {
			size_divisors[i] = i * size_divisors[i - 1];
		}
	}
}

void init_cycle_thru_perms() {
	int current_perm[32];

	for (int j = 0; j < 32; j++) {
		current_perm[j] = j;
	}

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 32; j++) {
			if (j < i - 1) {
				current_perm[j] = j + 1;
			}
			else if (j == i - 1) {
				current_perm[j] = 0;
			}
			else {
				current_perm[j] = j;
			}
		}
		if (i == 0) {
			for (int j = 0; j < 32; j++) {
				cycle_thru_perms[i][j] = current_perm[j];
			}
		}
		else {
			multiply_perms(current_perm, cycle_thru_perms[i - 1], 32, cycle_thru_perms[i]);
		}
		/*
		cout << "#" << i << ":\n";
		for (int k = 0; k < 8; k++) {
			cout << k << " to " << cycle_thru_perms[i][k] << "\n";
		}
		*/
	}
}

int map_combo_masks[32];
bool map_combo_masks_initialized = false;

void init_map_combo_masks() {
	map_combo_masks_initialized = true;
	int index = 0;
	unsigned int value = 1;
	while (index < 32) {
		map_combo_masks[index] = value;
		index++;
		value *= 2;
	}
}

unsigned int map_combo(unsigned int combo, int perm[32]) {
	if (!map_combo_masks_initialized) {
		puts("map combo masks uninitialized");
		exit(1);
	}
	unsigned int ret = 0;
	for (int i = 0; i < 32; i++) {
		//cout << "current ret: " << ret << "     ";
		//cout << "current i" << i << "     ";
		//cout << "current perm[i]: " << perm[i] << "\n";
		if (combo % 2 == 1) {
			//cout << "oring with map_combo_masks[perm[i]] which is " << map_combo_masks[perm[i]];
			ret |= map_combo_masks[perm[i]];
		}
		combo /= 2;
	}
	return ret;
}

bool mapping_works(DecisiveSet a, DecisiveSet b, int perm[32]) {
	for (unsigned int i = 0; i < a.m_size; i++) {
		//cout << map_combo(i, perm) << "mapped combo\n";
		//cout << map_combo(i, perm) << "\n";
		//cout << map_combo(i, perm) << "\n";
		//cout << i << " combo status: " << a.m_combos[i] << "a " << b.m_combos[map_combo(i, perm)] << "b\n";
		if (a.m_combos[i] && !b.m_combos[map_combo(i, perm)]) {
			return false;
		}
	}
	return true;
}

bool is_isomorphic(DecisiveSet a, DecisiveSet b) {
	//a.make_min_set_str(buf);
	//cout << "testing if " << buf << " and ";
	//b.make_min_set_str(buf);
	//cout << buf << " are isomorphic\n";
	if (a.m_numDeciders != b.m_numDeciders) {
		return false;
	}
	for (int i = 0; i <= a.m_numDeciders; i++) {
		//cout << i << " order combo counts: " << a.combo_order_count[i] << " vs " << b.combo_order_count[i] << "\n";
		if (a.combo_order_count[i] != b.combo_order_count[i]) {
			return false;
		}
	}

	int test_perm [32];
	int cycle_perm[32];
	int buffer_perm[32];
	for (int i = 0; i < 32; i++) {
		test_perm[i] = i;
	}
	unsigned long i = 0;
	while( i < size_divisors[a.m_numDeciders]) {

		//cout << "perm number: " << i << "\n";
		//cout << "perm: \n";
		//print_perm(test_perm, a.m_numDeciders);

		if (mapping_works(a, b, test_perm)) {
			//cout << "found a maps to b under this permutation.\n";
			return true;
		}

		i++;
		int largest_divisor_index = a.m_numDeciders;
		while (i % size_divisors[largest_divisor_index] != 0) {
			largest_divisor_index--;
		}
		for (int j = 0; j < 32; j++) {
			cycle_perm[j] = cycle_thru_perms[largest_divisor_index + 1][j];
		}
		multiply_perms(cycle_perm, test_perm, 32, buffer_perm);
		for (int j = 0; j < 32; j++) {
			test_perm[j] = buffer_perm[j];
		}
	}
	return false;
}

DecisiveSet unique_sets[8000];
int unique_count = 0;


void debug_print_unq_combo() {
	cout << "unique set debug: \n";
	for (int i = 0; i < unique_count; i++) {
		cout << "number " << i << ": "<< unique_sets[i].m_combos[1];
	}
	cout << "\n";
}

void iterate_upto_isomorphism(DSetConstructor constr) {
	if (constr.isDetermined()) {
		//debug_print_unq_combo();
		DecisiveSet set = DecisiveSet(constr.getNumDeciders());
		constr.produce(&set);
		set.set_minimal_set();
		/*
		//DEBUG
		for (int i = 0; i < set.m_size; i++) {
			cout << set.m_combos[i];
		}
		cout << "yeah\n";

		for (int i = 0; i < set.m_min_set_size; i++) {
			cout << set.m_minimal_set[i] << "\n";
		}
		*/
		set.make_min_set_str(buf);

		bool skip = false;
		for (int i = 0; i < unique_count; i++) {
			if (is_isomorphic(set, unique_sets[i])) {
				skip = true;
				break;
			}
		}
		if (!skip) {
			printf("%s\n", buf);
			unique_sets[unique_count].m_combos = (int*) malloc(sizeof(int) * set.m_size);
			if (!unique_sets[unique_count].m_combos) {
				fputs("out of memory", stderr);
			}

			// unique_sets[unique_count] = set;
			//
			memcpy(unique_sets[unique_count].m_combos, set.m_combos, sizeof(int) * set.m_size);
			memcpy(unique_sets[unique_count].combo_order_count, set.combo_order_count, sizeof(int) * 32);
			unique_sets[unique_count].m_size = set.m_size;
			unique_sets[unique_count].m_numDeciders = set.m_numDeciders;
			unique_sets[unique_count].m_minimal_set = 0;
			unique_sets[unique_count].m_min_set_capacity = 0;
			unique_sets[unique_count].m_min_set_size = 0;
			unique_sets[unique_count].set_minimal_set();
			//
			unique_count++;
		}
		free(set.m_combos);
		free(set.m_minimal_set);
		//printf("%s\n", buf);
		return;
	}
	else {
		unsigned int combo = 0;
		char det = 0;
		for (unsigned int i = 0; i < constr.getSize(); i++) {
			if (constr.get(i) == UNDET) {
				combo = i;
				det = 1;
				break;
			}
		}
		if (!det) {
			puts("something oopsed when iterating.");
			exit(1);
		}
		{
			DSetConstructor new_constr = DSetConstructor(constr.getNumDeciders());
			constr.apply(&new_constr, combo, true);
			iterate_upto_isomorphism(new_constr);
			free(new_constr.m_combos);
		}

		{
			DSetConstructor new_constr = DSetConstructor(constr.getNumDeciders());
			constr.apply(&new_constr, combo, false);
			iterate_upto_isomorphism(new_constr);
			free(new_constr.m_combos);
		}

	}
}

void iterate_through_all(DSetConstructor constr) {
	if (constr.isDetermined()) {
		DecisiveSet set = DecisiveSet(constr.getNumDeciders());
		constr.produce(&set);
		set.set_minimal_set();
		/*
		//DEBUG
		for (int i = 0; i < set.m_size; i++) {
			cout << set.m_combos[i];
		}
		cout << "yeah\n";
		
		for (int i = 0; i < set.m_min_set_size; i++) {
			cout << set.m_minimal_set[i] << "\n";
		}
		*/
		set.make_min_set_str(buf);
		free(set.m_combos);
		free(set.m_minimal_set);
		printf("%s\n", buf);
		count_sets++;
		return;
	}
	else {
		unsigned int combo = 0;
		char det = 0;
		for (unsigned int i = 0; i < constr.getSize(); i++) {
			if (constr.get(i) == UNDET) {
				combo = i;
				det = 1;
				break;
			}
		}
		if (!det) {
			puts("something oopsed when iterating.");
			exit(1);
		}
		{
			DSetConstructor new_constr = DSetConstructor(constr.getNumDeciders());
			constr.apply(&new_constr, combo, true);
			iterate_through_all(new_constr);
			free(new_constr.m_combos);
		}

		{
			DSetConstructor new_constr = DSetConstructor(constr.getNumDeciders());
			constr.apply(&new_constr, combo, false);
			iterate_through_all(new_constr);
			free(new_constr.m_combos);
		}

	}
}

void debug1() {
	DSetConstructor constr = DSetConstructor(4);
	DSetConstructor constr2 = DSetConstructor(4);

	constr.apply(&constr2, 1, true);
	//constr = constr.apply(3, true);
	for (unsigned int i = 0; i < constr2.m_size; i++) {
		cout << "constr2[" << i << "]: " << constr2.get(i);
	}

	DecisiveSet set = DecisiveSet(4);
	constr2.produce_minimum(&set);
	puts("out of produce_minimum");
	char buff[400];
	puts("print set string");
	set.make_set_str(buff);
	printf("%s\n", buff);
	set.set_minimal_set();
	puts("print min set string");
	set.make_min_set_str(buff);
	printf("%s\n", buff);
	puts("");
	puts("starting iteratification... ");
	DSetConstructor iterate = DSetConstructor(4);
	iterate.selfApplyFalse(0);
	iterate_through_all(iterate);
	cout << "count: " << count_sets << "\n";
}

void debug2() {
	DSetConstructor iterate = DSetConstructor(5);
	iterate.selfApplyFalse(0);
	iterate_upto_isomorphism(iterate);
	cout << "unique count: " << unique_count << "\n";
}

void debug3() {
	init_cycle_thru_perms();
	init_size_divisors();
	init_map_combo_masks();
	DSetConstructor iterate = DSetConstructor(6);
	iterate.selfApplyFalse(0);
	iterate_upto_isomorphism(iterate);
	cout << "done\n";
	cout << "unique count: " << unique_count << "\n";
}

int main() {
	//debug1();
	//debug2();
	//debug3();
	init_cycle_thru_perms();
	init_size_divisors();
	init_map_combo_masks();
	int size;
	cout << "What size decisive set do you wish to calculate?\n";
	scanf_s("%d", &size);
	DSetConstructor iterate = DSetConstructor(size);
	iterate.selfApplyFalse(0);
	iterate_upto_isomorphism(iterate);
	cout << "unique count: " << unique_count << "\n";
	return 0;
}
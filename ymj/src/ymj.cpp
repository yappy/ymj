#include <ymj.h>
#include <cassert>
#include <iterator>
#include <numeric>

/* private variables and functions */
namespace {

void push_mentu(ymj_state *state, uint8_t start, uint8_t is_kotu)
{
	assert(state->mentu_count < YMJ_MENTU_SIZE);
	ymj_mentu *m = &state->mentu[state->mentu_count];
	m->start = start;
	m->is_kotu = is_kotu;
	state->mentu_count++;
}

void for_each_hora(const ymj_state *state, uint8_t hai_offset,
	ymj_hora_callback callback)
{
	if (state->head != 0 && state->mentu_count == YMJ_MENTU_SIZE) {
		callback(state);
		return;
	}

	// copy to my local stack frame
	ymj_state work = *state;

	if (work.head == 0) {
		for (uint8_t hai = 0; hai < YMJ_COUNT_TABLE_SIZE; hai++) {
			if (work.hai_count[hai] >= 2) {
				work.head = hai;
				work.hai_count[hai] -= 2;
				// recursive call and restore
				for_each_hora(&work, hai_offset, callback);
				work = *state;
			}
		}
	}
	else {
		for (uint8_t hai = hai_offset; hai < YMJ_COUNT_TABLE_SIZE; hai++) {
			// find minimum hai id
			if (work.hai_count[hai] <= 0) {
				continue;
			}
			// hai_count[hai] (> 0) must be consumed
			bool can_use = false;
			// ko-tu
			if (work.hai_count[hai] >= 3) {
				can_use = true;
				work.hai_count[hai] -= 3;
				push_mentu(&work, hai, 1);
				// recursive call and restore
				for_each_hora(&work, hai, callback);
				work = *state;
			}
			// shun-tu
			if (ymj_is_suhai(hai) && ymj_hai_number(hai) <= 7) {
				if (work.hai_count[hai + 1] > 0 &&
					work.hai_count[hai + 2] > 0) {
					can_use = true;
					work.hai_count[hai]--;
					work.hai_count[hai + 1]--;
					work.hai_count[hai + 2]--;
					push_mentu(&work, hai, 0);
					// recursive call and restore
					for_each_hora(&work, hai, callback);
					work = *state;
				}
			}
			// give up if this hai cannot be used
			if (!can_use) {
				break;
			}
		}
	}
}

}	// namespace

/* exported functions */
int ymj_for_each_hora(const ymj_state *state, ymj_hora_callback callback)
{
	uint32_t sum = 0;
	// head_count * 2
	if (state->head != 0) {
		sum += 2;
	}
	// mentu_count * 3
	sum += state->mentu_count * 3;
	// not decided hai
	for (uint8_t hai = 0; hai < YMJ_COUNT_TABLE_SIZE; hai++) {
		// invalid hai id check
		if (state->hai_count[hai] > 0 && !ymj_is_valid_hai(hai)) {
			return 0;
		}
		// add
		sum += state->hai_count[hai];
	}
	// must be 14
	if (sum != YMJ_HAND_SIZE) {
		return 0;
	}

	// OK; call internal function
	for_each_hora(state, 0, callback);
	return 1;
}

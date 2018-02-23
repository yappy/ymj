#include <ymj.h>
#include <array>
#include <cstring>
#include <cstdio>

using Hand = std::array<uint8_t, YMJ_HAND_SIZE>;

void callback(const ymj_state *state)
{
	std::printf("(%2d)", state->head);
	for (int i = 0; i < 4; i++) {
		std::printf("(%c %d)",
			state->mentu[i].is_kotu ? 'k' : 's',
			state->mentu[i].start);
	}
	std::putchar('\n');
}

void create_state(ymj_state *state, const Hand &hand)
{
	std::memset(state, 0, sizeof(*state));
	for (const auto &hai : hand) {
		state->hai_count[hai]++;
	}
}

int main()
{
	ymj_state state;
	Hand hand {1,1,1,2,2,2,3,3,3,4,4,4,5,5};
	create_state(&state, hand);

	int ret = ymj_for_each_hora(&state, callback);
	std::printf("ymj_for_each_hora(): %d\n", ret);

	return 0;
}

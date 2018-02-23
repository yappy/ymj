#pragma once

#include <stdint.h>

#if __cplusplus >= 201103L
#	define YMJ_ENUM	enum class
#else
#	define YMJ_ENUM	enum
#endif

/*
 * Encoding
 * "kn" in decimal (k * 10 + n)
 *
 * k
 * 0:m, 1:p, 2:s, 3:z
 * n
 * 1-9(mps) or 1-7(z)
 *
 * "00" is for invalid or null state.
 */

/* hand (tehai) size */
static const int YMJ_HAND_SIZE = 14;
static const int YMJ_MENTU_SIZE = 4;

/* Max is table[37], and 8-align */
static const uint8_t YMJ_COUNT_TABLE_SIZE = 40;

static inline uint8_t ymj_hai_kind(uint8_t hai)
{
	return hai / 10;
}

static inline uint8_t ymj_hai_number(uint8_t hai)
{
	return hai % 10;
}

static inline int ymj_is_zihai(uint8_t hai)
{
	return ymj_hai_kind(hai) == 3;
}

static inline int ymj_is_suhai(uint8_t hai)
{
	return !ymj_is_zihai(hai);
}

static inline int ymj_is_valid_hai(uint8_t hai)
{
	switch (ymj_hai_kind(hai)) {
	case 0:
	case 1:
	case 2:
		return ymj_hai_number(hai) >= 1 && ymj_hai_number(hai) <= 9;
	case 3:
		return ymj_hai_number(hai) >= 1 && ymj_hai_number(hai) <= 7;
	default:
		return 0;
	}
}

/* Types */
typedef struct {
	uint8_t		start;
	uint8_t		is_kotu;
	uint8_t		is_kantu;
	uint8_t 	padding;
} ymj_mentu;

typedef struct {
	/* hai_count[15] == 3 means you have 5p x3. */
	uint8_t		hai_count[YMJ_COUNT_TABLE_SIZE];
	/* 0 means not decided yet */
	uint32_t	head;
	/* decided mentu */
	uint32_t	mentu_count;
	ymj_mentu	mentu[YMJ_MENTU_SIZE];
} ymj_state;

/* Functions */
#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

typedef void (*ymj_hora_callback)(const ymj_state *state);

int ymj_for_each_hora(const ymj_state *state, ymj_hora_callback callback);

#ifdef __cplusplus
}
#endif	/* __cplusplus */

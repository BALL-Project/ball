// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_COMMON_MACROS_H
#define BALL_COMMON_MACROS_H

#include <BALL/CONFIG/config.h>
#include <BALL/COMMON/constants.h>
#include <BALL/COMMON/rtti.h>

#include <chrono>    // std::chrono::seconds
#include <math.h>    // needed for fabs
#include <thread>    // std::this_thread::sleep_for
#include <typeinfo>  // needed for typeid

namespace BALL
{
	template<class X>
	int BALL_DEPRECATED BALL_INLINE ball_macro_sgn(X x)
	{
		return (x < 0 ? -1 : (x == 0) ? 0 : 1);
	}

	template<class X>
	bool BALL_DEPRECATED BALL_INLINE ball_macro_odd(X x)
	{
		return (x % 2) != 0;
	}

	template<class X>
	bool BALL_DEPRECATED BALL_INLINE ball_macro_int_odd(X x)
	{
		return (x & 0x1) == 1;
	}

	template<class X>
	bool BALL_DEPRECATED BALL_INLINE ball_macro_int_even(X x)
	{
		return (x & 0x1) == 0;
	}

	template<class X>
	X BALL_DEPRECATED BALL_INLINE ball_macro_real_round(X x)
	{
		return (X)(x > 0 ? long(x + 0.5) : long(x - 0.5));
	}

	template<class X, class Y, class E>
	bool BALL_DEPRECATED BALL_INLINE ball_macro_real_not_equal(X x, Y y, E e)
	{
		return fabs(x - y) > e;
	}

	template<class X, class Y, class E>
	bool BALL_DEPRECATED BALL_INLINE ball_macro_real_greater(X x, Y y, E e)
	{
		return (x - y) > e;
	}

	template<class X>
	X BALL_DEPRECATED BALL_INLINE ball_macro_real_abs(X x)
	{
		return fabs(x);
	}

	template<class X>
	bool BALL_DEPRECATED BALL_INLINE ball_macro_real_odd(X x)
	{
		return fmod(x, 2) != 0;
	}

	template<class X>
	bool BALL_DEPRECATED BALL_INLINE ball_macro_real_even(X x)
	{
		return fmod(x, 2) == 0;
	}

	template<class X>
	long BALL_DEPRECATED BALL_INLINE ball_macro_real_floor(X x)
	{
		return (long)(x > 0 ? x : (x == (long)(x) ? x : x - 1));
	}

	template<class X>
	long BALL_DEPRECATED BALL_INLINE ball_macro_real_ceiling(X x)
	{
		return (long)(x < 0 ? x : (x == (long)(x) ? x : x + 1));
	}

	template<class X>
	int BALL_DEPRECATED BALL_INLINE ball_macro_real_round_int(X x)
	{
		return (x > 0 ? (int)(x + 0.5) : -(int)(0.5 - x));
	}

	template<class T>
	T BALL_DEPRECATED BALL_INLINE ball_macro_char_mask()
	{
		return T(BALL_CHAR_SIZE * 8) - 1;
	}

	template<class T>
	T BALL_DEPRECATED BALL_INLINE ball_macro_char_shift()
	{
		unsigned bits = BALL_CHAR_SIZE * 8;
		return T(bits == 8 ? 3 : (bits == 16) ? 4 : 5);
	}

	template<class T>
	T BALL_DEPRECATED BALL_INLINE ball_macro_all_bits_set()
	{
		return ~T(0);
	}

	template<class T>
	T BALL_DEPRECATED BALL_INLINE ball_macro_all_bits_cleared()
	{
		return T(0);
	}

	template<class T>
	T BALL_DEPRECATED BALL_INLINE ball_macro_number_of_bytes(T x)
	{
		unsigned bits  = BALL_CHAR_SIZE * 8;
		unsigned shift = (bits == 8 ? 3u : (bits == 16) ? 4u : 5u);
		return (x + bits - 1) >> shift;
	}

	template<class T>
	T BALL_DEPRECATED BALL_INLINE ball_macro_bitarray_size(T x)
	{
		unsigned bits  = BALL_CHAR_SIZE * 8;
		unsigned shift = (bits == 8 ? 3u : (bits == 16) ? 4u : 5u);
		return ((x - 1) >> shift) + 1;
	}

	template<class BitArray, class X>
	void BALL_DEPRECATED BALL_INLINE ball_macro_bitarray_clear_bit(BitArray* a, X x)
	{
		unsigned bits  = BALL_CHAR_SIZE * 8;
		unsigned shift = (bits == 8 ? 3u : (bits == 16) ? 4u : 5u);
		a[x >> shift] &= ~(1 << (x & (bits - 1)));
	}

	template<class BitArray, class X>
	void BALL_DEPRECATED BALL_INLINE ball_macro_bitarray_set_bit(BitArray* a, X x)
	{
		unsigned bits  = BALL_CHAR_SIZE * 8;
		unsigned shift = (bits == 8 ? 3u : (bits == 16) ? 4u : 5u);
		a[x >> shift] |= (1 << (x & (bits - 1)));
	}

	template<class BitArray, class X>
	void BALL_DEPRECATED BALL_INLINE ball_macro_bitarray_toggle_bit(BitArray* a, X x)
	{
		unsigned bits  = BALL_CHAR_SIZE * 8;
		unsigned shift = (bits == 8 ? 3u : (bits == 16) ? 4u : 5u);
		a[x >> shift] ^= (1 << (x & (bits - 1)));
	}

	template<class BitArray, class X>
	bool BALL_DEPRECATED BALL_INLINE ball_macro_bitarray_is_bit_set(BitArray* a, X x)
	{
		unsigned bits  = BALL_CHAR_SIZE * 8;
		unsigned shift = (bits == 8 ? 3u : (bits == 16) ? 4u : 5u);
		return (a[x >> shift] & (1 << (x & (bits - 1)))) != 0;
	}

	template<class Bitset, class Bit>
	void BALL_DEPRECATED BALL_INLINE ball_macro_bit_set(Bitset& bitset, Bit bit)
	{
		bitset |= (1 << bit);
	}

	template<class Bitset>
	void BALL_DEPRECATED BALL_INLINE ball_macro_bit_set_all(Bitset& bitset)
	{
		bitset = -1;
	}

	template<class Bitset, class Bit>
	void BALL_DEPRECATED BALL_INLINE ball_macro_bit_set_all_to(Bitset& bitset, Bit bit)
	{
		bitset |= ~(-1 << (bit + 1));
	}

	template<class Bitset, class Bit>
	void BALL_DEPRECATED BALL_INLINE ball_macro_bit_set_all_from(Bitset& bitset, Bit bit)
	{
		bitset |= (-1 << bit);
	}

	template<class Bitset, class Bit>
	void BALL_DEPRECATED BALL_INLINE ball_macro_bit_clear(Bitset& bitset, Bit bit)
	{
		bitset &= ~(1 << bit);
	}

	template<class Bitset, class Bit>
	void BALL_DEPRECATED BALL_INLINE ball_macro_bit_clear_all_to(Bitset& bitset, Bit bit)
	{
		bitset &= (-1 << (bit + 1));
	}

	template<class Bitset, class Bit>
	void BALL_DEPRECATED BALL_INLINE ball_macro_bit_clear_all_from(Bitset& bitset, Bit bit)
	{
		bitset &= ~(-1 << bit);
	}

	template<class Bitset, class Bit>
	bool BALL_DEPRECATED BALL_INLINE ball_macro_bit_is_set(Bitset& bitset, Bit bit)
	{
		return bitset & (1 << bit);
	}
}

// Macro to block execution of current thread for at least x milliseconds
#define BALL_SLEEPFOR(x)                         std::this_thread::sleep_for(std::chrono::milliseconds(x))

#define BALL_MAX(a, b)                           (((a) < (b)) ? (b) : (a))
#define BALL_MAX3(x, y, z)                       ((x) > (y) ? BALL_MAX(x, z) : BALL_MAX(y, z))
#define BALL_MIN(a, b)                           (((a) > (b)) ? (b) : (a))
#define BALL_MIN3(x, y, z)                       ((x) < (y) ? BALL_MIN(x, z) : BALL_MIN(y, z))
#define BALL_ABS(x)                              (((x) >= 0) ? (x) : -(x))
#define BALL_SGN(x)                              (((x) < 0) ? -1 : ((x) == 0) ? 0 : 1)
#define BALL_ODD(x)                              BALL::ball_macro_odd(x)

#define BALL_INT_ODD(x)                          BALL::ball_macro_int_odd(x)
#define BALL_INT_EVEN(x)                         BALL::ball_macro_int_even(x)

#define BALL_REAL_ROUND                          BALL::ball_macro_real_round
#define BALL_REAL_EQUAL(x, y, e)                 (fabs((x) - (y)) <= e)
#define BALL_REAL_NOT_EQUAL(x, y, e)             BALL::ball_macro_real_not_equal(x, y, e)
#define BALL_REAL_LESS(x, y, e)                  (((x) - (y)) < -e)
#define BALL_REAL_LESS_OR_EQUAL(x, y, e)         (((x) - (y)) <= e)
#define BALL_REAL_GREATER(x, y, e)               BALL::ball_macro_real_greater(x, y, e)
#define BALL_REAL_GREATER_OR_EQUAL(x, y, e)      (((x) - (y)) >= -e)
#define BALL_REAL_ABS(x)                         BALL::ball_macro_real_abs(x)
#define BALL_REAL_SGN(x)                         BALL::ball_macro_sgn(x)
#define BALL_REAL_ODD(x)                         BALL::ball_macro_real_odd(x)
#define BALL_REAL_EVEN(x)                        BALL::ball_macro_real_even(x)
#define BALL_REAL_FLOOR(x)                       BALL::ball_macro_real_floor(x)
#define BALL_REAL_CEILING(x)                     BALL::ball_macro_real_ceiling(x)
#define BALL_REAL_ROUND_INT(x)                   BALL::ball_macro_real_round_int(x)


// The following macros assume BALL_CHAR_BITS is one of either 8, 16, or 32
#define BALL_CHAR_BITS                           BALL_CHAR_SIZE * 8
#define BALL_CHAR_MASK                           BALL::ball_macro_char_mask<unsigned>()
#define BALL_CHAR_SHIFT                          BALL::ball_macro_char_shift<unsigned>()
#define BALL_CHAR_ALL_BITS_SET                   BALL::ball_macro_all_bits_set<char>()
#define BALL_CHAR_ALL_BITS_CLEARED               BALL::ball_macro_all_bits_cleared<char>()
#define BALL_NUMBER_OF_BYTES(bits)               BALL::ball_macro_number_of_bytes(bits)

#define BALL_SIZEOF_ARRAY(a)                     (sizeof(a) / sizeof(*(a)))

#define BALL_BITARRAY_SIZE(number_of_bits)       BALL::ball_macro_bitarray_size(number_of_bits)
#define BALL_BITARRAY_CLEAR_BIT(array, x)        BALL::ball_macro_bitarray_clear_bit(array, x)
#define BALL_BITARRAY_SET_BIT(array, x)          BALL::ball_macro_bitarray_set_bit(array, x)
#define BALL_BITARRAY_TOGGLE_BIT(array, x)       BALL::ball_macro_bitarray_toggle_bit(array, x)
#define BALL_BITARRAY_IS_BIT_SET(array, x)       BALL::ball_macro_bitarray_is_bit_set(array, x)

#define BALL_BIT(bit)                            (1 << (bit))
#define BALL_BIT_SET(bitset, bit)                BALL::ball_macro_bit_set(bitset, bit)
#define BALL_BIT_SET_ALL(bitset)                 BALL::ball_macro_bit_set_all(bitset)
#define BALL_BIT_SET_ALL_TO(bitset, bit)         BALL::ball_macro_bit_set_all_to(bitset, bit)
#define BALL_BIT_SET_ALL_FROM(bitset, bit)       BALL::ball_macro_bit_set_all_from(bitset, bit)
#define BALL_BIT_CLEAR(bitset, bit)              BALL::ball_macro_bit_clear(bitset, bit)
#define BALL_BIT_CLEAR_ALL(bitset)               ((bitset) = 0)
#define BALL_BIT_CLEAR_ALL_TO(bitset, bit)       BALL::ball_macro_bit_clear_all_to(bitset, bit)
#define BALL_BIT_CLEAR_ALL_FROM(bitset, bit)     BALL::ball_macro_bit_clear_all_from(bitset, bit)
#define BALL_BIT_IS_SET(bitset, bit)             BALL::ball_macro_bit_is_set(bitset, bit)
#define BALL_BIT_IS_CLEARED(bitset, bit)         !((bitset) & (1 << (bit)))

#define BALL_ANGLE_RADIAN_TO_DEGREE(rad_angle)   (180.0 / ::BALL::Constants::PI * (rad_angle))
#define BALL_ANGLE_DEGREE_TO_RADIAN(deg_angle)   (::BALL::Constants::PI / 180.0 * (deg_angle))

#define BALL_OFFSET_OF(struct_name, struct_var_name)   ((long)&(((struct_name*)0)->struct_var_name))

#define BALL_DUMP_DEPTH(os, depth)               for (dump_indent_depth_ = 0; dump_indent_depth_ < depth; ++dump_indent_depth_) { os << "    "; }
#define BALL_DUMP_STREAM_PREFIX(os)              Size dump_indent_depth_ = 0;

#define BALL_DUMP_HEADER(os,cl,ob)               os << "Object: " << (void *)ob << " is instance of class: " << streamClassName(typeid(*ob)) << std::endl;
#define BALL_DUMP_CLASS_HEADER(os,cl,ob)         os << "Object: " << (void *)ob << " is instance of class: " << #cl << ::std::endl;
#define BALL_DUMP_STREAM_SUFFIX(os)              

#endif // BALL_COMMON_MACROS_H

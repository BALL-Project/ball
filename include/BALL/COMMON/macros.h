// $Id: macros.h,v 1.5 2000/10/19 20:09:38 oliver Exp $

#ifndef BALL_COMMON_MACROS_H
#define BALL_COMMON_MACROS_H

#ifndef BALL_CONFIG_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif

#ifndef BALL_COMMON_CONSTANTS_H
#	include <BALL/COMMON/constants.h>
#endif

#include <math.h>     // needed for fabs
#include <typeinfo>		// needed for typeid

#define BALL_MAX(a, b)                           (((a) < (b)) ? (b) : (a))
#define BALL_MAX3(x, y, z)                       ((x) > (y) ? BALL_MAX(x, z) : BALL_MAX(y, z))
#define BALL_MIN(a, b)                           (((a) > (b)) ? (b) : (a))
#define BALL_MIN3(x, y, z)                       ((x) < (y) ? BALL_MIN(x, z) : BALL_MIN(y, z))
#define BALL_ABS(x)                              (((x) >= 0) ? (x) : -(x))
#define BALL_SGN(x)                              (((x) < 0) ? -1 : ((x) == 0) ? 0 : 1)
#define BALL_ODD(x)                              (((x) % 2)!=0)

#define BALL_INT_ODD(x)                          (((x) & 0x1) == 1)
#define BALL_INT_EVEN(x)                         (((x) & 0x1) == 0)

#define BALL_REAL_ROUND                          BALL::Maths::round
#define BALL_REAL_EQUAL(x, y, e)                 (fabs((x) - (y)) <= e)
#define BALL_REAL_NOT_EQUAL(x, y, e)             (fabs((x) - (y)) > e)
#define BALL_REAL_LESS(x, y, e)                  (((x) - (y)) < -e)
#define BALL_REAL_LESS_OR_EQUAL(x, y, e)         (((x) - (y)) <= e)
#define BALL_REAL_GREATER(x, y, e)               (((x) - (y)) > e)
#define BALL_REAL_GREATER_OR_EQUAL(x, y, e)      (((x) - (y)) >= -e)
#define BALL_REAL_ABS(x)                         fabs(x)
#define BALL_REAL_SGN(x)                         BALL_SGN(x)
#define BALL_REAL_ODD(x)                         (((x) % 2) != 0)
#define BALL_REAL_EVEN(x)                        (((x) % 2) == 0)
#define BALL_REAL_FLOOR(x)                       (long)((x) > 0 ? (x): ((x) == (long)(x) ? (x) : (x) - 1))
#define BALL_REAL_CEILING(x)                     (long)((x) < 0 ? (x): ((x) == (long)(x) ? (x) : (x) + 1))
#define BALL_REAL_ROUND_INT(x)                   ((x) > 0 ? (int)(x + 0.5) : -(int)(0.5 - x))


// The following macros assume BALL_CHAR_BITS is one of either 8, 16, or 32
#define BALL_CHAR_BITS                           BALL_CHAR_SIZE * 8
#define BALL_CHAR_MASK                           BALL_CHAR_BITS - 1
#define BALL_CHAR_SHIFT                          ((BALL_CHAR_BITS == 8) ? 3 : (BALL_CHAR_BITS == 16) ? 4 : 5)
#define BALL_CHAR_ALL_BITS_SET                   ((BALL_CHAR_BITS == 8) ? 0xFF : (BALL_CHAR_BITS == 16) ? 0xFFFF : 0xFFFFFFFF)
#define BALL_CHAR_ALL_BITS_CLEARED               ((BALL_CHAR_BITS == 8) ? 0x00 : (BALL_CHAR_BITS == 16) ? 0x0000 : 0x00000000)
#define BALL_NUMBER_OF_BYTES(bits)               (((bits) + BALL_CHAR_MASK) >> BALL_CHAR_SHIFT)

#define BALL_SIZEOF_ARRAY(a)                     (sizeof(a) / sizeof(*(a)))

#define BALL_BITARRAY_SIZE(number_of_bits)       (((number_of_bits - 1) >> BALL_CHAR_SHIFT) + 1)
#define BALL_BITARRAY_CLEAR_BIT(array, x)        ((array)[(x) >> BALL_CHAR_SHIFT] &= ~(1 << ((x) & BALL_CHAR_MASK)))
#define BALL_BITARRAY_SET_BIT(array, x)          ((array)[(x) >> BALL_CHAR_SHIFT] |=  (1 << ((x) & BALL_CHAR_MASK)))
#define BALL_BITARRAY_TOGGLE_BIT(array, x)       ((array)[(x) >> BALL_CHAR_SHIFT] ^=  (1 << ((x) & BALL_CHAR_MASK)))
#define BALL_BITARRAY_IS_BIT_SET(array, x)       (((array)[(x) >> BALL_CHAR_SHIFT] &   (1 << ((x) & BALL_CHAR_MASK))) != 0)

#define BALL_BIT(bit)                            (1 << (bit))
#define BALL_BIT_SET(bitset, bit)                ((bitset) |= (1 << (bit)))
#define BALL_BIT_SET_ALL(bitset)                 ((bitset) = -1)
#define BALL_BIT_SET_ALL_TO(bitset, bit)         ((bitset) |= ~(-1 << (bit + 1)))
#define BALL_BIT_SET_ALL_FROM(bitset, bit)       ((bitset) |= (-1 << (bit)))
#define BALL_BIT_CLEAR(bitset, bit)              ((bitset) &= ~(1 << (bit)))
#define BALL_BIT_CLEAR_ALL(bitset)               ((bitset) = 0)
#define BALL_BIT_CLEAR_ALL_TO(bitset, bit)       ((bitset) &= (-1 << (bit + 1)))
#define BALL_BIT_CLEAR_ALL_FROM(bitset, bit)     ((bitset) &= ~(-1 << (bit)))
#define BALL_BIT_IS_SET(bitset, bit)             ((bitset) & (1 << (bit)))
#define BALL_BIT_IS_CLEARED(bitset, bit)         !((bitset) & (1 << (bit)))

#define BALL_ANGLE_RADIAN_TO_DEGREE(rad_angle)   (180.0 / ::BALL::Constants::PI * (rad_angle))
#define BALL_ANGLE_DEGREE_TO_RADIAN(deg_angle)   (::BALL::Constants::PI / 180.0 * (deg_angle))

#define BALL_OFFSET_OF(struct_name, struct_var_name)   ((long)&(((struct_name*)0)->struct_var_name))
  
#define BALL_DUMP_DEPTH(os, depth)               for (dump_indent_depth_ = 0; dump_indent_depth_ < depth; ++dump_indent_depth_) { os << "    "; }
#define BALL_DUMP_STREAM_PREFIX(os)              Size dump_indent_depth_ = 0;

#define BALL_DUMP_HEADER(os,cl,ob)               os << "Object: " << (void *)ob << " is instance of class: " << typeid(*ob).name() << std::endl;
#define BALL_DUMP_CLASS_HEADER(os,cl,ob)         os << "Object: " << (void *)ob << " is instance of class: " << #cl << ::std::endl;
#define BALL_DUMP_STREAM_SUFFIX(os)              

#endif // BALL_COMMON_MACROS_H

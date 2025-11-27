#ifndef __X_H__
#define __X_H__

#ifdef __cplusplus
extern "C" {
#endif

#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64)) && !defined(__WINDOWS__)
# define __WINDOWS__
#endif
#if defined(__linux__) || defined(__CYGWIN__)
# include <endian.h>
#elif defined(__MACH__)
# include <sys/types.h>
#elif defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__) || defined(__DragonFly__)
# include <sys/endian.h>
#elif defined(__WINDOWS__)
# include <winsock2.h>
# include <sys/param.h>
# include <sys/time.h>
#else
# error "platform not supported!"
#endif

#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>

/*
 * byteorder
 */
static inline uint16_t __swab16(uint16_t x)
{
	return ((x << 8) | ( x>> 8));
}

static inline uint32_t __swab32(uint32_t x)
{
	return ((x << 24) | (x >> 24) | \
		((x & (uint32_t)0x0000ff00UL)<<8) | \
		((x & (uint32_t)0x00ff0000UL)>>8));
}

static inline uint64_t __swab64(uint64_t x)
{
	return ((x << 56) | (x >> 56) | \
		((x & (uint64_t)0x000000000000ff00ULL)<<40) | \
		((x & (uint64_t)0x0000000000ff0000ULL)<<24) | \
		((x & (uint64_t)0x00000000ff000000ULL)<< 8) | \
		((x & (uint64_t)0x000000ff00000000ULL)>> 8) | \
		((x & (uint64_t)0x0000ff0000000000ULL)>>24) | \
		((x & (uint64_t)0x00ff000000000000ULL)>>40));
}

static inline uint32_t __swahw32(uint32_t x)
{
	return (((x & (uint32_t)0x0000ffffUL) << 16) | ((x & (uint32_t)0xffff0000UL) >> 16));
}

static inline uint32_t __swahb32(uint32_t x)
{
	return (((x & (uint32_t)0x00ff00ffUL) << 8) | ((x & (uint32_t)0xff00ff00UL) >> 8));
}

#if BYTE_ORDER == BIG_ENDIAN
#define cpu_to_le64(x)	(__swab64((uint64_t)(x)))
#define le64_to_cpu(x)	(__swab64((uint64_t)(x)))
#define cpu_to_le32(x)	(__swab32((uint32_t)(x)))
#define le32_to_cpu(x)	(__swab32((uint32_t)(x)))
#define cpu_to_le16(x)	(__swab16((uint16_t)(x)))
#define le16_to_cpu(x)	(__swab16((uint16_t)(x)))
#define cpu_to_be64(x)	((uint64_t)(x))
#define be64_to_cpu(x)	((uint64_t)(x))
#define cpu_to_be32(x)	((uint32_t)(x))
#define be32_to_cpu(x)	((uint32_t)(x))
#define cpu_to_be16(x)	((uint16_t)(x))
#define be16_to_cpu(x)	((uint16_t)(x))
#else
#define cpu_to_le64(x)	((uint64_t)(x))
#define le64_to_cpu(x)	((uint64_t)(x))
#define cpu_to_le32(x)	((uint32_t)(x))
#define le32_to_cpu(x)	((uint32_t)(x))
#define cpu_to_le16(x)	((uint16_t)(x))
#define le16_to_cpu(x)	((uint16_t)(x))
#define cpu_to_be64(x)	(__swab64((uint64_t)(x)))
#define be64_to_cpu(x)	(__swab64((uint64_t)(x)))
#define cpu_to_be32(x)	(__swab32((uint32_t)(x)))
#define be32_to_cpu(x)	(__swab32((uint32_t)(x)))
#define cpu_to_be16(x)	(__swab16((uint16_t)(x)))
#define be16_to_cpu(x)	(__swab16((uint16_t)(x)))
#endif

/*
 * Get unaligned
 */
static inline uint16_t __get_unaligned_le16(const uint8_t * p)
{
	return (p[0] << 0) | (p[1] << 8);
}

static inline uint16_t __get_unaligned_be16(const uint8_t * p)
{
	return (p[0] << 8) | (p[1] << 0);
}

static inline uint32_t __get_unaligned_le32(const uint8_t * p)
{
	return (p[0] << 0) | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
}

static inline uint32_t __get_unaligned_be32(const uint8_t * p)
{
	return (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | (p[3] << 0);
}

static inline uint64_t __get_unaligned_le64(const uint8_t * p)
{
	return ((uint64_t)__get_unaligned_le32(p + 4) << 32) | __get_unaligned_le32(p);
}

static inline uint64_t __get_unaligned_be64(const uint8_t * p)
{
	return ((uint64_t)__get_unaligned_be32(p) << 32) | __get_unaligned_be32(p + 4);
}

static inline uint16_t get_unaligned_le16(const void * p)
{
	return __get_unaligned_le16((const uint8_t *)p);
}

static inline uint16_t get_unaligned_be16(const void * p)
{
	return __get_unaligned_be16((const uint8_t *)p);
}

static inline uint32_t get_unaligned_le32(const void * p)
{
	return __get_unaligned_le32((const uint8_t *)p);
}

static inline uint32_t get_unaligned_be32(const void * p)
{
	return __get_unaligned_be32((const uint8_t *)p);
}

static inline uint64_t get_unaligned_le64(const void * p)
{
	return __get_unaligned_le64((const uint8_t *)p);
}

static inline uint64_t get_unaligned_be64(const void * p)
{
	return __get_unaligned_be64((const uint8_t *)p);
}

/*
 * Put unaligned
 */
static inline void __put_unaligned_le16(uint8_t * p, uint16_t val)
{
	*p++ = val;
	*p++ = val >> 8;
}

static inline void __put_unaligned_be16(uint8_t * p, uint16_t val)
{
	*p++ = val >> 8;
	*p++ = val;
}

static inline void __put_unaligned_le32(uint8_t * p, uint32_t val)
{
	__put_unaligned_le16(p + 2, val >> 16);
	__put_unaligned_le16(p, val);
}

static inline void __put_unaligned_be32(uint8_t * p, uint32_t val)
{
	__put_unaligned_be16(p, val >> 16);
	__put_unaligned_be16(p + 2, val);
}

static inline void __put_unaligned_le64(uint8_t * p, uint64_t val)
{
	__put_unaligned_le32(p + 4, val >> 32);
	__put_unaligned_le32(p, val);
}

static inline void __put_unaligned_be64(uint8_t * p, uint64_t val)
{
	__put_unaligned_be32(p, val >> 32);
	__put_unaligned_be32(p + 4, val);
}

static inline void put_unaligned_le16(void * p, uint16_t val)
{
	__put_unaligned_le16((uint8_t *)p, val);
}

static inline void put_unaligned_be16(void * p, uint16_t val)
{
	__put_unaligned_be16((uint8_t *)p, val);
}

static inline void put_unaligned_le32(void * p, uint32_t val)
{
	__put_unaligned_le32((uint8_t *)p, val);
}

static inline void put_unaligned_be32(void * p, uint32_t val)
{
	__put_unaligned_be32((uint8_t *)p, val);
}

static inline void put_unaligned_le64(void * p, uint64_t val)
{
	__put_unaligned_le64((uint8_t *)p, val);
}

static inline void put_unaligned_be64(void * p, uint64_t val)
{
	__put_unaligned_be64((uint8_t *)p, val);
}

/*
 * xdef
 */
#if !(defined(NULL))
#if defined(__cplusplus)
#define NULL							(0)
#else
#define NULL							((void *)0)
#endif
#endif

#if !(defined(offsetof))
#if (defined(__GNUC__) && (__GNUC__ >= 4))
#define offsetof(type, member)			__builtin_offsetof(type, member)
#else
#define offsetof(type, field)			((size_t)(&((type *)0)->field))
#endif
#endif

#if !(defined(container_of))
#define container_of(ptr, type, member)	({const typeof(((type *)0)->member) *__mptr = (ptr); (type *)((char *)__mptr - offsetof(type,member));})
#endif

#if !(defined(likely))
#if (defined(__GNUC__) && (__GNUC__ >= 3))
#define likely(expr)					(__builtin_expect(!!(expr), 1))
#else
#define likely(expr)					(!!(expr))
#endif
#endif

#if !(defined(unlikely))
#if (defined(__GNUC__) && (__GNUC__ >= 3))
#define unlikely(expr)					(__builtin_expect(!!(expr), 0))
#else
#define unlikely(expr)					(!!(expr))
#endif
#endif

#define XMAP(x, ia, ib, oa, ob)			((x - ia) * (ob - oa) / (ib - ia) + oa)
#define XMIN(a, b)						({typeof(a) _amin = (a); typeof(b) _bmin = (b); (void)(&_amin == &_bmin); _amin < _bmin ? _amin : _bmin;})
#define XMAX(a, b)						({typeof(a) _amax = (a); typeof(b) _bmax = (b); (void)(&_amax == &_bmax); _amax > _bmax ? _amax : _bmax;})
#define XCLAMP(v, a, b)					XMIN(XMAX(a, v), b)

#define XFLOOR(x)						((x) > 0 ? (int)(x) : (int)((x) - 0.9999999999))
#define XROUND(x)						((x) > 0 ? (int)((x) + 0.5) : (int)((x) - 0.5))
#define XCEIL(x)						((x) > 0 ? (int)((x) + 0.9999999999) : (int)(x))
#define XDIV255(x)						((((int)(x) + 1) * 257) >> 16)

#define ARRAY_SIZE(array)				(sizeof(array) / sizeof((array)[0]))
#define X(...)							("" #__VA_ARGS__ "")

#ifdef __cplusplus
}
#endif

#endif /* __X_H__ */

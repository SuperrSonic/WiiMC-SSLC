/*
 * Copyright (c) 2010-2011 Extrems <metaradil@gmail.com>
 *
 * This file is part of MPlayer CE.
 *
 * MPlayer CE is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * MPlayer CE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with MPlayer CE; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef AVUTIL_PPC_PAIRED_H
#define AVUTIL_PPC_PAIRED_H

#include <paired.h>

typedef float vec_f32_t __attribute__((vector_size(8)));
typedef unsigned char vec_u8_t __attribute__((vector_size(2)));
typedef unsigned short vec_u16_t __attribute__((vector_size(4)));
typedef signed char vec_s8_t __attribute__((vector_size(2)));
typedef signed short vec_s16_t __attribute__((vector_size(4)));

enum {
	GQR_TYPE_FLOAT = 0,
	GQR_TYPE_U8 = 4,
	GQR_TYPE_U16,
	GQR_TYPE_S8,
	GQR_TYPE_S16,
};

#define psq_l(d, rA, W, I) ({ \
	vector float frD; \
	asm volatile("psq_l %0,%1(%2),%3,%4" \
	             : "=f"(frD) : "i"(d), "b"(rA), "i"(W), "i"(I) : "memory"); \
	frD; })

#define psq_lx(rA, rB, W, I) ({ \
	vector float frD; \
	asm volatile("psq_lx %0,%1,%2,%3,%4" \
	             : "=f"(frD) : "b"(rA), "r"(rB), "i"(W), "i"(I) : "memory"); \
	frD; })

#define psq_lu(d, rA, W, I) ({ \
	vector float frD; \
	asm volatile("psq_lu %0,%2(%1),%3,%4" \
	             : "=f"(frD), "+b"(rA) : "i"(d), "i"(W), "i"(I) : "memory"); \
	frD; })

#define psq_lux(rA, rB, W, I) ({ \
	vector float frD; \
	asm volatile("psq_lux %0,%1,%2,%3,%4" \
	             : "=f"(frD), "+b"(rA) : "r"(rB), "i"(W), "i"(I) : "memory"); \
	frD; })

#define psq_st(frD, d, rA, W, I) \
	asm volatile("psq_st %0,%1(%2),%3,%4" \
	             :: "f"(frD), "i"(d), "b"(rA), "i"(W), "i"(I) : "memory")

#define psq_stx(frD, rA, rB, W, I) \
	asm volatile("psq_stx %0,%1,%2,%3,%4" \
	             :: "f"(frD), "b"(rA), "r"(rB), "i"(W), "i"(I) : "memory")

#define psq_stu(frD, d, rA, W, I) \
	asm volatile("psq_stu %1,%2(%0),%3,%4" \
	             : "+b"(rA) : "f"(frD), "i"(d), "i"(W), "i"(I) : "memory")

#define psq_stux(frD, rA, rB, W, I) \
	asm volatile("psq_stux %1,%0,%2,%3,%4" \
	             : "+b"(rA) : "f"(frD), "r"(rB), "i"(W), "i"(I) : "memory")

#define ps_neg(frB) ({ \
	vector float frD; \
	asm("ps_neg %0,%1" : "=f"(frD) : "f"(frB)); \
	frD; })

#define ps_add(frA, frB) ({ \
	vector float frD; \
	asm("ps_add %0,%1,%2" : "=f"(frD) : "f"(frA), "f"(frB)); \
	frD; })

#define ps_sub(frA, frB) ({ \
	vector float frD; \
	asm("ps_sub %0,%1,%2" : "=f"(frD) : "f"(frA), "f"(frB)); \
	frD; })

#define ps_mul(frA, frC) ({ \
	vector float frD; \
	asm("ps_mul %0,%1,%2" : "=f"(frD) : "f"(frA), "f"(frC)); \
	frD; })

#define ps_madd(frA, frC, frB) ({ \
	vector float frD; \
	asm("ps_madd %0,%1,%2,%3" : "=f"(frD) : "f"(frA), "f"(frC), "f"(frB)); \
	frD; })

#define ps_madds0(frA, frC, frB) ({ \
	vector float frD; \
	asm("ps_madds0 %0,%1,%2,%3" : "=f"(frD) : "f"(frA), "f"(frC), "f"(frB)); \
	frD; })

#define ps_madds1(frA, frC, frB) ({ \
	vector float frD; \
	asm("ps_madds1 %0,%1,%2,%3" : "=f"(frD) : "f"(frA), "f"(frC), "f"(frB)); \
	frD; })

#define ps_msub(frA, frC, frB) ({ \
	vector float frD; \
	asm("ps_msub %0,%1,%2,%3" : "=f"(frD) : "f"(frA), "f"(frC), "f"(frB)); \
	frD; })

#define ps_muls0(frA, frC) ({ \
	vector float frD; \
	asm("ps_muls0 %0,%1,%2" : "=f"(frD) : "f"(frA), "f"(frC)); \
	frD; })

#define ps_muls1(frA, frC) ({ \
	vector float frD; \
	asm("ps_muls1 %0,%1,%2" : "=f"(frD) : "f"(frA), "f"(frC)); \
	frD; })

#define ps_nmadd(frA, frC, frB) ({ \
	vector float frD; \
	asm("ps_nmadd %0,%1,%2,%3" : "=f"(frD) : "f"(frA), "f"(frC), "f"(frB)); \
	frD; })

#define ps_nmsub(frA, frC, frB) ({ \
	vector float frD; \
	asm("ps_nmsub %0,%1,%2,%3" : "=f"(frD) : "f"(frA), "f"(frC), "f"(frB)); \
	frD; })

#define ps_merge00(frA, frB) ({ \
	vector float frD; \
	asm("ps_merge00 %0,%1,%2" : "=f"(frD) : "f"(frA), "f"(frB)); \
	frD; })

#define ps_merge01(frA, frB) ({ \
	vector float frD; \
	asm("ps_merge01 %0,%1,%2" : "=f"(frD) : "f"(frA), "f"(frB)); \
	frD; })

#define ps_merge10(frA, frB) ({ \
	vector float frD; \
	asm("ps_merge10 %0,%1,%2" : "=f"(frD) : "f"(frA), "f"(frB)); \
	frD; })

#define ps_merge11(frA, frB) ({ \
	vector float frD; \
	asm("ps_merge11 %0,%1,%2" : "=f"(frD) : "f"(frA), "f"(frB)); \
	frD; })

#define ps_sel(frA, frC, frB) ({ \
	vector float frD; \
	asm("ps_sel %0,%1,%2,%3" : "=f"(frD) : "f"(frA), "f"(frC), "f"(frB)); \
	frD; })

#define ps_sum0(frA, frC, frB) ({ \
	vector float frD; \
	asm("ps_sum0 %0,%1,%2,%3" : "=f"(frD) : "f"(frA), "f"(frC), "f"(frB)); \
	frD; })

#define ps_sum1(frA, frC, frB) ({ \
	vector float frD; \
	asm("ps_sum1 %0,%1,%2,%3" : "=f"(frD) : "f"(frA), "f"(frC), "f"(frB)); \
	frD; })

#endif /* AVUTIL_PPC_PAIRED_H */

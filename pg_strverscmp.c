/*
 * Copyright (C)  Jan Dvorak <mordae@anilinux.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdlib.h>
#include <unistr.h>
#include <uninorm.h>
#include <unicase.h>

#include "postgres.h"
#include "fmgr.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

PG_FUNCTION_INFO_V1(pg_strverscmp);
PG_FUNCTION_INFO_V1(pg_strverscmp_eq);
PG_FUNCTION_INFO_V1(pg_strverscmp_ne);
PG_FUNCTION_INFO_V1(pg_strverscmp_gt);
PG_FUNCTION_INFO_V1(pg_strverscmp_ge);
PG_FUNCTION_INFO_V1(pg_strverscmp_lt);
PG_FUNCTION_INFO_V1(pg_strverscmp_le);

int u32_strverscmp(const uint32_t *p1, const uint32_t *p2);

static int normalize_and_compare(text *a, text *b)
{
	int res;

	size_t a32_len = 4 * VARSIZE(a) + 4;
	size_t b32_len = 4 * VARSIZE(b) + 4;

	uint32_t *a32 = palloc0(a32_len);
	uint32_t *b32 = palloc0(b32_len);

	u8_to_u32((uint8_t *) VARDATA(a), VARSIZE(a), a32, &a32_len);
	u8_to_u32((uint8_t *) VARDATA(b), VARSIZE(b), b32, &b32_len);

	res = u32_strverscmp(a32, b32);

	return res;
}

Datum pg_strverscmp(PG_FUNCTION_ARGS)
{
	text *ta = PG_GETARG_TEXT_P(0);
	text *tb = PG_GETARG_TEXT_P(1);

	PG_RETURN_INT32(normalize_and_compare(ta, tb));
}

#define MAKE_PREDICATE(FN_NAME, OPERATOR)					\
	Datum FN_NAME(PG_FUNCTION_ARGS)						\
	{									\
		text *ta = PG_GETARG_TEXT_P(0);					\
		text *tb = PG_GETARG_TEXT_P(1);					\
										\
		PG_RETURN_BOOL(normalize_and_compare(ta, tb) OPERATOR 0);	\
	}

MAKE_PREDICATE(pg_strverscmp_eq, ==)
MAKE_PREDICATE(pg_strverscmp_ne, !=)
MAKE_PREDICATE(pg_strverscmp_gt, > )
MAKE_PREDICATE(pg_strverscmp_ge, >=)
MAKE_PREDICATE(pg_strverscmp_lt, < )
MAKE_PREDICATE(pg_strverscmp_le, <=)

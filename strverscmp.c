/*
 * This file has been modified to use UCS-32 character encoding
 * and work with libunistring. Sorry for the mess.
 */

/* Compare strings while treating digits characters numerically.
   Copyright (C) 1997, 2002, 2009 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Jean-François Bignolles <bignolle@ecoledoc.ibp.fr>, 1997.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <unistr.h>
#include <unictype.h>
#include <unicase.h>

/* states: S_N: normal, S_I: comparing integral part, S_F: comparing
           fractionnal parts, S_Z: idem but with leading Zeroes only */
#define  S_N    0x0
#define  S_I    0x3
#define  S_F    0x6
#define  S_Z    0x9

/* result_type: CMP: return diff; LEN: compare using len_diff/diff */
#define  CMP    2
#define  LEN    3


/* Compare S1 and S2 as strings holding indices/version numbers,
   returning less than, equal to or greater than zero if S1 is less than,
   equal to or greater than S2 (for more info, see the texinfo doc).
*/

int u32_strverscmp(const uint32_t *a, const uint32_t *b);

inline static int uc_cmp(uint32_t a, uint32_t b)
{
  int result = 0;

  if (0 == u32_casecoll(&a, 1, &b, 1, NULL, UNINORM_NFC, &result))
    return result;

  /* Nothing meaningful to do in case of errors, so try our best. */
  return (a - b);
}

int u32_strverscmp(const uint32_t *a, const uint32_t *b)
{
  /* Symbol(s)    0       [1-9]   others
     Transition   (10) 0  (01) d  (00) x   */
  static const uint8_t next_state[] =
  {
      /* state    x    d    0  */
      /* S_N */  S_N, S_I, S_Z,
      /* S_I */  S_N, S_I, S_I,
      /* S_F */  S_N, S_F, S_F,
      /* S_Z */  S_N, S_F, S_Z
  };

  static const int8_t result_type[] =
  {
      /* state   x/x  x/d  x/0  d/x  d/d  d/0  0/x  0/d  0/0  */

      /* S_N */  CMP, CMP, CMP, CMP, LEN, CMP, CMP, CMP, CMP,
      /* S_I */  CMP, -1,  -1,  +1,  LEN, LEN, +1,  LEN, LEN,
      /* S_F */  CMP, CMP, CMP, CMP, CMP, CMP, CMP, CMP, CMP,
      /* S_Z */  CMP, +1,  +1,  -1,  CMP, CMP, -1,  CMP, CMP
  };

  uint32_t ac, bc;
  int state, diff, digit;

  if (a == b)
    return 0;

  ac = *a++;
  bc = *b++;

  digit = uc_digit_value(ac);

  /* Hint: '0' is a digit too.  */
  state = S_N + ((digit == 0) + (digit >= 0));

  while ((diff = uc_cmp(ac, bc)) == 0)
    {
      if (ac == 0)
        return diff;

      state = next_state[state];
      ac = *a++;
      bc = *b++;

      digit = uc_digit_value(ac);
      state += (digit == 0) + (digit >= 0);
    }

  digit = uc_digit_value(bc);
  state = result_type[state * 3 + (((digit == 0) + (digit >= 0)))];

  switch (state)
  {
    case CMP:
      return diff;

    case LEN:
      while (uc_digit_value(*a++) >= 0)
        if (uc_digit_value(*b++) < 0)
          return 1;

      return (uc_digit_value(*b) >= 0) ? -1 : diff;

    default:
      return state;
  }
}

/* vim:set sw=2 ts=2 et: */

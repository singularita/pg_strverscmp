# Hack for natural sorts in PostgreSQL

This extension adds `+<`, `+>`, `+<=`, `+>=`, `+=`, and `+<>` operators that use unicode-aware `strverscmp(3)`-like ordering along with a new operator class called `strverscmp_ops` for use with indexes

## How does it work?

It makes use of a shared library that includes a modified `strverscmp` implementation straight from `glibc` and some wrapper functions.

## Is it buggy?

Yes, very much.

- It is not binary safe, meaning that strings that contain the `\0` character will be treated as terminated by that character.
- It does not make any use of PostgreSQL collation tools and uses general, case insensitive collation by `libunistring`.
- That comparison function is very inefficient, calling the `u32_strcoll` for every compared character pair.

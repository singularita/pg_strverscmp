#!/usr/bin/make -f

PG_CPPFLAGS = -Werror

MODULE_big = pg_strverscmp
EXTENSION = pg_strverscmp
DATA = pg_strverscmp--1.0.sql
OBJS = pg_strverscmp.o strverscmp.o
SHLIB_LINK = -lunistring

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

# EOF

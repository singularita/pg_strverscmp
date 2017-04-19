CREATE FUNCTION pg_strverscmp_eq(text, text)
RETURNS boolean LANGUAGE c IMMUTABLE AS 'MODULE_PATHNAME';

CREATE FUNCTION pg_strverscmp_ne(text, text)
RETURNS boolean LANGUAGE c IMMUTABLE AS 'MODULE_PATHNAME';

CREATE FUNCTION pg_strverscmp_lt(text, text)
RETURNS boolean LANGUAGE c IMMUTABLE AS 'MODULE_PATHNAME';

CREATE FUNCTION pg_strverscmp_le(text, text)
RETURNS boolean LANGUAGE c IMMUTABLE AS 'MODULE_PATHNAME';

CREATE FUNCTION pg_strverscmp_gt(text, text)
RETURNS boolean LANGUAGE c IMMUTABLE AS 'MODULE_PATHNAME';

CREATE FUNCTION pg_strverscmp_ge(text, text)
RETURNS boolean LANGUAGE c IMMUTABLE AS 'MODULE_PATHNAME';

CREATE FUNCTION pg_strverscmp(text, text)
RETURNS integer LANGUAGE c IMMUTABLE AS 'MODULE_PATHNAME';

CREATE OPERATOR += (
  LEFTARG = text,
  RIGHTARG = text,
  PROCEDURE = pg_strverscmp_eq,
  COMMUTATOR = += ,
  NEGATOR = +<> ,
  RESTRICT = eqsel,
  JOIN = eqjoinsel,
  MERGES
);

CREATE OPERATOR +<> (
  LEFTARG = text,
  RIGHTARG = text,
  PROCEDURE = pg_strverscmp_ne,
  COMMUTATOR = +<> ,
  NEGATOR = += ,
  RESTRICT = neqsel,
  JOIN = neqjoinsel
);

CREATE OPERATOR +< (
  LEFTARG = text,
  RIGHTARG = text,
  PROCEDURE = pg_strverscmp_lt,
  COMMUTATOR = +> ,
  NEGATOR = +>= ,
  RESTRICT = scalarltsel,
  JOIN = scalarltjoinsel
);

CREATE OPERATOR +<= (
  LEFTARG = text,
  RIGHTARG = text,
  PROCEDURE = pg_strverscmp_le,
  COMMUTATOR = +>= ,
  NEGATOR = +> ,
  RESTRICT = scalarltsel,
  JOIN = scalarltjoinsel
);

CREATE OPERATOR +> (
  LEFTARG = text,
  RIGHTARG = text,
  PROCEDURE = pg_strverscmp_gt,
  COMMUTATOR = +< ,
  NEGATOR = +<= ,
  RESTRICT = scalargtsel,
  JOIN = scalargtjoinsel
);

CREATE OPERATOR +>= (
  LEFTARG = text,
  RIGHTARG = text,
  PROCEDURE = pg_strverscmp_ge,
  COMMUTATOR = +<= ,
  NEGATOR = +< ,
  RESTRICT = scalargtsel,
  JOIN = scalargtjoinsel
);

CREATE OPERATOR CLASS strverscmp_ops
FOR TYPE text USING btree
AS
  OPERATOR  1  +<  ,
  OPERATOR  2  +<= ,
  OPERATOR  3  +=  ,
  OPERATOR  4  +>= ,
  OPERATOR  5  +>  ,
  FUNCTION  1  pg_strverscmp(text, text);

-- vim:set sw=2 ts=2 et:

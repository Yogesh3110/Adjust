CREATE EXTENSION IF NOT EXISTS plpgsql;

CREATE OR REPLACE FUNCTION min_to_max(integer[])
    RETURNS text
    AS 'MODULE_PATHNAME', 'min_to_max_finalfn'
    LANGUAGE C STRICT;

CREATE AGGREGATE min_to_max (integer) (
    SFUNC = min_to_max_transfn,
    STYPE = internal,
    FINALFUNC = min_to_max_finalfn,
    INITCOND = '{}'
);


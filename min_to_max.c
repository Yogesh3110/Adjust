#include <limits.h>
#ifndef PG_INT32_MAX
#define PG_INT32_MAX INT_MAX
#endif
#ifndef PG_INT32_MIN
#define PG_INT32_MIN INT_MIN
#endif
#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "catalog/pg_type.h"

PG_MODULE_MAGIC;

typedef struct {
    int32 min_val;
    int32 max_val;
} MinMaxAggState;

/* Aggregate function called for each row */
PG_FUNCTION_INFO_V1(min_to_max_transfn);
Datum min_to_max_transfn(PG_FUNCTION_ARGS) {
    Oid val_type = get_fn_expr_argtype(fcinfo->flinfo, 1);
    if (val_type != INT4OID) {
        ereport(ERROR, (errmsg("min_to_max aggregate only supports integer columns")));
    }

    if (PG_ARGISNULL(0)) {
        MinMaxAggState *aggstate;
        aggstate = (MinMaxAggState *)palloc(sizeof(MinMaxAggState));
        aggstate->min_val = PG_INT32_MAX;
        aggstate->max_val = PG_INT32_MIN;
        PG_RETURN_POINTER(aggstate);
    }

    MinMaxAggState *aggstate = (MinMaxAggState *)PG_GETARG_POINTER(0);
    int32 val = PG_GETARG_INT32(1);

    if (val < aggstate->min_val) {
        aggstate->min_val = val;
    }

    if (val > aggstate->max_val) {
        aggstate->max_val = val;
    }

    PG_RETURN_POINTER(aggstate);
}

/* Aggregate function called after all rows have been processed */
PG_FUNCTION_INFO_V1(min_to_max_finalfn);
Datum min_to_max_finalfn(PG_FUNCTION_ARGS) {
    MinMaxAggState *aggstate = (MinMaxAggState *)PG_GETARG_POINTER(0);
    char *result_str;
    result_str = psprintf("%d -> %d", aggstate->min_val, aggstate->max_val);
    PG_RETURN_TEXT_P(cstring_to_text(result_str));
}

/* Extension initialization function */
PG_FUNCTION_INFO_V1(min_to_max_init);
Datum min_to_max_init(PG_FUNCTION_ARGS) {
    PG_RETURN_VOID();
}

/* Extension uninstallation function */
PG_FUNCTION_INFO_V1(min_to_max_fini);
Datum min_to_max_fini(PG_FUNCTION_ARGS) {
    PG_RETURN_VOID();
}


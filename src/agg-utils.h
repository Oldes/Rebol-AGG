//   ____  __   __        ______        __
//  / __ \/ /__/ /__ ___ /_  __/__ ____/ /
// / /_/ / / _  / -_|_-<_ / / / -_) __/ _ \
// \____/_/\_,_/\__/___(@)_/  \__/\__/_// /
//  ~~~ oldes.huhuman at gmail.com ~~~ /_/
//
// SPDX-License-Identifier: MIT
// =============================================================================
// Rebol/AGG extension utils
// =============================================================================

#ifndef AGG_UTILS_H
#define AGG_UTILS_H

#include "agg-rebol-extension.h"

REBOOL fetch_word(REBSER *cmds, REBCNT index, u32* words, REBCNT *cmd);
REBOOL fetch_mode(REBSER *cmds, REBCNT index, REBCNT *result, REBCNT start, REBCNT max);
REBOOL fetch_color(REBSER *cmds, REBCNT index, REBCNT *cmd);



#ifndef PI
#define PI 3.14159265358979323846264338327950288
#endif

#define TO_RADIANS(value) ((value) *= PI / 180.0)
#define AS_RADIANS(value) (value * PI / 180.0)

#define AS_RGBA(t) agg::rgba8(t.bytes[1], t.bytes[2], t.bytes[3], t.bytes[0]==3?255:t.bytes[4])
#define ARG_BOOL(n)  (bool)arg[n].int32a
#define ARG_PAIR_X(n) (double)arg[n].pair.x
#define ARG_PAIR_Y(n) (double)arg[n].pair.y
#define ARG_PAIR(n)  ARG_PAIR_X(n), ARG_PAIR_Y(n)
#define ARG_DOUBLE(n)(double)arg[n].dec64
#define ARG_INT64(n) (double)arg[n].int64
#define ARG_WORD(n) RL_FIND_WORD(agg_arg_words, arg[n].int32a);
#define ARG_NUMBER(n) (type==RXT_INTEGER ? ARG_INT64(n) : ARG_DOUBLE(n))

#define TYPE_IS_NUMBER (type == RXT_INTEGER || type == RXT_DECIMAL || type == RXT_PERCENT)
#define TYPE_IS_LOGIC  (type == RXT_LOGIC || type == RXT_NONE)


#define TUPLE_TO_COLOR(t) (REBCNT)((t.bytes[0]==3?255:t.bytes[4])<<24 | (t.bytes[1])<<16 | (t.bytes[2])<<8 |  (t.bytes[3]))

#define FETCH_2_PAIRS(c,i, a1, a2) (\
	   RXT_PAIR == RL_GET_VALUE(c, i,   &a1) \
	&& RXT_PAIR == RL_GET_VALUE(c, i+1, &a2))

#define FETCH_3_PAIRS(c,i, a1, a2, a3) (\
	   RXT_PAIR == RL_GET_VALUE(c, i,   &a1) \
	&& RXT_PAIR == RL_GET_VALUE(c, i+1, &a2) \
	&& RXT_PAIR == RL_GET_VALUE(c, i+2, &a3))

#define RESOLVE_ARG(a) (type = RL_GET_VALUE_RESOLVED(cmds, index++, &arg[a]));

#define RESOLVE_ARG_PAIR(a) RESOLVE_ARG(a) \
	if (type != RXT_PAIR) goto error;
#define RESOLVE_ARG_NUMBER(a) RESOLVE_ARG(a) \
	if (!TYPE_IS_NUMBER) goto error;

#define RESOLVE_ARG_PAIR_OPTIONAL(a) RESOLVE_ARG(a) \
	if (type != RXT_PAIR) { type = 0; index--; };

#define RESOLVE_ARG_PAIR_DOUBLES(a, d) RESOLVE_ARG(a) \
	if (type != RXT_PAIR) goto error; \
	doubles[d]   = arg[a].pair.x; \
	doubles[d+1] = arg[a].pair.y;

#define RESOLVE_ARG_PAIR_DOUBLES_OPTIONAL(a, d) RESOLVE_ARG(a) \
	if (type == RXT_PAIR) { \
		doubles[d]   = arg[a].pair.x; \
		doubles[d+1] = arg[a].pair.y; \
	} else { type = 0; index--; }

#define RESOLVE_INT_ARG(a) RESOLVE_ARG(a) \
	if (type != RXT_INTEGER) goto error;

#define RESOLVE_INT_ARG_OPTIONAL(a) RESOLVE_ARG(a) \
	if (type != RXT_INTEGER) {type = 0; index--; }

#define RESOLVE_NUMBER_ARG(a, d) RESOLVE_ARG(a) \
	if (type == RXT_DECIMAL || type == RXT_PERCENT) doubles[d] = arg[a].dec64; \
	else if (type == RXT_INTEGER) doubles[d] = (double)arg[a].int64; \
	else goto error;

#define RESOLVE_NUMBER_ARG_OPTIONAL(a, d) RESOLVE_ARG(a) \
	if (type == RXT_DECIMAL || type == RXT_PERCENT) doubles[d] = arg[a].dec64; \
	else if (type == RXT_INTEGER) doubles[d] = (double)arg[a].int64; \
	else { type = 0; index--;}

#define RESOLVE_NUMBER_OR_PAIR_ARG(a, d) RESOLVE_ARG(a) \
	if (type == RXT_DECIMAL || type == RXT_PERCENT) doubles[d] = doubles[d+1] = arg[a].dec64; \
	else if (type == RXT_INTEGER) doubles[d] = doubles[d+1] = (double)arg[a].int64; \
	else if (type == RXT_PAIR) {\
		doubles[d]   = arg[a].pair.x; \
		doubles[d+1] = arg[a].pair.y; \
	} else goto error;

#define RESOLVE_STRING_ARG(a) RESOLVE_ARG(a) \
	if (type != RXT_STRING) goto error;

#define GET_PAIR(a, d) RESOLVE_ARG(a) \
	if (type != RXT_PAIR) goto error; \
	doubles[d]   = arg[a].pair.x; \
	doubles[d+1] = arg[a].pair.y;

#define DRAW_GEOMETRY(ctx, mode, path) \
	if (has_fill  ) blContextFillGeometry  (&ctx, mode, path);\
	if (has_stroke) blContextStrokeGeometry(&ctx, mode, path);

#define ARG_X(n) (arg[n].pair.x)
#define ARG_Y(n) (arg[n].pair.y)

#define OPT_WORD_FLAG(flag, name) if (fetch_word(cmds, index, agg_arg_words, &cmd) && cmd == name) { flag = TRUE; index++; }

#endif //AGG_UTILS_H
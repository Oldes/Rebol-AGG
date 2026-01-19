//   ____  __   __        ______        __
//  / __ \/ /__/ /__ ___ /_  __/__ ____/ /
// / /_/ / / _  / -_|_-<_ / / / -_) __/ _ \
// \____/_/\_,_/\__/___(@)_/  \__/\__/_// /
//  ~~~ oldes.huhuman at gmail.com ~~~ /_/
//
// Project: Rebol/AGG extension
// SPDX-License-Identifier: MIT
// =============================================================================

#ifndef AGG_REBOL_EXTENSION_H
#define AGG_REBOL_EXTENSION_H

#include "rebol-extension.h"
#include "agg-extension.h"

#define DOUBLE_BUFFER_SIZE 16
#define ARG_BUFFER_SIZE    8

#ifdef __cplusplus
#define COMMAND extern "C" int
#else
#define COMMAND int
#endif

#define SERIES_TEXT(s)   ((char*)SERIES_DATA(s))

#define MIN_REBOL_VER 3
#define MIN_REBOL_REV 14
#define MIN_REBOL_UPD 1
#define VERSION(a, b, c) (a << 16) + (b << 8) + c
#define MIN_REBOL_VERSION VERSION(MIN_REBOL_VER, MIN_REBOL_REV, MIN_REBOL_UPD)

typedef struct ctx_agg_t {
	// Some context to be used.
	float     volume;
	RXICBI    callback;
} ctx_agg_t;

extern REBCNT Handle_AGG;
extern u32* agg_cmd_words;
extern u32* agg_arg_words;
extern REBDEC doubles[DOUBLE_BUFFER_SIZE];
extern RXIARG arg[ARG_BUFFER_SIZE];


//#define USE_TRACES
#ifdef  USE_TRACES
#include <stdio.h>
#define debug_print(fmt, ...) do { printf(fmt, __VA_ARGS__); } while (0)
#define trace(str) puts(str)
#else
#define debug_print(fmt, ...)
#define trace(str) 
#endif

#endif //AGG_REBOL_EXTENSION_H

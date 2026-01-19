//   ____  __   __        ______        __
//  / __ \/ /__/ /__ ___ /_  __/__ ____/ /
// / /_/ / / _  / -_|_-<_ / / / -_) __/ _ \
// \____/_/\_,_/\__/___(@)_/  \__/\__/_// /
//  ~~~ oldes.huhuman at gmail.com ~~~ /_/
//
// SPDX-License-Identifier: MIT
// =============================================================================
// Rebol/AGG extension handles management
// =============================================================================

#include "agg-commands.h"

extern "C"
REBLEN Common_mold(REBHOB *hob, REBSER *str) {
	REBLEN len;
	if (!str) return 0;
	SERIES_TAIL(str) = 0;
	APPEND_STRING(str, "0#%lx", (unsigned long)(uintptr_t)hob->handle);
	return len;
}

extern "C"
int AGGHandle_free(void* hndl) {
	RXIARG  arg;
	REBSER *blk;
	REBHOB *hob;

	if (!hndl) return 0;
	hob = (REBHOB *)hndl;

	printf("release engine: %p is referenced: %i\n", hob->data, IS_MARK_HOB(hob) != 0);
	UNMARK_HOB(hob);
	blk = hob->series;
	if (blk) {
		int i = blk->tail;
		while (i-->0) {
			if (RXT_HANDLE == RL_GET_VALUE(blk, i, &arg)) {
				RL_FREE_HANDLE_CONTEXT((REBHOB*)arg.handle.ptr);
			}
		}
		RESET_SERIES(blk);
		hob->series = NULL;
	}
//	Release the context....
//	ctx = (ctx_agg_t*)hob->data;
//	some_release_function(ctx->something);
	return 0;
}

extern "C"
int AGGHandle_get_path(REBHOB *hob, REBCNT word, REBCNT *type, RXIARG *arg) {
//	ctx_agg_t* ctx = (ctx_agg_t*)hob->data;
//	word = RL_FIND_WORD(arg_words, word);
//	switch (word) {
//	case W_ARG_VOLUME:
//		*type = RXT_DECIMAL;
//		arg->dec64 = (REBDEC)ctx->volume;
//		break;
//	}
//	case W_ARG_RESOURCES:
//		*type = RXT_BLOCK;
//		arg->series = hob->series;
//		arg->index = 0;
//		break;
//	default:
		return PE_BAD_SELECT;	
//	}
//	return PE_USE;
}

extern "C"
int AGGHandle_set_path(REBHOB *hob, REBCNT word, REBCNT *type, RXIARG *arg) {
//	ctx_agg_t* ctx = (ctx_agg_t*)hob->data;
//	word = RL_FIND_WORD(arg_words, word);
//	switch (word) {
//	case W_ARG_VOLUME:
//		switch (*type) {
//		case RXT_DECIMAL:
//		case RXT_PERCENT:
//			ctx->volume = (float)arg->dec64;
//			break;
//		case RXT_INTEGER:
//			ctx->volume = (float)arg->int64;
//			break;
//		default: 
//			return PE_BAD_SET_TYPE;
//		}
//		break;
//	default:
		return PE_BAD_SET;	
//	}
//	return PE_OK;
}

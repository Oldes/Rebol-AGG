//   ____  __   __        ______        __
//  / __ \/ /__/ /__ ___ /_  __/__ ____/ /
// / /_/ / / _  / -_|_-<_ / / / -_) __/ _ \
// \____/_/\_,_/\__/___(@)_/  \__/\__/_// /
//  ~~~ oldes.huhuman at gmail.com ~~~ /_/
//
// SPDX-License-Identifier: MIT
// =============================================================================
// Rebol/AGG extension
// =============================================================================

#include "agg-rebol-extension.h"

RL_LIB *RL; // Link back to reb-lib from embedded extensions

//==== Globals ===============================================================//
extern MyCommandPointer Command[];
REBCNT Handle_AGG;
REBCNT Handle_AGGImage;

u32* agg_cmd_words;
u32* agg_arg_words;
REBDEC doubles[DOUBLE_BUFFER_SIZE];
RXIARG arg[ARG_BUFFER_SIZE];
//============================================================================//

static const char* init_block = AGG_EXT_INIT_CODE;

int Common_mold(REBHOB *hob, REBSER *ser);

int AGGHandle_free(void* hndl);
int AGGHandle_get_path(REBHOB *hob, REBCNT word, REBCNT *type, RXIARG *arg);
int AGGHandle_set_path(REBHOB *hob, REBCNT word, REBCNT *type, RXIARG *arg);


int cmd_init_words(RXIFRM *frm, void *ctx) {
	agg_cmd_words = RL_MAP_WORDS(RXA_SERIES(frm,1));
	agg_arg_words = RL_MAP_WORDS(RXA_SERIES(frm,2));
	return RXR_TRUE;
}

RXIEXT const char *RX_Init(int opts, RL_LIB *lib) {
	RL = lib;
	REBYTE ver[8];
	RL_VERSION(ver);
	debug_print(
		"RXinit agg-extension; Rebol v%i.%i.%i\n",
		ver[1], ver[2], ver[3]);

	if (MIN_REBOL_VERSION > VERSION(ver[1], ver[2], ver[3])) {
		debug_print(
			"Needs at least Rebol v%i.%i.%i!\n",
			 MIN_REBOL_VER, MIN_REBOL_REV, MIN_REBOL_UPD);
		return 0;
	}
	if (!CHECK_STRUCT_ALIGN) {
		trace("CHECK_STRUCT_ALIGN failed!");
		return 0;
	}

	REBHSP spec;
	spec.mold = Common_mold;

	spec.size      = sizeof(ctx_agg_t);
	spec.flags     = HANDLE_REQUIRES_HOB_ON_FREE;
	spec.free      = AGGHandle_free;
	spec.get_path  = AGGHandle_get_path;
	spec.set_path  = AGGHandle_set_path;
	Handle_AGG = RL_REGISTER_HANDLE_SPEC((REBYTE*)"agg-handle", &spec);

//	Handle_AGGImage = RL_REGISTER_HANDLE(b_cast("AGGImage"), sizeof(void*), releaseImage);

	return init_block;
}

RXIEXT int RX_Call(int cmd, RXIFRM *frm, void *ctx) {
	return Command[cmd](frm, ctx);
}

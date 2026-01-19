//   ____  __   __        ______        __
//  / __ \/ /__/ /__ ___ /_  __/__ ____/ /
// / /_/ / / _  / -_|_-<_ / / / -_) __/ _ \
// \____/_/\_,_/\__/___(@)_/  \__/\__/_// /
//  ~~~ oldes.huhuman at gmail.com ~~~ /_/
//
// Project: Rebol/AGG extension
// SPDX-License-Identifier: MIT
// =============================================================================
// NOTE: auto-generated file, do not modify!

#ifndef AGG_EXTENSION_H
#define AGG_EXTENSION_H

typedef int (*MyCommandPointer)(RXIFRM *frm, void *ctx);


int cmd_init_words(RXIFRM *frm, void *ctx);
int cmd_test(RXIFRM *frm, void *ctx);
int cmd_draw(RXIFRM *frm, void *ctx);

enum ext_commands {
	CMD_AGG_INIT_WORDS,
	CMD_AGG_TEST,
	CMD_AGG_DRAW,
};

enum agg_cmd_words_enum {W_CMD_0,
	W_CMD_MOVE,
	W_CMD_LINE,
	W_CMD_CURVE,
	W_CMD_CURV,
	W_CMD_CUBIC,
	W_CMD_QUAD,
	W_CMD_HLINE,
	W_CMD_VLINE,
	W_CMD_QCURVE,
	W_CMD_QCURV,
	W_CMD_ARC,
	W_CMD_MOVE_BY,
	W_CMD_LINE_BY,
	W_CMD_CURVE_BY,
	W_CMD_CURV_BY,
	W_CMD_CUBIC_BY,
	W_CMD_QUAD_BY,
	W_CMD_HLINE_BY,
	W_CMD_VLINE_BY,
	W_CMD_QCURVE_BY,
	W_CMD_QCURV_BY,
	W_CMD_ARC_BY,
	W_CMD_POLYGON,
	W_CMD_SHAPE,
	W_CMD_BOX,
	W_CMD_RECT,
	W_CMD_RECTANGLE,
	W_CMD_CIRCLE,
	W_CMD_ELLIPSE,
	W_CMD_IMAGE,
	W_CMD_TEXT,
	W_CMD_FILL_ALL,
	W_CMD_CLEAR,
	W_CMD_CLEAR_ALL,
	W_CMD_CLEAR_CLIP,
	W_CMD_POINT,
	W_CMD_POINT_SIZE,
	W_CMD_TRIANGLE,
	W_CMD_PEN,
	W_CMD_FILL,
	W_CMD_LINE_WIDTH,
	W_CMD_LINE_COLOR,
	W_CMD_LINE_CAP,
	W_CMD_LINE_JOIN,
	W_CMD_ALPHA,
	W_CMD_BLEND,
	W_CMD_COMPOSITE,
	W_CMD_ANTI_ALIAS,
	W_CMD_ARROW,
	W_CMD_MATRIX,
	W_CMD_RESET_MATRIX,
	W_CMD_INVERT_MATRIX,
	W_CMD_ROTATE,
	W_CMD_SCALE,
	W_CMD_SKEW,
	W_CMD_TRANSLATE,
	W_CMD_TRANSFORM,
	W_CMD_CLOSE,
	W_CMD_CLIP,
	W_CMD_FONT,
	W_CMD_PUSH,
	W_CMD_FILL_PEN
};

enum agg_arg_words_enum {W_ARG_0,
	W_ARG_PAD,
	W_ARG_TILE,
	W_ARG_FLIP,
	W_ARG_TILE_Y,
	W_ARG_FLIP_Y,
	W_ARG_TILE_X,
	W_ARG_TILE_X_FLIP_Y,
	W_ARG_FLIP_X,
	W_ARG_FLIP_X_TILE_Y,
	W_ARG_LINEAR,
	W_ARG_RADIAL,
	W_ARG_CONICAL,
	W_ARG_SOURCE_OVER,
	W_ARG_SOURCE_COPY,
	W_ARG_SOURCE_IN,
	W_ARG_SOURCE_OUT,
	W_ARG_SOURCE_ATOP,
	W_ARG_DESTINATION_OVER,
	W_ARG_DESTINATION_COPY,
	W_ARG_DESTINATION_IN,
	W_ARG_DESTINATION_OUT,
	W_ARG_DESTINATION_ATOP,
	W_ARG_XOR,
	W_ARG_CLEAR,
	W_ARG_PLUS,
	W_ARG_MINUS,
	W_ARG_MODULATE,
	W_ARG_MULTIPLY,
	W_ARG_SCREEN,
	W_ARG_OVERLAY,
	W_ARG_DARKEN,
	W_ARG_LIGHTEN,
	W_ARG_COLOR_DODGE,
	W_ARG_COLOR_BURN,
	W_ARG_LINEAR_BURN,
	W_ARG_LINEAR_LIGHT,
	W_ARG_PIN_LIGHT,
	W_ARG_HARD_LIGHT,
	W_ARG_SOFT_LIGHT,
	W_ARG_DIFFERENCE,
	W_ARG_EXCLUSION,
	W_ARG_ADD,
	W_ARG_MITER,
	W_ARG_BEVEL,
	W_ARG_ROUND,
	W_ARG_MITER_ROUND,
	W_ARG_MITER_REVERT,
	W_ARG_OPEN,
	W_ARG_PIE,
	W_ARG_CLOSED,
	W_ARG_CHORD,
	W_ARG_SWEEP,
	W_ARG_LARGE,
	W_ARG_BUTT,
	W_ARG_SQUARE,
	W_ARG_ROUND_IN,
	W_ARG_TRIANGLE,
	W_ARG_TRIANGLE_IN,
	W_ARG_RESOURCES
};

#define AGG_EXT_INIT_CODE \
	"REBOL [Title: \"Rebol AGG Extension\" Name: AGG Type: module Version: 0.1.0 Needs: 3.14.1 Author: Oldes Date: 1-Feb-2026/19:59:09 License: MIT Url: https://github.com/Oldes/Rebol-AGG]\n"\
	"init-words: command [cmd-words [block!] arg-words [block!]]\n"\
	"test: command [\"Just an example\" img [pair! image!]]\n"\
	"draw: command [\"Draws scalable vector graphics to an image\" image [image! pair!] commands [block!]]\n"\
	"init-words words: [move line curve curv cubic quad hline vline qcurve qcurv arc move-by line-by curve-by curv-by cubic-by quad-by hline-by vline-by qcurve-by qcurv-by arc-by polygon shape box rect rectangle circle ellipse image text fill-all clear clear-all clear-clip point point-size triangle pen fill line-width line-color line-cap line-join alpha blend composite anti-alias arrow matrix reset-matrix invert-matrix rotate scale skew translate transform close clip font push fill-pen] [pad tile flip tile-y flip-y tile-x tile-x-flip-y flip-x flip-x-tile-y linear radial conical source-over source-copy source-in source-out source-atop destination-over destination-copy destination-in destination-out destination-atop xor clear plus minus modulate multiply screen overlay darken lighten color-dodge color-burn linear-burn linear-light pin-light hard-light soft-light difference exclusion add miter bevel round miter-round miter-revert open pie closed chord sweep large butt square round-in triangle triangle-in resources]\n"\
	"protect/hide 'init-words\n"

#endif //AGG_EXTENSION_H

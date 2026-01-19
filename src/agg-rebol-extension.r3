REBOL [
	title:  "Rebol/AGG module builder"
	type:    module
	date:    12-Jan-2026
	home:    https://github.com/Oldes/Rebol-AGG
	version: 0.0.2
	author: @Oldes
]

;- all extension command specifications ----------------------------------------
commands: [
	init-words: [cmd-words [block!] arg-words [block!]] ;; used internaly only!
	test: ["Just an example" img [pair! image!]]
	;--------------------------

	draw: [
		{Draws scalable vector graphics to an image}
		image [image! pair!]
		commands [block!]
	]
]

ext-values: {}

handles: make map! [
	agg-handle: [
		"AGG handle"
		resources      block!  none                "example.."
	]
]

cmd-words: [
	;@@ order of these is important!
	move
	line
	curve
	curv
	cubic
	quad
	hline
	vline
	qcurve
	qcurv
	arc
	; relative variants
	move-by
	line-by
	curve-by
	curv-by
	cubic-by
	quad-by
	hline-by
	vline-by
	qcurve-by
	qcurv-by
	arc-by

	polygon
	shape
	box
	rect
	rectangle
	circle
	ellipse
	
	image
	text
	fill-all ; fills all context area
	clear
	clear-all
	clear-clip
	point
	point-size
	triangle

	pen
	fill
	line-width
	line-color
	line-cap
	line-join
	alpha
	blend     ; blend-mode?
	composite ; composite-mode?
	anti-alias
	arrow

	matrix
	reset-matrix
	invert-matrix
	rotate
	scale
	skew
	translate
	transform
	close

	clip

	font

	push
	;- aliases (for compatibility)
	fill-pen ; alias for `fill` 
]
arg-words: [
	;@@ order of these is important!
	;- pattern/gradient modes
	pad              ; BL_EXTEND_MODE_PAD = 0,
	tile             ; BL_EXTEND_MODE_REPEAT = 1,
	flip             ; BL_EXTEND_MODE_REFLECT = 2,
	tile-y           ; BL_EXTEND_MODE_PAD_X_REPEAT_Y = 3,
	flip-y           ; BL_EXTEND_MODE_PAD_X_REFLECT_Y = 4,
	tile-x           ; BL_EXTEND_MODE_REPEAT_X_PAD_Y = 5,
	tile-x-flip-y    ; BL_EXTEND_MODE_REPEAT_X_REFLECT_Y = 6,
	flip-x           ; BL_EXTEND_MODE_REFLECT_X_PAD_Y = 7,
	flip-x-tile-y    ; BL_EXTEND_MODE_REFLECT_X_REPEAT_Y = 8,
	;- gradient types:
	linear
	radial
	conical
	;- blend modes:
	source-over      ; BL_COMP_OP_SRC_OVER = 0,
 	source-copy      ; BL_COMP_OP_SRC_COPY = 1,
  	source-in        ; BL_COMP_OP_SRC_IN = 2,
	source-out       ; BL_COMP_OP_SRC_OUT = 3,
	source-atop      ; BL_COMP_OP_SRC_ATOP = 4,
	destination-over ; BL_COMP_OP_DST_OVER = 5,
	destination-copy ; BL_COMP_OP_DST_COPY = 6,
	destination-in   ; BL_COMP_OP_DST_IN = 7,
	destination-out  ; BL_COMP_OP_DST_OUT = 8,
	destination-atop ; BL_COMP_OP_DST_ATOP = 9,
	xor              ; BL_COMP_OP_XOR = 10,
	clear            ; BL_COMP_OP_CLEAR = 11,
	plus             ; BL_COMP_OP_PLUS = 12,
	minus            ; BL_COMP_OP_MINUS = 13,
	modulate         ; BL_COMP_OP_MODULATE = 14,
	multiply         ; BL_COMP_OP_MULTIPLY = 15,
	screen           ; BL_COMP_OP_SCREEN = 16,
	overlay          ; BL_COMP_OP_OVERLAY = 17,
	darken           ; BL_COMP_OP_DARKEN = 18,
	lighten          ; BL_COMP_OP_LIGHTEN = 19,
	color-dodge      ; BL_COMP_OP_COLOR_DODGE = 20,
	color-burn       ; BL_COMP_OP_COLOR_BURN = 21,
	linear-burn      ; BL_COMP_OP_LINEAR_BURN = 22,
	linear-light     ; BL_COMP_OP_LINEAR_LIGHT = 23,
	pin-light        ; BL_COMP_OP_PIN_LIGHT = 24,
	hard-light       ; BL_COMP_OP_HARD_LIGHT = 25,
	soft-light       ; BL_COMP_OP_SOFT_LIGHT = 26,
	difference       ; BL_COMP_OP_DIFFERENCE = 27,
	exclusion        ; BL_COMP_OP_EXCLUSION = 28,
	add
	;- line-join:
    miter        ; Sharp corners
    bevel        ; Bevel (cut-off corners)
    round        ; Round corners
    miter-round  ; Miter with rounded truncation
    miter-revert ; Miter with revert-to-bevel fallback
	;- arc types:
	open
	pie
	closed ; same as pie
	chord
	sweep
	large
	;------------------------------
    butt         ; Flat end perpendicular to line direction
    square       ; Square extension beyond endpoint
    round        ; Circular semicircle at endpoint
    round-in     ; Inner circular semicircle
    triangle     ; Triangle pointing away from endpoint
    triangle-in  ; Inner triangle

;	stroke-alignment
;	center
;	inner
;	outer

]


handles-doc: copy {}

foreach [name spec] handles [
	append handles-doc ajoin [
		LF LF "#### __" uppercase form name "__ - " spec/1 LF
		LF "```rebol"
		LF ";Refinement       Gets                Sets                          Description"
	]
	foreach [name gets sets desc] next spec [
		append handles-doc rejoin [
			LF
			#"/" pad name 17
			pad mold gets 20
			pad mold sets 30
			#"^"" desc #"^""
		]
		append arg-words name
	]
	append handles-doc "^/```"
]
;print handles-doc
arg-words: unique arg-words

type-words: [
	;@@ Order is important!
]


;-------------------------------------- ----------------------------------------
reb-code: ajoin [
	{REBOL [Title: "Rebol AGG Extension"}
	{ Name: AGG Type: module}
	{ Version: 0.1.0}
	{ Needs: 3.14.1} ;; May be higher if needed!
	{ Author: Oldes}
	{ Date: } now/utc
	{ License: MIT}
	{ Url: https://github.com/Oldes/Rebol-AGG} ;- Use real URL!
	#"]"
]

logo: next {
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
}

enu-commands:  "" ;; command name enumerations
cmd-declares:  "" ;; command function declarations
cmd-dispatch:  "" ;; command functionm dispatcher
agg-cmd-words: "enum agg_cmd_words_enum {W_CMD_0"
agg-arg-words: "enum agg_arg_words_enum {W_ARG_0"

ma-arg-words: "enum ma_arg_words {W_ARG_0"
ma-type-words: "enum ma_type_words {W_TYPE_0"

;- generate C and Rebol code from the command specifications -------------------
foreach [name spec] commands [
	append reb-code ajoin [lf name ": command "]
	new-line/all spec false
	append/only reb-code mold spec

	name: form name
	replace/all name #"-" #"_"
	replace/all name #"?" #"q"
	
	append enu-commands ajoin ["^/^-CMD_AGG_" uppercase copy name #","]

	append cmd-declares ajoin ["^/int cmd_" name "(RXIFRM *frm, void *ctx);"]
	append cmd-dispatch ajoin ["^-cmd_" name ",^/"]
]

;- additional Rebol initialization code ----------------------------------------
foreach word cmd-words [
	word: uppercase form word
	replace/all word #"-" #"_"
	append agg-cmd-words ajoin [",^/^-W_CMD_" word]
]
foreach word arg-words [
	word: uppercase form word
	replace/all word #"-" #"_"
	append agg-arg-words ajoin [",^/^-W_ARG_" word]
]
append agg-cmd-words "^/};"
append agg-arg-words "^/};"
append reb-code ajoin [{
init-words words: } mold/flat cmd-words #" " mold/flat arg-words {
protect/hide 'init-words}
]

;append reb-code {}

;print reb-code

;- convert Rebol code to C-string ----------------------------------------------
init-code: copy ""
foreach line split reb-code lf [
	replace/all line #"^"" {\"}
	append init-code ajoin [{\^/^-"} line {\n"}] 
]

;-- C file aggs -----------------------------------------------------------
header: {$logo
#ifndef AGG_EXTENSION_H
#define AGG_EXTENSION_H

typedef int (*MyCommandPointer)(RXIFRM *frm, void *ctx);

$cmd-declares
^/enum ext_commands {$enu-commands^/};
^/$agg-cmd-words
^/$agg-arg-words
^/#define AGG_EXT_INIT_CODE $init-code

#endif //AGG_EXTENSION_H
}
;;------------------------------------------------------------------------------
ctable: {$logo
#include "rebol-extension.h"
#include "agg-extension.h"
MyCommandPointer Command[] = {
$cmd-dispatch};
}

;- output generated files ------------------------------------------------------
write %agg-extension.h reword :header self
write %agg-commands-table.c  reword :ctable self



;; README documentation...
doc: clear ""
hdr: clear ""
arg: clear ""
cmd: desc: a: t: s: readme: r: none
parse commands [
	any [
		quote init-words: skip
		|
		set cmd: set-word! into [
			(clear hdr clear arg r: none)
			(append hdr ajoin [LF LF "#### `" cmd "`"])
			set desc: opt string!
			any [
				set a word!
				set t opt block!
				set s opt string!
				(
					unless r [append hdr ajoin [" `:" a "`"]]
					append arg ajoin [LF "* `" a "`"] 
					if t [append arg ajoin [" `" mold t "`"]]
					if s [append arg ajoin [" " s]]
				)
				|
				set r refinement!
				set s opt string!
				(
					append arg ajoin [LF "* `/" r "`"] 
					if s [append arg ajoin [" " s]]
				)
			]
			(
				append doc hdr
				append doc LF
				append doc any [desc ""]
				append doc arg
			)
		]
	]
]

try/except [
	readme: read/string %../README.md
	readme: clear find/tail readme "## Extension commands:"
	append readme ajoin [
		LF doc
		LF LF
;		LF "## Used handles and its getters / setters" 
;		handles-doc
;		LF LF
;		LF "## Other extension values:"
;		LF "```rebol"
;		trim/tail ext-values
;		LF "```"
;		LF
	]
	write %../README.md head readme
] :print



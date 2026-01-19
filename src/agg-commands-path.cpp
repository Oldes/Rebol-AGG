//   ____  __   __        ______        __
//  / __ \/ /__/ /__ ___ /_  __/__ ____/ /
// / /_/ / / _  / -_|_-<_ / / / -_) __/ _ \
// \____/_/\_,_/\__/___(@)_/  \__/\__/_// /
//  ~~~ oldes.huhuman at gmail.com ~~~ /_/
//
// SPDX-License-Identifier: MIT
// =============================================================================
// Rebol/AGG extension commands
// =============================================================================

#include "agg-commands.h"

//void* releasePath(void* path) {
//  //debug_print("releasing path: %p\n", path);
//  blPathDestroy(path);
//  return NULL;
//}

REBCNT agg_init_path_from_block(agg::path_storage *path, REBSER* cmds, REBCNT index) {
    REBCNT cmd, type;
    REBCNT cmd_pos = 0;
    bool sweepFlag = false;
    bool largeFlag = false;

    while (index < cmds->tail) {
        if (!fetch_word(cmds, index++, agg_cmd_words, &cmd)) {
            trace("expected word as a command!");
            goto error;
        }
    process_cmd: // label is used from error loop which skip all args until it reaches valid command

        cmd_pos = index; // this could be used to report error position
        //debug_print("cmd index: %u\n", index);
        switch (cmd) {

        ///////////////////////////////////////////////////////////////////////////
        /// Move path pen to absolute position
        case W_CMD_MOVE:
            RESOLVE_ARG_PAIR(0); /// X/Y coordinates
            path->move_to(ARG_PAIR(0));
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Move path pen by relative offset
        case W_CMD_MOVE_BY:
            RESOLVE_ARG_PAIR(0); /// Delta (dx, dy)
            path->move_rel(ARG_PAIR(0));
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Add one or more line segments to current path
        case W_CMD_LINE:
            RESOLVE_ARG_PAIR(0); /// First endpoint coordinate
            path->line_to(ARG_PAIR(0));
            /// Additional chained endpoints (optional)
            while (RXT_PAIR == RL_GET_VALUE(cmds, index, &arg[0])) {
                path->line_to(ARG_PAIR(0));
                index++;
            }
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Add one or more relative line segments to current path
        case W_CMD_LINE_BY:
            RESOLVE_ARG_PAIR(0); /// First delta from current position
            path->line_rel(ARG_PAIR(0));
            /// Additional chained deltas (optional)
            while (RXT_PAIR == RL_GET_VALUE(cmds, index, &arg[0])) {
                path->line_rel(ARG_PAIR(0));
                index++;
            }
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Add elliptical arc to path
        case W_CMD_ARC:
            RESOLVE_ARG_PAIR(3)   /// Arc endpoint coordinates
            RESOLVE_ARG_NUMBER(0) /// X-axis radius
            RESOLVE_ARG_NUMBER(1) /// Y-axis radius
            RESOLVE_ARG_NUMBER(2) /// Ellipse rotation angle in degrees
            sweepFlag = false;
            largeFlag = false;
            OPT_WORD_FLAG(sweepFlag, W_ARG_SWEEP);
            OPT_WORD_FLAG(largeFlag, W_ARG_LARGE);
            path->arc_to(ARG_NUMBER(0), ARG_NUMBER(1), AS_RADIANS(ARG_NUMBER(2)), largeFlag, sweepFlag, ARG_PAIR(3));
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Add elliptical arc with relative endpoint
        case W_CMD_ARC_BY:
            RESOLVE_ARG_PAIR(3)   /// Relative arc endpoint delta
            RESOLVE_ARG_NUMBER(0) /// X-axis radius
            RESOLVE_ARG_NUMBER(1) /// Y-axis radius
            RESOLVE_ARG_NUMBER(2) /// Ellipse rotation angle in degrees
            sweepFlag = false;
            largeFlag = false;
            OPT_WORD_FLAG(sweepFlag, W_ARG_SWEEP);
            OPT_WORD_FLAG(largeFlag, W_ARG_LARGE);
            path->arc_rel(ARG_NUMBER(0), ARG_NUMBER(1), AS_RADIANS(ARG_NUMBER(2)), largeFlag, sweepFlag, ARG_PAIR(3));
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Add one or more cubic Bézier curve4 segments
        case W_CMD_CURVE:
            // A cubic Bézier curve is defined by a start point, an end point, and two control points.
            while (FETCH_3_PAIRS(cmds, index, arg[1], arg[2], arg[3])) {
                index += 3;
                path->curve4(ARG_PAIR(1), ARG_PAIR(2), ARG_PAIR(3));
            }
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Add one or more relative cubic Bézier curve4 segments
        case W_CMD_CURVE_BY:
            while (FETCH_3_PAIRS(cmds, index, arg[1], arg[2], arg[3])) {
                index += 3;
                path->curve4_rel(ARG_PAIR(1), ARG_PAIR(2), ARG_PAIR(3));
            }
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Add one or more smooth cubic Bézier curve4 segments (relative controls)
        case W_CMD_CURV:
            while (FETCH_2_PAIRS(cmds, index, arg[1], arg[2])) {
                index += 2;
                path->curve4(ARG_PAIR(1), ARG_PAIR(2));
            }
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Add one or more smooth cubic Bézier curve4 segments (all relative)
        case W_CMD_CURV_BY:
            while (FETCH_2_PAIRS(cmds, index, arg[1], arg[2])) {
                index += 2;
                path->curve4_rel(ARG_PAIR(1), ARG_PAIR(2));
            }
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Add one or more quadratic Bézier curve3 segments
        case W_CMD_QCURVE:
            // A quadratic Bézier curve is defined by a start point, an end point, and one control point.
            while (FETCH_2_PAIRS(cmds, index, arg[1], arg[2])) {
                index += 2;
                path->curve3(ARG_PAIR(1), ARG_PAIR(2));
            }
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Add one or more relative quadratic Bézier curve3 segments
        case W_CMD_QCURVE_BY:
            while (FETCH_2_PAIRS(cmds, index, arg[1], arg[2])) {
                index += 2;
                path->curve3_rel(ARG_PAIR(1), ARG_PAIR(2));
            }
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Add one or more smooth quadratic Bézier curve3 segments
        case W_CMD_QCURV:
            while (RXT_PAIR == RL_GET_VALUE(cmds, index, &arg[0])) {
                index += 1;
                path->curve3(ARG_PAIR(0));
            }
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Add one or more smooth quadratic Bézier curve3 segments (relative)
        case W_CMD_QCURV_BY:
            while (RXT_PAIR == RL_GET_VALUE(cmds, index, &arg[0])) {
                index += 1;
                path->curve3_rel(ARG_PAIR(0));
            }
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Add horizontal line to path end X coordinate
        case W_CMD_HLINE:
            RESOLVE_ARG_NUMBER(0) /// X position (Y stays same)
            path->hline_to(ARG_NUMBER(0));
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Add horizontal line by relative delta X
        case W_CMD_HLINE_BY:
            RESOLVE_ARG_NUMBER(0) /// Delta X (Y stays same)
            path->hline_rel(ARG_NUMBER(0));
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Add vertical line to path end Y coordinate
        case W_CMD_VLINE:
            RESOLVE_ARG_NUMBER(0) /// Y position (X stays same)
            path->vline_to(ARG_NUMBER(0));
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Add vertical line by relative delta Y
        case W_CMD_VLINE_BY:
            RESOLVE_ARG_NUMBER(0) /// Delta Y (X stays same)
            path->vline_rel(ARG_NUMBER(0));
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Close current path subpath
        case W_CMD_CLOSE:
            path->close_polygon();
            break;


        default:
            debug_print("unknown command.. index: %u\n", index);
            goto error;
        } // switch end
        continue;
    error:
        // command errors does not stop evaluation... remaining commands may be processed..
        debug_print("CMD error at index... %u\n", cmd_pos);
        index = cmd_pos;
        // find next valid command name
        while (index < cmds->tail) {
            if (fetch_word(cmds, index++, agg_cmd_words, &cmd)) {
                goto process_cmd;
            }
        }
        return 1;
    } // while end
    return 0;
}

/*
TODO:
int cmd_path(RXIFRM* frm, void* reb_ctx) {
    debug_print("pathHandleId: %u\n", Handle_BLPath);
    REBHOB* hob = RL_MAKE_HANDLE_CONTEXT(Handle_BLPath);
    if (hob == NULL) {
        RXA_SERIES(frm,1) = "Blend2D failed to make a path handle!";
        return RXR_ERROR;
    }
    BLPathCore* path = (BLPathCore*)hob->data;
    debug_print("New path handle: %u data: %p\n", hob->sym, hob->data);
    blPathInit(path);

    b2d_init_path_from_block(path, RXA_SERIES(frm, 1), RXA_INDEX(frm, 1));

    hob->flags |= HANDLE_CONTEXT; //@@ temp fix!
    RXA_HANDLE(frm, 1) = hob;
    RXA_HANDLE_TYPE(frm, 1) = hob->sym;
    RXA_HANDLE_FLAGS(frm, 1) = hob->flags;
    RXA_TYPE(frm, 1) = RXT_HANDLE;
    return RXR_VALUE;
}
*/
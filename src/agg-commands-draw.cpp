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

void process_draw(Agg2D *gc, REBSER *cmds, REBCNT index) {
    REBCNT cmd_pos, cmd, type;
    Agg2D::ArcType   arc_type;
    Agg2D::LineJoin join_type;
    Agg2D::LineCap   cap_type;
    double rot, rx, ry, cx, cy, sx, sy, tx, ty;
    REBCNT word, idx, num;
    Agg2D::Transformations tr;
    REBSER *ser;


    //printf("index: %u, tail: %u\n", index, cmds->tail);
    while (index < cmds->tail) {
        if (!fetch_word(cmds, index++, agg_cmd_words, &cmd)) {
            trace("expected word as a command!");
            goto error;
        }
    process_cmd: // label is used from error loop which skip all args until it reaches valid command

        cmd_pos = index; // this could be used to report error position
        debug_print("cmd index: %u cmd: %u\n", index, cmd);
        switch (cmd) {
        ///////////////////////////////////////////////////////////////////////////
        case W_CMD_FILL:
        case W_CMD_FILL_PEN:
            // Set fill color or toggle for subsequent path rendering
            RESOLVE_ARG(0) /// color
            if (type == RXT_TUPLE) {
                gc->fillColor(AS_RGBA(arg[0])); // RGBA color value
            }
            else if (TYPE_IS_LOGIC) {
                gc->fillColor(ARG_BOOL(0));     // true=enabled false=disabled
            }
            else if (type == RXT_IMAGE) {
                REBYTE* pixels = ((REBSER*)arg[0].image)->data;
                int w = arg[0].width;
                int h = arg[0].height;
                gc->fillImage(pixels, w, h, w * PixelFormat::pix_width);
            }
            else if (type == RXT_LIT_WORD) {
                word = RL_FIND_WORD(agg_arg_words, arg[0].int32a);
                if (word == W_ARG_LINEAR) {
                    puts("TODO... linear gradient");
                }
                else goto error;
            }
            else goto error;
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Set stroke color or toggle for subsequent path rendering
        case W_CMD_PEN:
        case W_CMD_LINE_COLOR:
            RESOLVE_ARG(0) /// color
            if (type == RXT_TUPLE) {
                gc->lineColor(AS_RGBA(arg[0])); // RGBA color value   
            }
            else if (type == RXT_LOGIC || type == RXT_NONE) {
                gc->lineColor(ARG_BOOL(0));     // true=enabled false=disabled
            }
            else goto error;
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Set line width for subsequent path rendering
        case W_CMD_LINE_WIDTH:
            RESOLVE_ARG(0)
            /// Line thickness in pixels/world units
            if (type == RXT_INTEGER) {
                gc->lineWidth((double)ARG_INT64(0));
            }
            else if (type == RXT_DECIMAL || type == RXT_PERCENT) {
                gc->lineWidth(ARG_DOUBLE(0));
            }
            else goto error;
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Set line join style for subsequent path rendering
        case W_CMD_LINE_JOIN:

            if (fetch_word(cmds, index, agg_arg_words, &word)) {
                if (word == W_ARG_MITER) {
                    join_type = Agg2D::JoinMiter;  // Sharp corners
                }
                else if (word == W_ARG_BEVEL) {
                    join_type = Agg2D::JoinBevel;  // Bevel (cut-off corners)
                }
                else if (word == W_ARG_ROUND) {
                    join_type = Agg2D::JoinRound;  // Round corners
                }
                else if (word == W_ARG_MITER_ROUND) {
                    join_type = Agg2D::JoinMiterRound;  // Miter with rounded truncation
                }
                else if (word == W_ARG_MITER_REVERT) {
                    join_type = Agg2D::JoinMiterRevert;  // Miter with revert-to-bevel fallback
                }
                else goto error;
                index++;
            }
            else {
                RESOLVE_ARG(0)
                /// Raw numeric cap enum value
                if (type == RXT_INTEGER) {
                    join_type = static_cast<Agg2D::LineJoin>(ARG_INT64(0));
                }
                else goto error;
            }
            gc->lineJoin(join_type);
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Set line cap style for path endpoints
        case W_CMD_LINE_CAP:
            RESOLVE_ARG(0)
            /// Raw numeric cap enum value
            if (type == RXT_INTEGER) {
                cap_type = static_cast<Agg2D::LineCap>(ARG_INT64(0));
            }
            else if (type == RXT_WORD) {
                REBCNT word = ARG_WORD(0);
                if (word == W_ARG_BUTT) {
                    // Flat end perpendicular to line direction
                    cap_type = Agg2D::CapButt;
                }
                else if (word == W_ARG_SQUARE) {
                    // Square extension beyond endpoint
                    cap_type = Agg2D::CapSquare;
                }
                else if (word == W_ARG_ROUND) {
                    // Circular semicircle at endpoint
                    cap_type = Agg2D::CapRound;
                }
                else if (word == W_ARG_ROUND_IN) {
                    // Inner circular semicircle
                    cap_type = Agg2D::CapRoundIn;
                }
                else if (word == W_ARG_TRIANGLE) {
                    // Triangle pointing away from endpoint
                    cap_type = Agg2D::CapTriangle;
                }
                else if (word == W_ARG_TRIANGLE_IN) {
                    // Inner triangle
                    cap_type = Agg2D::CapTriangleIn;
                }
                else goto error;
            }
            else goto error;
            gc->lineCap(cap_type);
            // AGG cannot set different START/END caps like Blend2D,
            // so the optional second argument is ignored.
            RESOLVE_INT_ARG_OPTIONAL(1); // end cap
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Set master alpha for subsequent drawing operations
        case W_CMD_ALPHA:
            RESOLVE_ARG(0)
            /// Alpha multiplier (0.0=transparent, 1.0=opaque)
            if (TYPE_IS_NUMBER) {
                gc->masterAlpha(ARG_NUMBER(0));
            }
            else goto error;
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Set anti-aliasing gamma control
        case W_CMD_ANTI_ALIAS:
            RESOLVE_ARG(0)
            if (TYPE_IS_NUMBER) {
                gc->antiAliasGamma(ARG_NUMBER(0));
            }
            else if (TYPE_IS_LOGIC) {
                gc->antiAliasGamma(ARG_BOOL(0) ? 1.0 : 1000.0);   
            }
            else goto error;
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Set point size for subsequent point drawing operations
        case W_CMD_POINT_SIZE:
            /// Point radius or Diameter/extent
            RESOLVE_NUMBER_OR_PAIR_ARG(0, 0);
            gc->pointSize(doubles[0], doubles[1]);
            break;

        ///////////////////////////////////////////////////////////////////////////
        case W_CMD_ARROW:
            /// Point radius or Diameter/extent
            RESOLVE_NUMBER_OR_PAIR_ARG(0, 0);
            gc->pointSize(doubles[0], doubles[1]);
            break;

        ///////////////////////////////////////////////////////////////////////////
        case W_CMD_BLEND:
            RESOLVE_ARG(0)
            if (type == RXT_LIT_WORD) {
                REBCNT word = ARG_WORD(0);
                Agg2D::BlendMode mode;
                switch(word){
                case W_ARG_CLEAR:            mode = Agg2D::BlendClear; break;
                case W_ARG_SOURCE_COPY:      mode = Agg2D::BlendSrc; break;
                case W_ARG_DESTINATION_COPY: mode = Agg2D::BlendDst; break;
                case W_ARG_SOURCE_OVER:      mode = Agg2D::BlendSrcOver; break;
                case W_ARG_DESTINATION_OVER: mode = Agg2D::BlendDstOver; break;
                case W_ARG_SOURCE_IN:        mode = Agg2D::BlendSrcIn; break;
                case W_ARG_DESTINATION_IN:   mode = Agg2D::BlendDstIn; break;
                case W_ARG_SOURCE_OUT:       mode = Agg2D::BlendSrcOut; break;
                case W_ARG_DESTINATION_OUT:  mode = Agg2D::BlendDstOut; break;
                case W_ARG_SOURCE_ATOP:      mode = Agg2D::BlendSrcAtop; break;
                case W_ARG_DESTINATION_ATOP: mode = Agg2D::BlendDstAtop; break;
                case W_ARG_XOR:              mode = Agg2D::BlendXor; break;
                case W_ARG_ADD:              mode = Agg2D::BlendAdd; break;
                case W_ARG_MULTIPLY:         mode = Agg2D::BlendMultiply; break;
                case W_ARG_SCREEN:           mode = Agg2D::BlendScreen; break;
                case W_ARG_OVERLAY:          mode = Agg2D::BlendOverlay; break;
                case W_ARG_DARKEN:           mode = Agg2D::BlendDarken; break;
                case W_ARG_LIGHTEN:          mode = Agg2D::BlendLighten; break;
                case W_ARG_COLOR_DODGE:      mode = Agg2D::BlendColorDodge; break;
                case W_ARG_COLOR_BURN:       mode = Agg2D::BlendColorBurn; break;
                case W_ARG_HARD_LIGHT:       mode = Agg2D::BlendHardLight; break;
                case W_ARG_SOFT_LIGHT:       mode = Agg2D::BlendSoftLight; break;
                case W_ARG_DIFFERENCE:       mode = Agg2D::BlendDifference; break;
                case W_ARG_EXCLUSION:        mode = Agg2D::BlendExclusion; break;
                default: goto error;
                }
                gc->blendMode(mode);
            }
            else goto error;
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Draw open polyline from sequence of points (stroke only)
        case W_CMD_LINE:
        ///////////////////////////////////////////////////////////////////////////
        /// Draw closed polygon from sequence of points
        case W_CMD_POLYGON:    
            /// Flexible coordinate input supporting multiple points
            gc->resetPath();
            RESOLVE_ARG(0)
            /// Sequential pairs: moveTo first, lineTo rest
            if (type == RXT_PAIR) {
                gc->moveTo(ARG_PAIR(0));
                type = RL_GET_VALUE(cmds, index, &arg[0]);
                while (RXT_PAIR == type) {
                    gc->lineTo(ARG_PAIR(0));
                    type = RL_GET_VALUE(cmds, ++index, &arg[0]);
                }
            }
            /// Vector of pair coordinates
            else if (RXT_VECTOR == type) {
                ser = (REBSER*)arg[0].series;
                if (VECT_TYPE(ser) != VTSF64) goto error; // only double values are allowed
                idx = arg[0].index;
                num = (ser->tail - idx)  >> 1;   // 2 values per point
                REBDEC* data = (REBDEC*)ser->data + idx; // the value may not be at head
                gc->moveTo(data[0], data[1]);
                num--; data += 2;
                while(num-- > 0) {
                    gc->lineTo(data[0], data[1]);
                    data += 2;
                }
            }
            /// Block of pair coordinates
            else if (type == RXT_BLOCK) {
                ser = (REBSER*)arg[0].series;
                num = arg[0].index;
                type = RL_GET_VALUE(ser, num, &arg[0]);
                if (RXT_PAIR != type) goto error;
                gc->moveTo(ARG_PAIR(0));
                type = RL_GET_VALUE(ser, ++num, &arg[0]);
                while (RXT_PAIR == type) {
                    gc->lineTo(ARG_PAIR(0));
                    type = RL_GET_VALUE(ser, ++num, &arg[0]);
                }
            }
            else goto error;
            if (cmd == W_CMD_LINE) {
                gc->drawPath(Agg2D::StrokeOnly);
            }
            else {
                gc->closePolygon();
                gc->drawPath();
            }
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Draw cubic Bézier spline (moveTo + cubicCurveTo segments)
        case W_CMD_CUBIC:
            gc->resetPath();
            RESOLVE_ARG(0)
            /// Initial position (moveTo)
            if (type == RXT_PAIR) {
                gc->moveTo(ARG_PAIR(0));
            }
            else goto error;
            /// Sets of 3 pairs per cubic segment: control1 control2 end
            /// Sequential cubic Bézier segments
            while (FETCH_3_PAIRS(cmds, index, arg[1], arg[2], arg[3])) {
                gc->cubicCurveTo(ARG_PAIR(1), ARG_PAIR(2), ARG_PAIR(3));
                index += 3;
            }
            gc->drawPath(Agg2D::FillAndStroke);
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Draw rectangle with optional rounded corners
        case W_CMD_BOX:
            RESOLVE_ARG(0) /// Top-left corner
            if (type != RXT_PAIR) goto error;
            RESOLVE_ARG(1) /// Bottom-right corner
            if (type != RXT_PAIR) goto error;
            RESOLVE_ARG(2) /// Corner radius (defaults to 0=sharp)
            if (!TYPE_IS_NUMBER) {
                index--;
                gc->rectangle(ARG_PAIR(0), ARG_PAIR(1));
                break;
            }
            RESOLVE_ARG(3) /// Vertical corner radius (defaults to radius-x)
            if (!TYPE_IS_NUMBER) {
                index--;
                gc->roundedRect(ARG_PAIR(0), ARG_PAIR(1), ARG_NUMBER(2));
            }
            else {
                gc->roundedRect(ARG_PAIR(0), ARG_PAIR(1), ARG_NUMBER(2), ARG_NUMBER(3));
            }
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Draw rectangle with optional rounded corners
        case W_CMD_RECT:
        case W_CMD_RECTANGLE:
            RESOLVE_ARG(0) /// Top-left corner
            if (type != RXT_PAIR) goto error;
            RESOLVE_ARG(1) /// Width-height
            if (type != RXT_PAIR) goto error;
            tx = ARG_PAIR_X(0) + ARG_PAIR_X(1);
            ty = ARG_PAIR_Y(0) + ARG_PAIR_Y(1);
            RESOLVE_ARG(2) /// Corner radius (defaults to 0=sharp)
            if (!TYPE_IS_NUMBER) {
                index--;
                gc->rectangle(ARG_PAIR(0), tx, ty);
                break;
            }
            RESOLVE_ARG(3) /// Vertical corner radius (defaults to radius-x)
            if (!TYPE_IS_NUMBER) {
                index--;
                gc->roundedRect(ARG_PAIR(0), tx, ty, ARG_NUMBER(2));
            }
            else {
                gc->roundedRect(ARG_PAIR(0), tx, ty, ARG_NUMBER(2), ARG_NUMBER(3));
            }
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Draw ellipse/circle with optional elliptical radius
        case W_CMD_CIRCLE:
            RESOLVE_ARG(0) /// Ellipse center (x,y)
            if (type != RXT_PAIR) goto error;
            RESOLVE_ARG(1) /// Horizontal radius
            if (!TYPE_IS_NUMBER) goto error;
            rx = ARG_NUMBER(1);
            ry = rx;
            RESOLVE_ARG(2) /// Vertical radius (defaults to radius-x)
            if (TYPE_IS_NUMBER) {
                ry = ARG_NUMBER(2);
            } else index--;
            gc->ellipse(ARG_PAIR(0), rx, ry);        
            break;


        ///////////////////////////////////////////////////////////////////////////
        /// Draw ellipse defined by top-left corner and size
        case W_CMD_ELLIPSE:
            RESOLVE_ARG(0) /// Top-left bounding box corner
            if (type != RXT_PAIR) goto error;
            RESOLVE_ARG(1) /// Width/height of ellipse bounding box
            if (type != RXT_PAIR) goto error;
            rx = ARG_PAIR_X(1) * 0.5;
            ry = ARG_PAIR_Y(1) * 0.5;
            gc->ellipse(ARG_PAIR_X(0) + rx, ARG_PAIR_X(0) + ry, rx, ry);
            break;


        ///////////////////////////////////////////////////////////////////////////
        /// Draw one or more triangles from vertex coordinates
        case W_CMD_TRIANGLE:
            /// Flexible triangle vertex input (groups of 3 points)
            RESOLVE_ARG(0)
            /// Sequential triplets: triangle(p0,p1,p2) repeated
            if (type == RXT_PAIR) {
                index--;
                while (FETCH_3_PAIRS(cmds, index, arg[0], arg[1], arg[2])) {
                    gc->triangle(ARG_PAIR(0), ARG_PAIR(1), ARG_PAIR(2));
                    index += 3;
                }
            }
            /// Flattened x1 y1 x2 y2 x2 y3 ... (6 values per triangle)
            else if (type == RXT_VECTOR) {
                ser = (REBSER*)arg[0].series;
                if (VECT_TYPE(ser) != VTSF64) goto error; // only double values are allowed
                idx = arg[0].index;
                num = (REBCNT)floor((REBDEC)(ser->tail - idx) / 6); // 6 values per triangle (3 points)
                REBDEC* data = (REBDEC*)ser->data + idx;
                while (num-- > 0) {
                    gc->triangle(data[0], data[1], data[2], data[3], data[4], data[5]);
                    data += 6;
                }
            }
            /// Block of 3-pair triplets
            else if (type == RXT_BLOCK) {
                ser = (REBSER*)arg[0].series;
                idx = arg[0].index;
                while (FETCH_3_PAIRS(ser, idx, arg[0], arg[1], arg[2])) {
                    gc->triangle(ARG_PAIR(0), ARG_PAIR(1), ARG_PAIR(2));
                    idx += 3;
                }
            }
            else goto error;
            break;


        ///////////////////////////////////////////////////////////////////////////
        /// Draw elliptical arc (pie/chord capable)
        case W_CMD_ARC:
            puts("arc");
            RESOLVE_ARG(0) /// Center point of the ellipse
            if (type != RXT_PAIR) goto error;
            RESOLVE_ARG(1) /// Elliptical radius
            if (type != RXT_PAIR) goto error;
            RESOLVE_ARG(2) /// Starting angle in degrees
            if (!TYPE_IS_NUMBER) goto error;
            RESOLVE_ARG(3) /// Sweep angle in degrees
            if (!TYPE_IS_NUMBER) goto error;
            arc_type = Agg2D::ArcOpen; /// Open arc (no fill closure)
            if (fetch_word(cmds, index, agg_arg_words, &cmd)) {
                if (cmd == W_ARG_PIE) {
                    arc_type = Agg2D::ArcPie;   /// Pie slice (radial lines to center)
                }
                else if (cmd == W_ARG_CHORD) {
                    arc_type = Agg2D::ArcChord; /// Chord (connects arc ends)
                }
                else if (cmd != W_ARG_OPEN) goto error;
                index++;
            }
            gc->arc(ARG_PAIR(0), ARG_PAIR(1), AS_RADIANS(ARG_NUMBER(2)), AS_RADIANS(ARG_NUMBER(3)), arc_type);
            break;     

/*
        case W_CMD_ARC:
            RESOLVE_ARG_PAIR_DOUBLES(0, 0);  // center (cx, cy) -> doubles[0,1]
            RESOLVE_ARG_PAIR_DOUBLES(1, 2);  // radius (rx, ry) -> doubles[2,3]
            RESOLVE_NUMBER_ARG(2, 4); // begin angle -> doubles[4]
            RESOLVE_NUMBER_ARG(3, 5); // sweep angle -> doubles[5]

            TO_RADIANS(doubles[4]);  // start radians
            TO_RADIANS(doubles[5]);  // sweep radians

            // arc, pie, or chord?
            arc_type = Agg2D::ArcOpen;
            //type = RL_GET_VALUE(cmds, index, &arg[4]);
            if (fetch_word(cmds, index, agg_arg_words, &cmd) && 
                cmd >= W_ARG_PIE && cmd <= W_ARG_CHORD) {
                index++;
                arc_type = (cmd == W_ARG_CHORD) ? Agg2D::ArcChord : Agg2D::ArcPie;
            }
            gc->arc(ARG_PAIR(0), ARG_PAIR(1), doubles[4], doubles[5], arc_type);
            break;
*/


        ///////////////////////////////////////////////////////////////////////////
        /// Draw one or more points at specified coordinates
        /// Point size set by preceding point-size command
        case W_CMD_POINT:
            /// Flexible coordinate input supporting multiple points
            RESOLVE_ARG(0)
            /// One or more individual pair coordinates
            if (type == RXT_PAIR) {
                while (type == RXT_PAIR) {
                    gc->point(ARG_PAIR(0));
                    RESOLVE_ARG_PAIR_DOUBLES_OPTIONAL(0, 0);
                }
            }
            /// Vector of x/y coordinates (2 values per point)
            else if (type == RXT_VECTOR) {
                ser = (REBSER*)arg[0].series;
                if (VECT_TYPE(ser) != VTSF64) goto error; // only double values are allowed
                idx  = arg[0].index;
                num  = (ser->tail - idx) >> 1;   // 2 values per point
                REBDEC* data = (REBDEC*)ser->data + idx; // the value may not be at head
                while(num-- > 0) {
                    gc->point(data[0], data[1]);
                    data += 2;
                }
            }
            /// Block of pair coordinates
            else if (type == RXT_BLOCK) {
                ser = (REBSER*)arg[0].series;
                idx = arg[0].index;
                type = RL_GET_VALUE(ser, idx, &arg[0]);
                if (RXT_PAIR != type) goto error;
                while (RXT_PAIR == type) {
                    gc->point(ARG_PAIR(0));
                    type = RL_GET_VALUE(ser, ++idx, &arg[0]);
                }
            }
            else goto error;
            break;


        ///////////////////////////////////////////////////////////////////////////
        /// Translate coordinate system by offset vector
        case W_CMD_TRANSLATE:
            RESOLVE_ARG_PAIR(0)  /// Translation vector (dx, dy)
            gc->pre_translate(ARG_PAIR(0));
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Scale coordinate system
        case W_CMD_SCALE:
            RESOLVE_ARG(0)
            /// Uniform scaling (sx=sy)
            if (TYPE_IS_NUMBER) {
                gc->pre_scale(ARG_NUMBER(0), ARG_NUMBER(0));
            }
            /// Non-uniform scaling (sx, sy)
            else if (type == RXT_PAIR) {
                gc->pre_scale(ARG_PAIR(0));
            }
            else goto error;
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Apply skew/shear transformation
        case W_CMD_SKEW:
            RESOLVE_NUMBER_OR_PAIR_ARG(0, 0); /// Skew angles sx,sy in degrees
            RESOLVE_ARG_PAIR_DOUBLES_OPTIONAL(1, 2);
            if (type == RXT_PAIR) {
                gc->translate(-doubles[2], -doubles[3]);  // Move pivot to origin
                gc->skew(doubles[0], doubles[1]);         // Skew
                gc->translate(doubles[2], doubles[3]);    // Move back
            }
            else {
                gc->pre_skew(doubles[0], doubles[1]);
            }
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Apply rotation transformation
        case W_CMD_ROTATE:
            RESOLVE_NUMBER_ARG(0, 0); /// Rotation angle in degrees
            TO_RADIANS(doubles[0]);   /// converted to radians
            RESOLVE_ARG_PAIR_DOUBLES_OPTIONAL(1, 1);
            if (RXT_PAIR == type) {
                // Rotate around pivot point (cx, cy)
                // AGG: translate to pivot, rotate, translate back
                //printf("TR: %lf %lf\n", doubles[1], doubles[2]);
                gc->translate(-doubles[1], -doubles[2]); // Move pivot to origin
                gc->rotate(doubles[0]);                  // Rotate
                gc->translate(doubles[1], doubles[2]);   // Move back
            } else {
                gc->pre_rotate(doubles[0]);              // Rotate
            }
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Apply affine transformation relative to optional center
        case W_CMD_TRANSFORM:
            RESOLVE_ARG(0); // optional rotation/scale pivot point
            if (type == RXT_PAIR) {
                cx = ARG_PAIR_X(0);
                cy = ARG_PAIR_Y(0);
            } else {
                cx = cy = 0;
                index--;
            }
            RESOLVE_ARG_NUMBER(1); // Angle in degrees
            rot = AS_RADIANS(ARG_NUMBER(1));
            RESOLVE_ARG_NUMBER(2); // X scale factor
            sx = ARG_NUMBER(2);
            RESOLVE_ARG_NUMBER(3); // Y scale factor
            sy = ARG_NUMBER(3);
            RESOLVE_ARG_PAIR(4);   // Translation offset
            gc->transform(cx, cy, rot, sx, sy, ARG_PAIR(4));
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Set full 2D affine transformation matrix
        case W_CMD_MATRIX:
            RESOLVE_ARG(0);
            if (type == RXT_VECTOR) {
                ser = (REBSER*)arg[0].series;
                idx = arg[0].index;
                // Validate that vecor has at least 6 double values.
                if (VECT_TYPE(ser) != VTSF64 || (ser->tail - idx) < 6) goto error;
                double* data = (double*)ser->data + idx;
                std::copy(data, data + 6, tr.affineMatrix);
            }
            else if (type == RXT_BLOCK) {
                // Resolve 6 numbers [sx, shy, shx, sy, tx, ty] from the block
                ser = (REBSER*)arg[0].series;
                idx = arg[0].index;
                for(int i=0; i<6; i++, idx++) {
                    type = RL_GET_VALUE(ser, idx, &arg[0]);
                    if (!TYPE_IS_NUMBER) goto error;
                    tr.affineMatrix[i] = ARG_NUMBER(0);
                }
                
            } else goto error;
            gc->transformations(tr);
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Reset transformation matrix to identity
        case W_CMD_RESET_MATRIX:
            gc->resetTransformations();
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Invert current transformation matrix
        case W_CMD_INVERT_MATRIX:
            gc->invertTransformations();
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Set rectangular clipping region
        case W_CMD_CLIP:
            RESOLVE_ARG_PAIR(0); /// Top-left clip corner
            RESOLVE_ARG_PAIR(1); /// Bottom-right clip corner
            gc->clipBox(ARG_PAIR(0), ARG_PAIR(1));
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Clear entire surface to transparent black
        case W_CMD_CLEAR_ALL:
            gc->clearAll(0,0,0,0);
            break;

        case W_CMD_FILL_ALL:
            gc->fillAll();
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Clear clipping region with optional fill color
        case W_CMD_CLEAR_CLIP:
            RESOLVE_ARG(0)
            /// RGBA background color to fill before clearing
            if (type == RXT_TUPLE) {
                gc->clearClipBox(AS_RGBA(arg[0]));
            }
            /// Clear to transparent without fill (full surface)
            else {
                index--; // revert optional
                gc->clearClipBox(0,0,0,0);
            }
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Push state, execute block, then pop state (scoping group)
        case W_CMD_PUSH:
            RESOLVE_ARG(0)
            /// Drawing commands executed in isolated state
            if (type == RXT_BLOCK) {
                gc->pushState();
                process_draw(gc, (REBSER*)arg[0].series, arg[0].index);
                gc->popState();
            }
            else goto error;
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Load font from file path or string
        case W_CMD_FONT:
            RESOLVE_ARG(0)
            /// TODO: treat file and string input differently!
            if (type == RXT_FILE || type == RXT_STRING) {
                REBSER *file = (REBSER*)arg[0].series;
                gc->font(SERIES_TEXT(file), 14, false, false, Agg2D::VectorFontCache);
            }
            else goto error;
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Draw text at position with optional height
        case W_CMD_TEXT:
            RESOLVE_ARG_PAIR(0) /// X/Y coordinates
            RESOLVE_ARG(1)      /// Font height in pixels
            if (TYPE_IS_NUMBER) {
                gc->fontHeight(ARG_NUMBER(1));
            }
            else {index--;} // revert optional
            RESOLVE_ARG(2)      /// Text to render
            if (type == RXT_STRING) {
                gc->text(ARG_PAIR(0), SERIES_TEXT((REBSER*)arg[2].series));
            }
            else goto error;
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Define shape path from block of commands
        case W_CMD_SHAPE:
            RESOLVE_ARG(0)
            /// Path commands: move line curve close etc.
            if (type == RXT_BLOCK) {
                agg::path_storage path;
                path.remove_all();
                agg_init_path_from_block(&path, (REBSER*)arg[0].series, arg[0].index);
                gc->path(&path);
            }
            else goto error;
            break;

        ///////////////////////////////////////////////////////////////////////////
        /// Blit image at position or scaled to destination rect
        case W_CMD_IMAGE:
            RESOLVE_ARG(0);
            if (type == RXT_IMAGE) {
                
                RESOLVE_ARG_PAIR(1); // position

                REBYTE* pixels = ((REBSER*)arg[0].image)->data;
                int w = arg[0].width;
                int h = arg[0].height;

                Agg2D::Image image(pixels, w, h, w * PixelFormat::pix_width);
                image.premultiply();
                
                // Check for scaled rect
                type = RL_GET_VALUE(cmds, index, &arg[2]);
                if (type == RXT_PAIR || type == RXT_PERCENT) {
                    double dstX1 = ARG_PAIR_X(1);
                    double dstY1 = ARG_PAIR_Y(1);
                    double dstX2;
                    double dstY2;
                    if (type == RXT_PERCENT) {
                        dstX2 = dstX1 + ARG_DOUBLE(2) * w;
                        dstY2 = dstY1 + ARG_DOUBLE(2) * h;
                    }
                    else {
                        dstX2 = dstX1 + ARG_PAIR_X(2);
                        dstY2 = dstY1 + ARG_PAIR_Y(2); 
                    }
                    gc->transformImage(image, 0,0,w,h, dstX1, dstY1, dstX2, dstY2);
                    index++;
                } else {
                    gc->blendImage(image, ARG_PAIR(1), (int)(gc->masterAlpha()*255)); // position only
                }
            }
            else goto error;
            break;




        default:
            debug_print("unknown command.. index: %u\n", index);
            goto error;
        } // switch end
        continue;
    error:
        // command errors does not stop evaluation... remaining commands may be processed..
        //TODO: these errors could be collected in blend2d module as warnings instead of this debug print!
        debug_print("CMD error at index... %u\n", cmd_pos);
        index = cmd_pos;
        // find next valid command name
        while (index < cmds->tail) {
            if (fetch_word(cmds, index++, agg_cmd_words, &cmd)) {
                goto process_cmd;
            }
        }
    } // while end
}

int cmd_draw(RXIFRM *frm, void *reb_ctx) {
    REBXYF size;
    REBINT w, h;
    REBSER *reb_img = 0;

    if (RXA_TYPE(frm, 1) == RXT_PAIR) {
        size = RXA_PAIR(frm, 1);
        w = ROUND_TO_INT(size.x);
        h = ROUND_TO_INT(size.y);
        reb_img = (REBSER *)RL_MAKE_IMAGE(w,h);
    }
    else {
        w = RXA_IMAGE_WIDTH(frm, 1);
        h = RXA_IMAGE_HEIGHT(frm, 1);
        reb_img = (REBSER *)RXA_ARG(frm,1).image;
    }

    RXA_TYPE(frm, 1) = RXT_IMAGE;
    RXA_ARG(frm, 1).width  = w;
    RXA_ARG(frm, 1).height = h;
    RXA_ARG(frm, 1).image = reb_img;

    const int pixelSize = PixelFormat::pix_width;

    Agg2D gc;
    gc.attach(SERIES_DATA(reb_img), w, h, w * pixelSize);
    gc.flipText(true);

    process_draw(&gc, RXA_SERIES(frm, 2), RXA_INDEX(frm, 2));

    return RXR_VALUE;
}
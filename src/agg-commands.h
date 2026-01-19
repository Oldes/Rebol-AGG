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

#include <cstdint>
extern "C" {
#include <stdio.h>
#include "agg-rebol-extension.h"
#include "agg-utils.h"
}

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4244)  // Disable C4244: conversion possible loss of data
#pragma warning(disable: 4267)  // Disable C4267: conversion from 'size_t' to 'unsigned int'
#endif

#include <agg_ellipse.h>
#include <agg_pixfmt_rgba.h>
#include <agg_rasterizer_scanline_aa.h>
#include <agg_renderer_base.h>
#include <agg_renderer_scanline.h>
#include <agg_scanline_p.h>
#include <agg_rounded_rect.h>
#include <agg_conv_stroke.h>
#include <agg_conv_transform.h>
#include <agg_span_allocator.h>
#include <agg_span_interpolator_linear.h>
#include <agg_span_gradient.h>
#include <agg_path_storage.h>
#include <agg_conv_stroke.h>
#include <agg_blur.h>
#include <agg_path_storage.h>
#include "agg2d.h"

#ifdef _WIN32
#include "agg_font_win32_tt.h"
typedef agg::font_engine_win32_tt_int32 FontEngine;
#else
#include "agg_font_freetype.h"
typedef agg::font_engine_freetype_int32 FontEngine;
#endif

#ifdef _WIN32
#pragma warning(pop)
#endif

#define AGG_NAMESPACE_START namespace agg {
#define AGG_NAMESPACE_END }


typedef agg::pixfmt_bgra32  PixelFormat;
typedef agg::renderer_base<PixelFormat> RendererBaseType;



#define FRM_IS_HANDLE(n, t)   (RXA_TYPE(frm,n) == RXT_HANDLE && RXA_HANDLE_TYPE(frm, n) == t)
#define ARG_Double(n)         RXA_DEC64(frm,n)
#define ARG_Float(n)          (float)RXA_DEC64(frm,n)
#define ARG_Int32(n)          RXA_INT32(frm,n)
#define ARG_Handle_Series(n)  RXA_HANDLE_CONTEXT(frm, n)->series;

#define RETURN_HANDLE(hob)                   \
    RXA_HANDLE(frm, 1)       = hob;          \
    RXA_HANDLE_TYPE(frm, 1)  = hob->sym;     \
    RXA_HANDLE_FLAGS(frm, 1) = hob->flags;   \
    RXA_TYPE(frm, 1) = RXT_HANDLE;           \
    return RXR_VALUE

#define RETURN_ERROR(err)  do {RXA_SERIES(frm, 1) = err; return RXR_ERROR;} while(0)

#define APPEND_STRING(str, ...) \
    len = snprintf(NULL,0,__VA_ARGS__);\
    if (len > SERIES_REST(str)-SERIES_LEN(str)) {\
        RL_EXPAND_SERIES(str, SERIES_TAIL(str), len);\
        SERIES_TAIL(str) -= len;\
    }\
    len = snprintf( \
        SERIES_TEXT(str)+SERIES_TAIL(str),\
        SERIES_REST(str)-SERIES_TAIL(str),\
        __VA_ARGS__\
    );\
    SERIES_TAIL(str) += len;


REBCNT agg_init_path_from_block(agg::path_storage *path, REBSER* cmds, REBCNT index);

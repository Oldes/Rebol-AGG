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


template<class SrcPixelFormat>
 void
 drawIcon(SrcPixelFormat& pixFmt, int x, int y, int width, int height)
 {
     agg::rendering_buffer   renBuffer;
     SrcPixelFormat dstPixFmt (renBuffer);
     dstPixFmt.attach (pixFmt, x, y , x + width - 1, y + height - 1);
     agg::renderer_base<SrcPixelFormat>  rBase (dstPixFmt);
 
     agg::rasterizer_scanline_aa<> ras;
     ras.auto_close(false);
     agg::scanline_p8 scanline;
 
     // draw a purple circle
     {
         agg::ellipse circle (100, 100, 70, 70);
 
         agg::conv_stroke<agg::ellipse> strokePath (circle);
 
         double strokeWidth = 10.0;
         strokePath.width(strokeWidth);
         strokePath.line_cap(agg::butt_cap);
         strokePath.line_join(agg::miter_join);
         strokePath.miter_limit(strokeWidth);
 
         ras.reset();
         ras.add_path(strokePath);
         const agg::rgba8 purpleColor (0xee, 0, 0xcc, 0xcc);
         agg::render_scanlines_aa_solid(ras, scanline, rBase, purpleColor);
     }
 
     // draw a gradient button
     {
         typedef agg::gradient_linear_color<agg::rgba8> ColorFunctionType;
         typedef agg::gradient_x GradientFunctionType;
 
         typedef agg::span_interpolator_linear<agg::trans_affine> InterpolatorType;
         typedef agg::span_allocator<agg::rgba8> SpanAllocatorType;
         typedef agg::span_gradient<agg::rgba8,
                                    InterpolatorType,
                                    GradientFunctionType,
                                    ColorFunctionType> SpanGradientType;
 
         agg::ellipse circle (30, 30, 30, 30);
 
         GradientFunctionType gradientFunction;
         agg::trans_affine matrix;
         matrix.translate (70, 70);
 
         agg::trans_affine invertMatrix (matrix);
         invertMatrix.invert();
         InterpolatorType interpolator (invertMatrix);
 
         ColorFunctionType colorFunction (agg::rgba8 (0xff, 0xd7, 0, 0xff),
                                          agg::rgba8 (0xff, 0, 0, 0xff),
                                          60);
 
         SpanGradientType sg (interpolator, gradientFunction, colorFunction, 0.0, 60.0);
         SpanAllocatorType sa;
 
         agg::conv_transform<agg::ellipse> tp(circle, matrix);
         ras.reset ();
         ras.add_path(tp);
 
         agg::render_scanlines_aa(ras, scanline, rBase, sa, sg);
     }
 }

 template<class PixFormat, class BlurType>
 void
 blurImage(PixFormat& pixFmt, int x, int y, int width, int height, BlurType& blur, double radius)
 {
     agg::rendering_buffer   renBuffer;
     PixFormat subPixFmt (renBuffer);
     subPixFmt.attach (pixFmt, x, y , x + width - 1, y + height - 1);
     blur.blur (subPixFmt, radius);
 }

 void
 drawText (const char* str,
           double x,
           double y,
           bool kerning,
           FontEngine& fontEngine,
           agg::font_cache_manager<FontEngine>& fontCache,
           agg::rgba8 color,
           RendererBaseType& rBase)
 {
     bool first = true;
 
     // these three are only needed for outline rendering
     agg::conv_curve<FontEngine::path_adaptor_type> curves(fontCache.path_adaptor());
     agg::rasterizer_scanline_aa<> ras;
     agg::scanline_p8 scanline;

     double ox = x;
 
     for (; *str != 0; ++str)
     {
         unsigned ch = *str;
         if (ch == '\n') {
         	x = ox;
         	y += fontEngine.height();// + fontEngine.line_extra();
         	continue;
         }
         const agg::glyph_cache* glyph = fontCache.glyph(ch);
         if (glyph)
         {
             if (first)
             {
                 first = false;
             }
             else
             {
                 if (kerning)
                 {
                     fontCache.add_kerning(&x, &y);
                 }
             }
             fontCache.init_embedded_adaptors(glyph, x, y);
             switch (glyph->data_type)
             {
                 case agg::glyph_data_mono:
                 {
                     agg::render_scanlines_bin_solid(fontCache.mono_adaptor(), fontCache.mono_scanline(), rBase, color);
                     break;
                 }
                 case agg::glyph_data_gray8:
                 {
                 	//agg::render_scanlines(fontCache.gray8_adaptor(), fontCache.gray8_scanline(), rBase);
                     agg::render_scanlines_aa_solid(fontCache.gray8_adaptor(), fontCache.gray8_scanline(), rBase, color);
                     break;
                 }
                 case agg::glyph_data_outline:
                 {
                     ras.reset();
                     ras.add_path(curves);
                     agg::render_scanlines_aa_solid(ras, scanline, rBase, color);
                     break;
                 }
                 default:
                 {
                     printf("Unknown glyph type: %d", glyph->data_type);
                     return;
                 }
             }
             x += glyph->advance_x;
             y += glyph->advance_y;
         }
     }
 }

#ifdef _WIN32
int CALLBACK enum_fonts_proc(ENUMLOGFONTEXA* elf, NEWTEXTMETRICEXA* ntm, int type, LPARAM lParam)
{
    printf("Font: \"%s\" (charset=%d)\n", elf->elfLogFont.lfFaceName, elf->elfLogFont.lfCharSet);
    return 1;  // Continue enumeration
}
#endif


// Sample image data (4x4 RGB pixels, repeat as pattern)
static agg::int8u g_image[] = {
    255,0,0,  0,255,0,  0,0,255,  255,255,0,   // Row 1
    0,255,255,255,0,255,255,255,0, 0,0,255,    // Row 2
    255,0,255,0,0,0,   255,255,255,255,0,0,   // Row 3
    0,255,0,  255,0,0,  0,0,255,  255,255,255  // Row 4
};



COMMAND cmd_test_old(RXIFRM *frm, void *ctx) {
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
 
         agg::rendering_buffer   renderBuffer;
         PixelFormat             pixFmt;
         RendererBaseType        rBase;
 
         unsigned char *imageBuffer = SERIES_DATA(reb_img);
 
         // Hook everything up
         renderBuffer.attach (imageBuffer, w, h, w * pixelSize);
         pixFmt.attach(renderBuffer);
         rBase.attach(pixFmt);
 
         const agg::rgba8 transparentWhiteColor (0xff, 0xff, 0xff, 0);
         const agg::rgba8 greenColor (0, 0xff, 0, 0xff);
         const agg::rgba8 blueColor (0, 0, 0xff, 0xff);
         const agg::rgba8 redColor (0xff, 0, 0, 0xff);
 
         // clear the buffer with transparent white color
         rBase.clear(transparentWhiteColor);
 
         // The code below simply illustrates that it is very straightforward
         // to directly manipulate the image buffer.  That said, one hardly
         // ever needs to do so.
 
         // top border
         for (int x = 0; x < w; ++x)
         {
             rBase.copy_pixel(x, 0, greenColor);
         }
         // bottom border
         for (int x = 0; x < w; ++x)
         {
             rBase.copy_pixel(x, h - 1, greenColor);
         }
         // left border
         for (int y = 0; y < h; ++y)
         {
             rBase.copy_pixel(0, y, greenColor);
         }
         // right border
         for (int y = 0; y < h; ++y)
         {
             rBase.copy_pixel(w - 1, y, greenColor);
         }


         agg::rasterizer_scanline_aa<> ras;
         agg::scanline_p8 scanline;

         {
             agg::rounded_rect rect1 (5, 5, 195, 195, 20);
             ras.reset();
             ras.add_path(rect1);
             agg::render_scanlines_aa_solid(ras, scanline, rBase, blueColor);

             agg::ellipse eliipse (100, 100, 50, 80);
             ras.reset();
             ras.add_path(eliipse);
             agg::render_scanlines_aa_solid(ras, scanline, rBase, redColor);

         }
         {
        	drawIcon (pixFmt, 0, 0, 200, 200);
            agg::recursive_blur<agg::rgba8, agg::recursive_blur_calc_rgba<> > blur;
            blurImage (pixFmt, 10, 10, 180, 180, blur, 10);
        }

        {
		    // Circle path (center 200,200, radius 150)
		    agg::ellipse circle(200.0, 200.0, 150.0, 150.0);

        }

        {
#ifdef _WIN32
	    // Create screen DC for font engine
	    HDC dc_screen = GetDC(NULL);
	    FontEngine fontEngine(dc_screen);
// List font names...
//EnumFontFamiliesA(dc_screen, NULL, (FONTENUMPROCA)enum_fonts_proc, 0);
#else
	    FontEngine fontEngine;
#endif
         agg::font_cache_manager<FontEngine> fontCache (fontEngine);

         char text[] = "Hello\nWorld!";
         double fontSize = 100;
         bool fontHint = false;
         bool fontKerning = false;

         fontEngine.hinting(fontHint);
         fontEngine.height(fontSize);
        // fontEngine.italic(true);

         // mono
         {
#ifdef _WIN32
             if (!fontEngine.create_font("Arial", agg::glyph_ren_agg_gray8))
#else
             if (!fontEngine.load_font("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 0, agg::glyph_ren_agg_gray8))
#endif
             {
                 trace ("Font is not found.");

             } else {

             	

	             // enable font hint
	             fontEngine.hinting(fontHint);
	             // flip y since the image coordinate system has the origin at
	             // top left corner and y axis is flipped
	             fontEngine.flip_y(true);
	             fontEngine.height(fontSize);

	             agg::trans_affine mtx;
	        	//mtx *= agg::trans_affine_skewing(-0.3, 0);
	        	//mtx *= agg::trans_affine_rotation(agg::deg2rad(20.0));
	        	//mtx *= agg::trans_affine_scaling(3.0, 3.0);
	        	fontEngine.transform(mtx);

fontCache.precache(' ', 127);

	             drawText (text,
	                       10, 250,
	                       fontKerning,
	                       fontEngine,
	                       fontCache,
	                       redColor,
	                       rBase);
	        }
         }
#ifdef _WIN32
         ReleaseDC(NULL, dc_screen);
#endif
 
        }
	return RXR_VALUE;
}

COMMAND cmd_test(RXIFRM *frm, void *ctx) {
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

    unsigned char *imageBuffer = SERIES_DATA(reb_img);

    const int pixelSize = PixelFormat::pix_width;
 
    Agg2D m_graphics;
    m_graphics.attach (imageBuffer, w, h, w * pixelSize);
    m_graphics.clearAll(255, 255, 0);

    //m_graphics.scale(0.5, 0.5);
    // Rounded Rect
    m_graphics.lineColor(0, 0, 0);
    m_graphics.noFill();
    m_graphics.roundedRect(0.5, 0.5, 200-0.5, 200-0.5, 20.0);

    //m_graphics.scale(1, 1);

#ifdef _WIN32
    m_graphics.font("Arial", 14, false, false);
#else
    m_graphics.font("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 14, false, false);
#endif
    m_graphics.flipText(true);
    m_graphics.fillColor(0, 0, 0);
    m_graphics.noLine();
    m_graphics.text(100, 20, "Regular Raster Text -- Fast, but can't be");
    return RXR_VALUE;
}
// Danzeff-G - A complete rewrite of the Danzeff OSK library.
//
// This work is BSD-licensed. See LICENSE for more details.
//
// Please report bugs at : geecko.dev@free.fr

#include "danzeff.h"
#include <glib2d.h>
#include <pspkernel.h>

#define TEX_N 8
#define MODE_N 2
#define BASE_DIR "res/"

static bool d_holding = false;
static bool d_dirty = true;
static bool d_shifted = false;
static unsigned int d_mode = DANZEFF_LETTERS;
static bool d_mode_lock = false;
static bool d_init = false;
//static bool d_rotated = false;
static int d_selected_x = 1;
static int d_selected_y = 1;

g2dImage *d_tex[TEX_N] = {0};

static char d_path[TEX_N][32] =
{
    BASE_DIR "keys.png",   BASE_DIR "keys_s.png",
    BASE_DIR "keys_c.png", BASE_DIR "keys_s_c.png",
    BASE_DIR "nums.png",   BASE_DIR "nums_s.png",
    BASE_DIR "nums_c.png", BASE_DIR "nums_s_c.png"
};

static char d_layout[4][3][3][5] =
{
    {
        { ",abc", ".def"   , "!ghi" },
        { "-jkl", "\010m n", "?opq" },
        { "(rst", ":uvw"   , ")xyz" }
    },

    {
        { "^ABC", "@DEF"   , "*GHI"  },
        { "_JKL", "\010M N", "\"OPQ" },
        { "=RST", ";UVW"   , "/XYZ"  }
    },

    {
        { "\0\0\0001", "\0\0\0002", "\0\0\0003" },
        { "\0\0\0004", "\010\0 5" , "\0\0\0006" },
        { "\0\0\0007", "\0\0\0008", "\0\00009"  }
    },

    {
        { ",(.)", "\"<'>"    , "-[_]"  },
        { "!{?}", "\010\0 \0", "+\\=/" },
        { ":@;#", "~$`%"     , "*^|&"  }
    }
};

void danzeffInit()
{
    if (d_init) return;

    int i;

    g2dInit();

    for (i=0; i<TEX_N; i++)
    {
        d_tex[i] = g2dTexLoad(d_path[i], G2D_SWIZZLE);
    }
    
    d_init = true;
}


void danzeffTerm()
{
    if (!d_init) return;
    
    int i;

    for (i=0; i<TEX_N; i++)
    {
        g2dTexFree(&d_tex[i]);
    }
    
    d_init = false;
}


DanzeffChar danzeffRead(SceCtrlData pad)
{
    int x, y, choice;
    DanzeffChar dchar;

    x = 1 - (pad.Lx < 85) + (pad.Lx > 170);
    y = 1 - (pad.Ly < 85) + (pad.Ly > 170);

    if (d_selected_x != x || d_selected_y != y)
    {
        d_dirty = true;
        d_selected_x = x;
        d_selected_y = y;
    }
    if ((!d_shifted &&  (pad.Buttons & PSP_CTRL_RTRIGGER)) ||
        ( d_shifted && !(pad.Buttons & PSP_CTRL_RTRIGGER)))
    {
        d_dirty = true;
        d_shifted = !d_shifted;
    }

    dchar = 0;

    if (!d_holding)
    {
        if (pad.Buttons &
            (PSP_CTRL_CROSS|PSP_CTRL_CIRCLE|PSP_CTRL_TRIANGLE|PSP_CTRL_SQUARE))
        {
            choice = 0;
            if      (pad.Buttons & PSP_CTRL_TRIANGLE)
                choice = 0;
            else if (pad.Buttons & PSP_CTRL_SQUARE)
                choice = 1;
            else if (pad.Buttons & PSP_CTRL_CROSS)
                choice = 2;
            else if (pad.Buttons & PSP_CTRL_CIRCLE)
                choice = 3;

            dchar = d_layout[2*d_mode + d_shifted][y][x][choice];
        }
        else if (pad.Buttons & PSP_CTRL_LEFT)
        {
            dchar = DANZEFF_LEFT;
        }
        else if (pad.Buttons & PSP_CTRL_RIGHT)
        {
            dchar = DANZEFF_RIGHT;
        }
        else if (pad.Buttons & PSP_CTRL_SELECT)
        {
            dchar = DANZEFF_SELECT;
        }
        else if (pad.Buttons & PSP_CTRL_START)
        {
            dchar = DANZEFF_START;
        }
        else if (pad.Buttons & PSP_CTRL_LTRIGGER && !d_mode_lock)
        {
            d_mode = (d_mode+1) % MODE_N;
        }
    }

    d_holding = pad.Buttons & ~PSP_CTRL_RTRIGGER;

    return dchar;
}


void danzeffRender(int x, int y)
{
    g2dColor color;

    d_dirty = false;
    color = WHITE;
    
    if (d_selected_x != 1 || d_selected_y != 1)
    {
        color = G2D_MODULATE(color, 255, 128);
    }

    g2dBeginRects(d_tex[4*d_mode + 2*d_shifted]);
    {
        g2dSetColor(color);
        g2dSetCoordXY(x, y);
        g2dAdd();
    }
    g2dEnd();
    
    g2dBeginRects(d_tex[4*d_mode + 2*d_shifted + 1]);
    {
        g2dSetCoordXY(x + d_selected_x*43, y + d_selected_y*43);
        g2dSetCropXY(d_selected_x*64, d_selected_y*64);
        g2dSetCropWH(64, 64);
        g2dSetScaleWH(64, 64);
        g2dAdd();
    }
    g2dEnd();
}


//void danzeffSetRotated(bool rotated);


void danzeffSetMode(unsigned int mode)
{
    if (mode >= MODE_N) return;

    d_mode = mode;
}


void danzeffSetModeLock(bool lock)
{
    d_mode_lock = lock;
}


bool danzeffIsDirty()
{
    return d_dirty;
}

// EOF

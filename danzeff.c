// Danzeff-G by Geecko - A complete rewrite of the Danzeff OSK library.
//
// This work is licensed under the Creative Commons BY-SA 3.0 Unported License.
// See LICENSE for more details.
//
// Please report bugs at : geecko.dev@free.fr

#include "danzeff.h"

#include <glib2d.h>

#include "danzeff.h"
#include "pspkernel.h"

#define TEX_N 8
#define BASE_DIR "res/"

static bool holding = false;
static bool dirty = true;
static bool shifted = false;
static bool dmode = true;
static bool init = false;
//static bool rotated = false;
static int selected_x = 1;
static int selected_y = 1;

g2dImage *tex[TEX_N] = {0};

static char path[TEX_N][32] =
{
    BASE_DIR "keys.png",   BASE_DIR "keys_s.png",
    BASE_DIR "keys_c.png", BASE_DIR "keys_s_c.png",
    BASE_DIR "nums.png",   BASE_DIR "nums_s.png",
    BASE_DIR "nums_c.png", BASE_DIR "nums_s_c.png"
};

static char layout[4][3][3][5] =
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
    if (init) return;

    int i;

    g2dInit();

    for (i=0; i<TEX_N; i++)
    {
        tex[i] = g2dTexLoad(path[i], G2D_SWIZZLE);
    }
    
    init = true;
}


void danzeffTerm()
{
    if (!init) return;
    
    int i;

    for (i=0; i<TEX_N; i++)
    {
        g2dTexFree(&tex[i]);
    }
    
    init = false;
}


DanzeffChar danzeffRead(SceCtrlData pad)
{
    int x, y, choice;
    DanzeffChar character;

    x = 1 - (pad.Lx < 85) + (pad.Lx > 170);
    y = 1 - (pad.Ly < 85) + (pad.Ly > 170);

    if (selected_x != x || selected_y != y)
    {
        dirty = true;
        selected_x = x;
        selected_y = y;
    }
    if ((!shifted &&  (pad.Buttons & PSP_CTRL_RTRIGGER)) ||
        ( shifted && !(pad.Buttons & PSP_CTRL_RTRIGGER)))
    {
        dirty = true;
        shifted = !shifted;
    }

    character = 0;

    if (!holding)
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

            character = layout[2*dmode + shifted][y][x][choice];
        }
        else if (pad.Buttons & PSP_CTRL_LEFT)
        {
            character = DANZEFF_LEFT;
        }
        else if (pad.Buttons & PSP_CTRL_RIGHT)
        {
            character = DANZEFF_RIGHT;
        }
        else if (pad.Buttons & PSP_CTRL_SELECT)
        {
            character = DANZEFF_SELECT;
        }
        else if (pad.Buttons& PSP_CTRL_START)
        {
            character = DANZEFF_START;
        }
    }

    holding = pad.Buttons & ~PSP_CTRL_RTRIGGER;

    return character;
}


void danzeffRender(int x, int y)
{
    g2dColor color;

    dirty = false;
    color = WHITE;
    
    if (selected_x != 1 || selected_y != 1)
    {
        color = G2D_MODULATE(color, 255, 128);
    }

    g2dBeginRects(tex[4*dmode + 2*shifted]);
    {
        g2dSetColor(color);
        g2dSetCoordXY(x, y);
        g2dAdd();
    }
    g2dEnd();
    
    g2dBeginRects(tex[4*dmode + 2*shifted + 1]);
    {
        g2dSetCoordXY(x + selected_x*43, y + selected_y*43);
        g2dSetCropXY(selected_x*64, selected_y*64);
        g2dSetCropWH(64, 64);
        g2dSetScaleWH(64, 64);
        g2dAdd();
    }
    g2dEnd();
}


//void danzeffSetRotated(bool rotated);


void danzeffSetMode(bool mode)
{
    dmode = mode;
}


bool danzeffIsDirty()
{
    return dirty;
}

// EOF

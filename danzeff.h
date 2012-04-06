/**
 * \file danzeff.h
 * \brief Danzeff-G header
 */

#ifndef DANZEFF_HEADER
#define DANZEFF_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <pspctrl.h>

#define DANZEFF_W 150
#define DANZEFF_H 150

typedef enum
{
    DANZEFF_LETTERS = 0,
    DANZEFF_NUMBERS = 1
} DanzeffMode;

typedef enum
{
    DANZEFF_LEFT = 1,
    DANZEFF_RIGHT = 2,
    DANZEFF_SELECT = 3,
    DANZEFF_START = 4
} DanzeffKey;

typedef unsigned int DanzeffChar;

void danzeffInit();
void danzeffTerm();
DanzeffChar danzeffRead(SceCtrlData pad);
void danzeffRender(int x, int y);
//void danzeffSetRotated(bool rotated);
void danzeffSetMode(unsigned int mode);
void danzeffSetModeLock(bool lock);
bool danzeffIsDirty();

#ifdef __cplusplus
}
#endif

#endif // DANZEFF_HEADER

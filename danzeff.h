/**
 * \file danzeff.h
 * \brief Danzeff-G header
 */

#ifndef DANZEFF_HEAD
#define DANZEFF_HEAD

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <pspctrl.h>

#define DANZEFF_W 150
#define DANZEFF_H 150

enum DanzeffKeys
{
    DANZEFF_LEFT = 1,
    DANZEFF_RIGHT = 2,
    DANZEFF_SELECT = 3,
    DANZEFF_START = 4
};

typedef unsigned int DanzeffChar;

void danzeffInit();
void danzeffTerm();
DanzeffChar danzeffRead(SceCtrlData pad);
void danzeffRender(int x, int y);
//void danzeffSetRotated(bool rotated);
void danzeffSetMode(bool mode);
bool danzeffIsDirty();

#ifdef __cplusplus
}
#endif

#endif // DANZEFF_HEAD

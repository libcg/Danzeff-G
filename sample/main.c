#include <danzeff.h>
#include <glib2d.h>
#include <intraFont.h>
#include <pspctrl.h>
#include <pspkernel.h>

PSP_MODULE_INFO("Danzeff", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

/* Callbacks */
int exit_callback(int arg1, int arg2, void *common) {
  sceKernelExitGame();
  return 0; }
int CallbackThread(SceSize args, void *argp) {
  int cbid;
  cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
  sceKernelRegisterExitCallback(cbid);
  sceKernelSleepThreadCB();
  return 0; }
int SetupCallbacks() {
  int thid = 0;
  thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
  if(thid >= 0) sceKernelStartThread(thid, 0, 0);
  return thid; }

int main()
{
    intraFont* font;
    SceCtrlData pad;
    DanzeffChar dchar;
    char str[1024] = "Edit me!";
    int str_size = 8;

    SetupCallbacks();
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
    g2dInit();
    intraFontInit();
    font = intraFontLoad("flash0:/font/ltn8.pgf", INTRAFONT_CACHE_MED);
    
    danzeffInit();
    danzeffSetMode(DANZEFF_LETTERS);
    danzeffSetModeLock(false);
    
    while (1)
    {
        sceCtrlPeekBufferPositive(&pad, 1);
        dchar = danzeffRead(pad);

        switch (dchar)
        {
            case '\010': // Del
                if (str_size == 0) break;
                str[--str_size] = '\0';
                break;
                
            case DANZEFF_LEFT:
            case DANZEFF_RIGHT:
            case DANZEFF_SELECT:
            case DANZEFF_START:
            case '\0':
                break; // Do nothing
                
            default: // ASCII char
                if (str_size+2 >= 1024) break;
                str[str_size++] = (char)dchar;
                break;                
        }

        g2dClear(AZURE);

        danzeffRender(G2D_SCR_W/2 - DANZEFF_W/2, 0);
        
        intraFontSetStyle(font, 1.f, WHITE, 0, 0.f, INTRAFONT_ALIGN_CENTER);
        intraFontPrint(font, G2D_SCR_W/2, 3*G2D_SCR_H/4, str);

        g2dFlip(G2D_VSYNC);
    }

    danzeffTerm();
    intraFontUnload(font);
    intraFontShutdown();
    g2dTerm();

    sceKernelExitGame();
    return 0;
}

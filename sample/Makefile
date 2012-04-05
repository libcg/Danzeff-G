MEDIA = /media/PANDORA/psp/game/CAT_Homebrews\ perso/
TARGET = danzeff_sample

OBJS = main.o
LIBS = -ldanzeff -lintrafont -lglib2d -ljpeg -lpng -lz -lpspgum -lpspgu \
       -lpsprtc -lm -lpspvram

CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Danzeff-G sample

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak 
install:
	cp ./EBOOT.PBP $(MEDIA)$(TARGET)
dir:
	mkdir $(MEDIA)$(TARGET)
lclean:
	rm *.o

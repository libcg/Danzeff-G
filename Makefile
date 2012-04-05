PSPSDK = $(shell psp-config --pspsdk-path)
PSPDIR = $(shell psp-config --psp-prefix)

CFLAGS = -O2 -G0 -g

OBJS = danzeff.o
TARGET_LIB = libdanzeff.a

include $(PSPSDK)/lib/build.mak

install: all
	mkdir -p $(PSPDIR)/include $(PSPDIR)/lib
	cp *.h $(PSPDIR)/include
	cp *.a $(PSPDIR)/lib

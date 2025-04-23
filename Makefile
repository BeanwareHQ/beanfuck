CC = cc
INCLUDE = -I./3rdparty/include
LIBS =  
OBJ = beanfuck.o 3rdparty/asv/asv.o

RELEASE_CFLAGS = -O2 -Wall -Wextra -pedantic -march=native -flto=auto $(INCLUDE) $(LIBS)
DEBUG_CFLAGS = -O0 -g -Wall -Wextra -pedantic -fsanitize=address $(INCLUDE) $(LIBS)
TARBALLFILES = Makefile LICENSE.md README.md beanfuck.c 3rdparty 

TARGET = debug

ifeq ($(TARGET),debug)
	CFLAGS=$(DEBUG_CFLAGS)
else
	CFLAGS=$(RELEASE_CFLAGS)
endif

beanfuck: setup $(OBJ)
	$(CC) $(CFLAGS) -o beanfuck $(OBJ)

setup: deps settings

settings:

beanfuck.o:

deps: 
	mkdir -p 3rdparty/include
	test -d 3rdparty/asv || git clone "https://github.com/ezntek/asv" 3rdparty/asv
	test -f 3rdparty/asv/asv.o || make -C 3rdparty/asv
	cp 3rdparty/asv/*.h 3rdparty/include/
	
updatedeps:
	rm -rf 3rdparty/*
	make deps

tarball: deps
	mkdir -p beanfuck
	cp -rv $(TARBALLFILES) beanfuck/
	tar czvf beanfuck.tar.gz beanfuck
	rm -rf beanfuck

defaults:

clean:
	rm -rf beanfuck beanfuck.tar.gz beanfuck $(OBJ)
	rm -rf 3rdparty/include/*

distclean: clean defaults
	rm -rf 3rdparty/*

.PHONY: clean cleanall

## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(MAIN_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = main

main_SOURCES = \
	main.cpp \
	shader.h \
	camera.h \
	mesh.h \
	model.h

main_LDFLAGS = 

main_LDADD = $(MAIN_LIBS)

if NATIVE_WIN32
main_LDFLAGS += -mwindows
endif

res_DATA = E.obj \
	lamp.fs \
	lamp.vs \
	lighting.fs \
	lighting.vs \
	E.mtl \
	P.mtl \
	P.obj
resdir = $(pkgdatadir)/shaders

EXTRA_DIST = $(res_DATA)


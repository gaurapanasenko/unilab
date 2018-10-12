## Process this file with automake to produce Makefile.in

## Created by Anjuta


uidir = $(pkgdatadir)/ui
ui_DATA = main.ui


AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(MAIN_CFLAGS) \
	-I$(srcdir)/miniparser/.

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = main

main_SOURCES = \
	main.cc \
	derivedwindow.cc \
	derivedwindow.h \
	model.cc \
	model.h \
	MiniParser.cpp \
	MiniParser.hpp

main_LDFLAGS = 

main_LDADD = $(MAIN_LIBS)


if NATIVE_WIN32
main_LDFLAGS += -mwindows
endif



EXTRA_DIST = $(ui_DATA)

# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(uidir)
	-rm -r $(pkgdatadir)


AM_CXXFLAGS = -Wall \
	-g 

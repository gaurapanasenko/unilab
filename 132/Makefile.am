## Process this file with automake to produce Makefile.in
## Created by Anjuta

dist_doc_DATA = \
	COPYING \
	AUTHORS \
	ChangeLog \
	INSTALL \
	NEWS \
	Readme.md \
	README



# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(docdir)

AM_CFLAGS = -Wall \
	-g 

AM_CPPFLAGS = -DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" 

lib_LTLIBRARIES =  \
	libsuperint.la

libsuperint_la_SOURCES =  \
	superint.cpp \
	superint.h

libsuperint_la_LDFLAGS = -module \
	-avoid-version \
	-shared 

bin_PROGRAMS =  \
	main

main_SOURCES =  \
	main.cpp \
	tests.cpp \
	tests.h

main_LDADD = libsuperint.la 

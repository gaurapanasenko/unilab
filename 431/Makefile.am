## Process this file with automake to produce Makefile.in
## Created by Anjuta

dist_doc_DATA = \
	README \
	COPYING \
	AUTHORS \
	ChangeLog \
	INSTALL \
	NEWS \
	Readme.md



# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(docdir)


bin_PROGRAMS =  \
	main

main_SOURCES =  \
	classes.cpp \
	classes.h \
	main.cpp

main_LDADD = $(GL_LIBS) \
	-lglut 

AM_CPPFLAGS = -DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir). \
	$(GL_CFLAGS). \
	$(FREEGLUT_CFLAGS). \
	$(GL_CFLAGS). \
	$(GL_CFLAGS). \
	$(GL_CFLAGS)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" 

AM_CFLAGS = -Wall \
	-g 

AM_CXXFLAGS = -Wall \
	-g 

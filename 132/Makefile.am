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

SUBDIRS = \
	libsuperint \
	src

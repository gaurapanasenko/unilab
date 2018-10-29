## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)."\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	-I$(top_srcdir)/libsuperint

AM_CFLAGS =\
	 -Wall\
	 -g

TESTS = tests

bin_PROGRAMS = main

noinst_PROGRAMS = tests

main_SOURCES = \
	main.cpp \
	tests.cpp \
	tests.h

tests_SOURCES = \
	run_tests.cpp \
	tests.cpp \
	tests.h

main_LDADD = ../libsuperint/libsuperint.la

tests_LDADD = ../libsuperint/libsuperint.la

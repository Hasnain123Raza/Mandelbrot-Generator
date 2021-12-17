bin_PROGRAMS = mandelbrot
mandelbrot_SOURCES = $(top_srcdir)/src/main.c \
    $(top_srcdir)/src/stb_image_write.c \
    $(top_srcdir)/src/generator.c \
    $(top_srcdir)/src/convertor.c \
    $(top_srcdir)/src/calculator.c \
    $(top_srcdir)/src/util.c
EXTRA_DIST = $(top_srcdir)/include/stb_image_write.h \
    $(top_srcdir)/include/generator.h \
    $(top_srcdir)/include/convertor.h \
    $(top_srcdir)/include/calculator.h \
    $(top_srcdir)/include/common.h \
    $(top_srcdir)/include/util.h
AM_CPPFLAGS = -I$(top_srcdir)/include
LDADD = -lm
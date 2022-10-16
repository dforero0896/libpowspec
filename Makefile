CC = gcc
CFLAGS = -std=c99 -O3 -Wall
LIBS = -lm -lfftw3
INCL = -Isrc -Iio -Ilib -Imath


LIBS = -DOMP -fopenmp #-lfftw3_omp
# Settings for FFTW
#FFTW_DIR = /opt/cray/pe/fftw/3.3.8.4/$(CRAY_CPU_TARGET)/lib
LIBS += -L$(FFTW_DIR)
INCL += -I$(FFTW_DIR)/../include

# Setting for single precision density fields and FFT
#LIBS += -DSINGLE_PREC

# Settings for OpenMP (comment the following line to disable OpenMP)

# Settings for CFITSIO (not implemented yet)

SRCS = $(wildcard src/*.c lib/*.c io/*.c math/*.c)

libpowspec_f:
	$(CC) $(CFLAGS) -pthread -fPIC -shared -o libpowspec_f.so $(SRCS) -lm -lfftw3f $(LIBS) -DSINGLE_PREC $(INCL) 
libpowspec:
	$(CC) $(CFLAGS) -pthread -fPIC -shared -o libpowspec.so $(SRCS) -lm -lfftw3 $(LIBS) $(INCL)

clean:
	rm libpowspec.so libpowspec_f.so


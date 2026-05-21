CC      = gcc
CFLAGS  = -g
RM      = rm -f


default: all

all: fpga

fpga: fpga_if.c
	$(CC) $(CFLAGS) -o fpga fpga_if.c

clean veryclean:
	$(RM) fpga


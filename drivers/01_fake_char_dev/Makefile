obj-m = fake_char_dev.o
KVERSION = $(shell uname -r)
CC := gcc-12
all:
		make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules
clean:
		make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean
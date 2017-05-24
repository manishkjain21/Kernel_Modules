obj-m+=timer3.o

all:
	make -C ${KERNELDIR} M=$(PWD) modules
clean:
	make -C ${KERNELDIR} M=$(PWD) clean

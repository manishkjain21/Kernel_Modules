obj-m+=kernel_panda.o

all:
	make -C ${KERNELDIR} M=$(PWD) modules
clean:
	make -C ${KERNELDIR} M=$(PWD) clean

obj-m+=kernel_thread.o

all:
	make -C ${KERNELDIR} M=$(PWD) modules
clean:
	make -C ${KERNELDIR} M=$(PWD) clean

CROSS_COMPILE := arm-linux-gnueabihf-

all: lxaccelldriver

lxaccelldriver:
	@echo "Building application.... lxapp"
	$(CROSS_COMPILE)gcc lxapp.c -o lxapp

clean:
	@echo "Cleaning application.... lxapp"
	-rm -rf lxapp

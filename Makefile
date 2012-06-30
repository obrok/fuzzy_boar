all: build

build:
	ino build

upload: build
	ino upload

setup:
	cd lib/BinaryMuse-ino-f2cd9e0 && python setup.py install

clean:
	ino clean

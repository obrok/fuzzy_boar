all: build

build:
	ino build

upload: build
	ino upload

clean:
	ino clean

all: build

build:
	ino build

upload: build
	ino upload

setup:
	cd lib/BinaryMuse-ino-f2cd9e0 && python setup.py install

test:
	all

clean:
	ino clean

swing:
	gcc -Wall --pedantic -shared -fpic src/calculate_engines.c -o ruby/libquadcopter.so
	LD_LIBRARY_PATH=/home/vagrant/arduino/fuzzy_boar/ruby/ ruby ruby/swing.rb

##
## author: Brando
## date: 3/7/23
##

.PHONY: bflibc bflibcpp bflibrust bfnet bftest

all: setup 
	cd bflibc && make clean && make build CONFIG=debug && make build CONFIG=release
	cd bflibcpp && make clean && make build CONFIG=debug && make build CONFIG=release
	cd bfnet && make clean && make build CONFIG=debug && make build CONFIG=release
	cd bftest && make clean && make build CONFIG=debug && make build CONFIG=release
	cd bflibrust && make debug release
	cp -afv bflibc/bin/* bin/
	cp -afv bflibcpp/bin/* bin/
	cp -afv bfnet/bin/* bin/
	cp -afv bftest/bin/* bin/
	cp -afv bflibrust/bin/* bin/

setup: 
	mkdir -p bin

clean:
	cd bflibc && make clean
	cd bflibcpp && make clean
	cd bfnet && make clean
	cd bftest && make clean
	cd bflibrust && make clean
	rm -rfv bin


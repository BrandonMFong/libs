##
## author: Brando
## date: 3/7/23
##

.PHONY: bflibc bflibcpp bflibrust

all: setup 
	cd bflibc && make debug release
	cd bflibcpp && make debug release
	cd bfnet && make debug release
	cd bflibrust && make debug release
	cp -afv bflibc/bin/* bin/
	cp -afv bflibcpp/bin/* bin/
	cp -afv bfnet/bin/* bin/
	cp -afv bflibrust/bin/* bin/

test:
	./scripts/runtests

setup: 
	mkdir -p bin

clean:
	cd bflibc && make clean
	cd bflibcpp && make clean
	cd bfnet && make clean
	cd bflibrust && make clean
	rm -rfv bin


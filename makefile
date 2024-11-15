##
## author: Brando
## date: 3/7/23
##

CLIBS = bflibc bflibcpp bflibrust bfnet bftest
RUSTLIBS = bflibrust

.PHONY: $(CLIBS) $(RUSTLIBS)

build: setup $(CLIBS) $(RUSTLIBS)

$(CLIBS): setup
	cd $@ && make clean && make build CONFIG=debug && make build CONFIG=release
	cp -afv $@/bin/* bin/

$(RUSTLIBS): setup
	cd $@ && make debug release
	cp -afv $@/bin/* bin/

setup: bin
	mkdir -p $^

clean: bin
	rm -rfv $^


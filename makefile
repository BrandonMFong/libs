##
## author: Brando
## date: 3/7/23
##

.PHONY: bflibc bflibcpp bflibrust

PROJECTS := bflibc bflibcpp bflibrust

all: setup $(PROJECTS)

$(PROJECTS): setup
	cd $@ && make clean
	cd $@ && make debug release
	cp -afv $@/bin/* bin/

test:
	./scripts/runtests

setup: 
	mkdir -p bin

clean:
	rm -rfv bin


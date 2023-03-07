##
## author: Brando
## date: 3/7/23
##

.PHONY: bflibc bflibcpp bflibrust

PROJECTS := bflibc bflibcpp bflibrust

all: setup $(PROJECTS)

$(PROJECTS):
	cd $@ && make clean
	cd $@ && make release
	cd $@ && make debug
	cp -afv $@/bin/* bin/

setup: 
	mkdir -p bin

clean:
	rm -rfv bin

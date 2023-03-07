##
## author: Brando
## date: 3/7/23
##

R_BIN_PATH = bin/release/rust
D_BIN_PATH = bin/debug/rust

all: release debug

release: setup
	cargo build --release --target-dir $(R_BIN_PATH)

debug: setup
	cargo build --target-dir $(D_BIN_PATH)

test:
	cargo test

setup:
	mkdir -p $(D_BIN_PATH)
	mkdir -p $(R_BIN_PATH)

clean:
	rm -rfv bin


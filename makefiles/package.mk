# author: Brando
# date: 12/9/24
#

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
PACKAGE_MODE = package-linux
endif
ifeq ($(UNAME_S),Darwin)
PACKAGE_MODE = package-macos
endif

package: $(PACKAGE_MODE)

package-linux: $(PACKAGE_NAME) $(PACKAGE_NAME)/$(BIN_NAME)
	zip -r $(BIN_PATH)/$(PACKAGE_NAME).zip $(PACKAGE_NAME)
	tar vczf $(BIN_PATH)/$(PACKAGE_NAME).tar.gz $(PACKAGE_NAME)

package-macos: $(PACKAGE_NAME) $(PACKAGE_NAME)/$(BIN_NAME)
	hdiutil create -fs HFS+ -volname $(PACKAGE_NAME) -srcfolder $(PACKAGE_NAME) $(BIN_PATH)/$(PACKAGE_NAME).dmg

$(PACKAGE_NAME):
	mkdir -p $@

$(PACKAGE_NAME)/$(BIN_NAME): $(BIN_PATH)/$(BIN_NAME)
	@cp -afv $< $(PACKAGE_NAME)


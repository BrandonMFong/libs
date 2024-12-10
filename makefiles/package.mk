# author: Brando
# date: 12/9/24
#

ifeq ($(PACKAGE_BIN_TARGET),)
$(error ERROR: "please define `PACKAGE_BIN_TARGET` in your makefile. These are files that are going to be packaged")
endif

ifeq ($(PACKAGE_NAME),)
$(error ERROR: "please define `PACKAGE_NAME` in your makefile. This will define the name of the compressed package")
endif

ifeq ($(PACKAGE_BIN_PATH),)
$(error ERROR: "please define `PACKAGE_BIN_PATH` in your makefile. This is where the package files will be outputed")
endif

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
PACKAGE_MODE = package-linux
endif
ifeq ($(UNAME_S),Darwin)
PACKAGE_MODE = package-macos
endif

package: $(PACKAGE_MODE)

package-linux: $(PACKAGE_NAME) $(PACKAGE_NAME)/$(BIN_NAME)
	zip -r $(PACKAGE_BIN_PATH)/$(PACKAGE_NAME).zip $(PACKAGE_NAME)
	tar vczf $(PACKAGE_BIN_PATH)/$(PACKAGE_NAME).tar.gz $(PACKAGE_NAME)

package-macos: $(PACKAGE_NAME) $(PACKAGE_NAME)/$(BIN_NAME)
	hdiutil create -fs HFS+ -volname $(PACKAGE_NAME) -srcfolder $(PACKAGE_NAME) $(PACKAGE_BIN_PATH)/$(PACKAGE_NAME).dmg

$(PACKAGE_NAME):
	mkdir -p $@

$(PACKAGE_NAME)/$(BIN_NAME): $(PACKAGE_BIN_PATH)/$(BIN_NAME)
	@cp -afv $< $(PACKAGE_NAME)

### codesigning

codesign:
	codesign -s "$(IDENTITY)" --options=runtime --timestamp $(PACKAGE_BIN_PATH)/$(BIN_NAME)

### notarize

notarize:
	xcrun notarytool \
	submit \
	--apple-id "$(EMAIL)" \
	--password "$(PW)" \
	--team-id "$(TEAMID)" \
	--wait \
	$(PACKAGE_BIN_PATH)/$(PACKAGE_NAME).dmg

staple:
	xcrun stapler staple $(PACKAGE_BIN_PATH)/$(PACKAGE_NAME).dmg


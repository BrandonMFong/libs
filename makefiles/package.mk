# author: Brando
# date: 12/9/24
#

package: $(PACKAGE_MODE)

package-linux: $(PACKAGE_NAME) $(PACKAGE_NAME)/$(BIN_NAME)
	zip -r $(BIN_PATH)/$(PACKAGE_NAME)-$(PLATFORM).zip $(PACKAGE_NAME)
	tar vczf $(BIN_PATH)/$(PACKAGE_NAME)-$(PLATFORM).tar.gz $(PACKAGE_NAME)

package-macos: $(PACKAGE_NAME) $(PACKAGE_NAME)/$(BIN_NAME)
	hdiutil create -fs HFS+ -volname Chat -srcfolder $(PACKAGE_NAME) $(BIN_PATH)/$(PACKAGE_NAME)-$(PLATFORM).dmg

$(PACKAGE_NAME):
	mkdir -p $@

$(PACKAGE_NAME)/$(BIN_NAME): $(BIN_PATH)/$(BIN_NAME)
	@cp -afv $< $(PACKAGE_NAME)


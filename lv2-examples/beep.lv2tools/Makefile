BUNDLE = lv2pftci-beep.lv2
INSTALL_DIR = /home/pi/zynthian/zynthian-plugins/mod-lv2

$(BUNDLE): manifest.ttl beep.ttl beep.so
	rm -rf $(BUNDLE)
	mkdir $(BUNDLE)
	cp $^ $(BUNDLE)

beep.so: beep.cpp beep.peg
	g++ -shared -fPIC -DPIC beep.cpp `pkg-config --cflags --libs lv2-plugin` -o beep.so

beep.peg: beep.ttl
	lv2peg beep.ttl beep.peg

install: $(BUNDLE)
	mkdir -p $(INSTALL_DIR)
	rm -rf $(INSTALL_DIR)/$(BUNDLE)
	cp -R $(BUNDLE) $(INSTALL_DIR)

clean:
	rm -rf $(BUNDLE) beep.so beep.peg


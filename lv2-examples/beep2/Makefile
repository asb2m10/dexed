BUNDLE = lv2pftci-beep2.lv2
INSTALL_DIR = /home/pi/zynthian/zynthian-plugins/mod-lv2


$(BUNDLE): manifest.ttl beep2.ttl beep2.so
	rm -rf $(BUNDLE)
	mkdir $(BUNDLE)
	cp $^ $(BUNDLE)

beep2.so: beep.cpp beep.peg
	g++ -shared -fPIC -DPIC beep.cpp -I. -I/usr/local/include/lvtk-2 -L/usr/local/lib -llvtk_plugin2 -o beep2.so

beep.peg: beep2.ttl
	lv2peg beep2.ttl beep.peg

install: $(BUNDLE)
	mkdir -p $(INSTALL_DIR)
	rm -rf $(INSTALL_DIR)/$(BUNDLE)
	cp -R $(BUNDLE) $(INSTALL_DIR)

clean:
	rm -rf $(BUNDLE) beep2.so beep.peg


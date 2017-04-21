COMMAND1 = (CC=$(DEVKITPPC)/bin/powerpc-eabi-gcc CXX=$(DEVKITPPC)/bin/powerpc-eabi-g++ ./configure --host=powerpc-eabi --build=powerpc-eabi-gnu --prefix="${DEVKITPRO}/portlibs/ppc" --libdir="${DEVKITPRO}/portlibs/ppc/lib" --disable-shared && \
      make && sudo -E make install)

all: dir1 dir2 dir3

dir1:
	cd fribidi && $(COMMAND1)
dir2:
	cd libexif && $(COMMAND1)
dir3:
	cd libiconv && $(COMMAND1)

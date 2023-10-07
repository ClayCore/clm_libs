.PHONY: prepare build clean

BUILDDIR := build

prepare: | $(BUILDDIR)
	cmake -S . -B $(BUILDDIR) -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DCLM_WARNINGS_AS_ERRORS=OFF

build: | $(BUILDDIR)
	cmake --build $(BUILDDIR) --config Release

clean: | $(BUILDDIR)
	rm -rf $(BUILDDIR)

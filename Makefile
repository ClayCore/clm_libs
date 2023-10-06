.PHONY: prepare build clean

BUILDDIR := build

prepare: | $(BUILDDIR)
	cmake -S . -B $(BUILDDIR) -G "Ninja" -DCMAKE_BUILD_TYPE=Release

build: | $(BUILDDIR)
	cmake --build $(BUILDDIR) --config Release

clean: | $(BUILDDIR)
	rm -rf $(BUILDDIR)

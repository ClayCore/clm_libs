.PHONY: prepare build clean

BUILDDIR := target

prepare: | $(BUILDDIR)
	cmake -S . -B $(BUILDDIR)

build: | $(BUILDDIR)
	cmake --build $(BUILDDIR)

clean: | $(BUILDDIR)
	rm -rf $(BUILDDIR)

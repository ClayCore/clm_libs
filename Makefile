.PHONY: build_minsize build_opt clean

BUILDDIR := target

$(BUILDDIR):
	mkdir -p $@

build_minsize: | $(BUILDDIR)
	meson setup $(BUILDDIR) -Dbuildtype=minsize
	meson compile -C $(BUILDDIR)

build_opt: | $(BUILDDIR)
	meson setup $(BUILDDIR) -Dbuildtype=release

clean: | $(BUILDDIR)
	rm -rf $(BUILDDIR)

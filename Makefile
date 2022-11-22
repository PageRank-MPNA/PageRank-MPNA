.PHONY: clean all parallel sequential

parallel:
	cd $@ && $(MAKE)

sequential:
	cd $@ && $(MAKE)
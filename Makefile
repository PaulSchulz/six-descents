SUBDIRS := src

.phony: all $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	make -C $@

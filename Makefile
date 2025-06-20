TARGETS = anneal binary binary2

.PHONY: default all clean

default: all
all:
	@for dir in $(TARGETS); do \
		$(MAKE) -C $$dir ; \
	done
	
clean:
	@for dir in $(TARGETS); do \
		$(MAKE) -C $$dir clean ; \
	done


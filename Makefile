TARGETS = anneal binary

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


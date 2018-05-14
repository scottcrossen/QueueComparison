# Makefile by Scott Leland Crossen
# @Copyright 2017 Scott Leland Crossen

# ==== Definitions ====

BYUNETID = slxn42
VERSION = 1
TMPDIR = tmp
SOURCEDIR = source
RELDIR = rel
COMP = gcc
BINOBJS = ${TMPDIR}/main.o ${TMPDIR}/linkedqueue.o ${TMPDIR}/arrayqueue.o ${TMPDIR}/mmqueue.o
MAIN = ${RELDIR}/main
COMPFLAGS = -Wall -O2
LINKFLAGS = -lrt

# ==== Recipes ====

.PHONY: default
.DEFAULT: default
default: all

.PHONY: all
all: $(MAIN)

.PHONY: clean
clean:
	rm -rf $(TMPDIR) $(RELDIR)

.PHONY: test
test:
	./${MAIN}

# ==== Targets ====

.PRECIOUS: ${TMPDIR}/%.o
${TMPDIR}/%.o: ${SOURCEDIR}/%.c ${SOURCEDIR}/%.h
	mkdir -p $(@D)
	$(COMP) -c $< -o $@ $(COMPFLAGS)

.PRECIOUS: ${RELDIR}/%
${RELDIR}/%: ${BINOBJS}
	mkdir -p $(@D)
	$(COMP) $^ -o $@ $(LINKFLAGS)

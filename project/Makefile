# Makefile by Scott Leland Crossen
# @Copyright 2017 Scott Leland Crossen

# ==== Definitions ====

BYUNETID = slxn42
VERSION = 1
BINDIR = bin
SOURCEDIR = source
RELDIR = rel
COMP = gcc
BINOBJS = ${BINDIR}/main.o ${BINDIR}/linkedqueue.o ${BINDIR}/arrayqueue.o ${BINDIR}/memorymappedqueue.o
MAIN = ${RELDIR}/main
COMPFLAGS = -Wall -O2
LINKFLAGS =

# ==== Recipes ====

.PHONY: default
.DEFAULT: default
default: all

.PHONY: all
all: $(MAIN)

.PHONY: clean
clean:
	rm -rf $(BINDIR) $(RELDIR)

.PHONY: test
test:
	./${MAIN}

# ==== Targets ====

.PRECIOUS: ${BINDIR}/%.o
${BINDIR}/%.o: ${SOURCEDIR}/%.c ${SOURCEDIR}/%.h
	mkdir -p $(@D)
	$(COMP) -c $< -o $@ $(COMPFLAGS)

.PRECIOUS: ${RELDIR}/%
${RELDIR}/%: ${BINOBJS}
	mkdir -p $(@D)
	$(COMP) $^ -o $@ $(LINKFLAGS)

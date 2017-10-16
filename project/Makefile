# Makefile by Scott Leland Crossen
# @Copyright 2017 Scott Leland Crossen

# ==== Definitions ====

BYUNETID = slxn42
VERSION = 1
BINDIR = bin
SOURCEDIR = source
RELDIR = rel
COMP = gcc
OBJS = $(TSH) ${RELDIR}/arrayqueue
COMPFLAGS = -Wall -O2
LINKFLAGS =

# ==== Recipes ====

.PHONY: default
.DEFAULT: default
default: all

.PHONY: all
all: $(OBJS)

.PHONY: clean
clean:
	rm -rf $(BINDIR) $(RELDIR)

# ==== Targets ====

.PRECIOUS: ${BINDIR}/%.o
${BINDIR}/%.o: ${SOURCEDIR}/%.c
	mkdir -p $(@D)
	$(COMP) -c $< -o $@ $(COMPFLAGS)

.PRECIOUS: ${RELDIR}/%
${RELDIR}/%: ${BINDIR}/%.o
	mkdir -p $(@D)
	$(COMP) $^ -o $@ $(LINKFLAGS)

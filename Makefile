# created by Benson
# used for CS 246 final project by 
# group of Alick, Benson, Leo

MAKEFILEDIR := $(shell cd $(dir $(abspath $(lastword $(MAKEFILE_LIST)))) && pwd)

CXX = g++-14
CXXFLAGS = -std=c++20 -MMD -Wall -g -I$(MAKEFILEDIR)/include

SRCDIR = src
BUILDDIR = build
TARGET = Chess

SRCS = $(shell find $(SRCDIR) -name '*.cc')
OBJS = $(patsubst $(SRCDIR)/%.cc,$(BUILDDIR)/%.o,$(SRCS))
DEPS = ${OBJS:.o=.d}

${TARGET}: ${OBJS}
	${CXX} ${CXXFLAGS} ${OBJS} -o ${TARGET} -lX11

$(BUILDDIR)/%.o: $(SRCDIR)/%.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

.PHONY: clean

clean:
	rm ${OBJS} ${TARGET} ${DEPS}

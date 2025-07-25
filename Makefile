# created by Benson
# used for CS 246 final project by 
# group of Alick, Benson, Leo

CXX = g++-14
CXXFLAGS = -std=c++20 -MMD -Wall -g -Iinclude

SRCDIR = src
BUILDDIR = build/main
TARGET = Chess.out

noIcon ?= 0
ifeq ($(noIcon),1)
	CXXFLAGS += -DNOICON
	TARGET = Chess-noIcon.out
	BUILDDIR = build/noIcon
endif

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

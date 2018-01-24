ARCH = $(shell uname -m)

PREFIX = /usr/local

BINFILES = fahrraddriver
OBJECTS = 

INSTBIN = $(patsubst %,$(PREFIX)/bin/%,$(BINFILES))

ifeq "$(ARCH)" "x86_64"
CXXFLAGS += -msse -msse2 -mfpmath=sse -ffast-math -fomit-frame-pointer -fno-finite-math-only
endif

CXXFLAGS += -Wall -O3 -L./

EXTERNALS = 

fahrraddriver: EXTERNALS += libserial

LDLIBS += -ltascar -ldl
LDFLAGS += -L../tascar/build
CXXFLAGS += -I../tascar/src
CPPFLAGS += -I../tascar/src

LDLIBS += `pkg-config --libs $(EXTERNALS)`
CXXFLAGS += `pkg-config --cflags $(EXTERNALS)`

all: libtascar
	mkdir -p build
	$(MAKE) -C build -f ../Makefile $(BINFILES)

libtascar:
	$(MAKE) -C tascar lib

install:
	$(MAKE) -C build -f ../Makefile $(INSTBIN)

uninstall:
	rm -f $(INSTBIN)

clean:
	rm -Rf *~ src/*~ build doc/html

VPATH = ../src

.PHONY : doc

doc:
	cd doc && sed -e 's/PROJECT.NUMBER.*=.*/&'`cat ../version`'/1' doxygen.cfg > .temp.cfg && doxygen .temp.cfg
	rm -Rf doc/.temp.cfg

include $(wildcard *.mk)

fahrraddriver: EXTERNALS += liblo libserial

$(PREFIX)/bin/%: %
	cp $< $@

$(BINFILES): $(OBJECTS)

%: %.o
	$(CXX) $(CXXFLAGS) $(LDLIBS) $^ -o $@

%.o: %.cc
	$(CPP) $(CPPFLAGS) -MM -MF $(@:.o=.mk) $<
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Local Variables:
# compile-command: "make"
# End:

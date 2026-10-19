CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
CPPFLAGS = -Iinclude

ifeq ($(OS),Windows_NT)
    EXEEXT = .exe
    RUN = .\$(TARGET)$(EXEEXT)
    RM = del /Q
else
    EXEEXT =
    RUN = ./$(TARGET)$(EXEEXT)
    RM = rm -f
endif

TARGET = inventaris
OBJS = src/main.o src/data.o src/berkas.o src/tampilan.o
PROGRAM = $(TARGET)$(EXEEXT)

.PHONY: all run clean FORCE

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

src/main.o: src/main.cpp include/berkas.h include/tampilan.h include/data.h FORCE
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

src/data.o: src/data.cpp include/data.h FORCE
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

src/berkas.o: src/berkas.cpp include/berkas.h include/data.h FORCE
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

src/tampilan.o: src/tampilan.cpp include/tampilan.h include/data.h FORCE
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

run: $(PROGRAM)
	$(RUN)

clean:
	-$(RM) $(PROGRAM) $(OBJS)

FORCE:

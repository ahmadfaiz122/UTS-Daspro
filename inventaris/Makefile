CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

TARGET = inventaris
OBJS = main.o data.o berkas.o tampilan.o

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cpp berkas.h tampilan.h data.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

data.o: data.cpp data.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

berkas.o: berkas.cpp berkas.h data.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

tampilan.o: tampilan.cpp tampilan.h data.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)

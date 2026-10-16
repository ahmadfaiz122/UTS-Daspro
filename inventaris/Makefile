CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
CPPFLAGS = -Iinclude

TARGET = inventaris
OBJS = src/main.o src/data.o src/berkas.o src/tampilan.o

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

src/main.o: src/main.cpp include/berkas.h include/tampilan.h include/data.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

src/data.o: src/data.cpp include/data.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

src/berkas.o: src/berkas.cpp include/berkas.h include/data.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

src/tampilan.o: src/tampilan.cpp include/tampilan.h include/data.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)

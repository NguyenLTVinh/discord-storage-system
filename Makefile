CXX = g++
CXXFLAGS = -Wall -Iincludes

TARGET = app

SOURCES = $(wildcard source/*.cc)
MAIN = main.cc
OBJECTS = $(SOURCES:.cc=.o) $(MAIN:.cc=.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)
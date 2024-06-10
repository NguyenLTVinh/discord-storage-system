CXX = g++
CXXFLAGS = -Wall -Iincludes -I/usr/include
LDFLAGS = -ldpp

TARGET = app

SOURCES = $(wildcard source/*.cc)
MAIN = main.cc
OBJECTS = $(SOURCES:.cc=.o) $(MAIN:.cc=.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

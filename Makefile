CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
SRCDIR = .
SOURCES = $(wildcard $(SRCDIR)/*.cpp) \
          $(wildcard $(SRCDIR)/game/*.cpp) \
          $(wildcard $(SRCDIR)/core/*.cpp) \
          $(wildcard $(SRCDIR)/roles/*.cpp) \
          $(wildcard $(SRCDIR)/ui/*.cpp) \
          $(wildcard $(SRCDIR)/data/*.cpp) \
          $(wildcard $(SRCDIR)/utils/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = dispatch_game

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run
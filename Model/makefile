CXX = g++
CXXFLAGS = -Wall -g
TARGET = myprogram
SRCS = function.cpp model.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
LIBS = -lgsl -lgslcblas
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
clean:
	rm -f $(OBJS) $(TARGET)
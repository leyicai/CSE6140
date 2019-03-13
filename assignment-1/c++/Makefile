# Kasimir Gabert
# Georgia Institute of Technology, Fall 2018

BIN = hc_centrality
CPP = g++
CPPFLAGS = -std=c++11 -Wall -Wextra -g

SRCS = $(sort $(wildcard *.cpp))

OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -MP -MMD -c $< -o $@

$(BIN): $(OBJS)
	$(CPP) $(CPPFLAGS) $^ -o $@

.PHONY: all
all: $(BIN)

.PHONY: clean
clean:
	rm -vf $(OBJS)
	rm -vf $(DEPS)
	rm -vf $(BIN)

-include $(DEPS)

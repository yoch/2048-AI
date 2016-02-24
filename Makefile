CXX=g++
CXXFLAGS=-std=c++11 -MMD -MP -W -Wall -Wextra -O3
LDFLAGS=
EXEC=2048
SRCDIR=src
OBJDIR=build
SRC=$(wildcard $(SRCDIR)/*.cpp)
OBJ=$(addprefix $(OBJDIR)/, $(notdir $(SRC:.cpp=.o)))

-include $(OBJ:.o=.d)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(SRCDIR)/%.cpp: $(SRCDIR)/%.cpp

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

all: $(EXEC)

.PHONY: clean mrproper

clean:
	rm -rf $(OBJ)

mrproper: clean
	rm -rf $(EXEC)

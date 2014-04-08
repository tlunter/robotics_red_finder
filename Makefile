CC=gcc
CFLAGS=-c -Wall $(shell pkg-config --cflags opencv)
LIBS=$(shell pkg-config --libs opencv)
SRCDIR=src
OBJDIR=obj
BINDIR=build
SOURCES=$(wildcard $(SRCDIR)/*.cpp)
OBJECTS=$(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))
INCLUDES=$(wildcard $(SRCDIR)/*.h)
EXECUTABLE=camera_capture

all: out $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(BINDIR)/$@ $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(INCLUDES)
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

out:
	mkdir -p $(SRCDIR) $(OBJDIR) $(BINDIR)

clean:
	rm -rf $(OBJDIR)/*.o $(BINDIR)/$(EXECUTABLE)

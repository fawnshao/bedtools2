UTILITIES_DIR = ../utils/
OBJ_DIR = ../../obj/
BIN_DIR = ../../bin/

# -------------------
# define our includes
# -------------------
INCLUDES = -I$(UTILITIES_DIR)/bedFile/ \
           -I$(UTILITIES_DIR)/gzstream/ \
           -I$(UTILITIES_DIR)/GenomeFile/ \
           -I$(UTILITIES_DIR)/lineFileUtilities/ \
           -I$(UTILITIES_DIR)/fileType/ \
           -I$(UTILITIES_DIR)/BamTools/include \
		   -I$(UTILITIES_DIR)/BlockedIntervals \
		   -I$(UTILITIES_DIR)/version/
# ----------------------------------
# define our source and object files
# ----------------------------------
SOURCES= genomeCoverageMain.cpp genomeCoverageBed.cpp genomeCoverageBed.h
OBJECTS= genomeCoverageMain.o genomeCoverageBed.o
BUILT_OBJECTS= $(patsubst %,$(OBJ_DIR)/%,$(OBJECTS))


all: $(BUILT_OBJECTS)

.PHONY: all

$(BUILT_OBJECTS): $(SOURCES)
	@echo "  * compiling" $(*F).cpp
	@$(CXX) -c -o $@ $(*F).cpp $(LDFLAGS) $(CXXFLAGS) $(INCLUDES)

clean:
	@echo "Cleaning up."
	@rm -f $(OBJ_DIR)/genomeCoverageMain.o $(OBJ_DIR)/genomeCoverageBed.o

.PHONY: clean

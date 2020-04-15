SOURCES=main.cpp Messaging.cpp CarPark5000.cpp Car.cpp Entry.cpp Exit.cpp
OBJECTS=$(addprefix ${BUILD_DIR}/,$(SOURCES:.cpp=.o))
DEPS=$(addprefix ${BUILD_DIR}/,$(SOURCES:.cpp=.d))
EXE=${BIN_DIR}/CarPark5000
CXXFLAGS?=-I.
LIBS=-lpthread
CXX=g++
DEBUG ?= false

#Default target is host
ARCH ?= host

#If build is for host
ifeq (${ARCH}, host)
CXX=g++
BUILD_DIR=build/host
BIN_DIR=bin/host
endif

#If build is for target
ifeq (${ARCH}, target)
CXX=arm-rpizw-g++
BUILD_DIR=build/target
BIN_DIR=bin/target
endif

#Checking for segfaults and undefined by setting debug true and build for host
ifeq (${DEBUG}, true)
ifeq (${ARCH}, host)
CXXFLAGS=-I. -fsanitize=address,undefined
endif
endif

#Linking the executable
$(EXE): $(DEPS) $(OBJECTS)
	mkdir -p ${BIN_DIR}
	$(CXX) $(OBJECTS) -o $@ $(CXXFLAGS) $(LIBS)

#Building object files
${BUILD_DIR}/%.o: %.cpp ${BUILD_DIR}/%.d
	$(CXX) -c -o $@ $< $(CXXFLAGS)

#Building makefiles (.d)
${BUILD_DIR}/%.d: %.cpp
	mkdir -p ${BUILD_DIR}
	$(CXX) -MT$@ -MM $(CXXFLAGS) $< > $@
	$(CXX) -MT$(@:.d=.o) -MM $(CXXFLAGS) $< >> $@

ifneq ($(filter-out clean format tidy ,$(MAKECMDGOALS)),)
-include $(DEPS)
endif

.PHONY: clean format tidy

clean:
	rm -rf build bin *.format *.tidy

#Formatting to tidy up .cpp files
format: $(SOURCES:.cpp=.format)
%.format: %.cpp
	@echo "Formatting file '$<'"...
	@clang-format -i $<
	@echo "" > $@

#Tidying up the .cpp files
tidy: $(SOURCES:.cpp=.tidy)
%.tidy: %.cpp
	@echo "Tidying file '$<'"...
	@clang-tidy $< -- $(CXXFLAGS)
	@echo "" > $@

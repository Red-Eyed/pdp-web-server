
MAKEFLAGS+="-j -l $(shell grep -c ^processor /proc/cpuinfo) "
GTEST_DIR = ./gtest

# Where to find user code.
USER_DIR_TEST = .
USER_DIR = ..

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread -I../ -I./ -I./gtest/include/

# All tests produced by this Makefile.  Remember to add new tests you
# created to the list.
TESTS = Web_Server_On_Cpp_Tests

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h $(GTEST_DIR)/include/gtest/internal/*.h

# House-keeping build targets.

all : $(TESTS)

clean :
	rm -f $(TESTS) gtest.a gtest_main.a *.o

# Builds gtest.a and gtest_main.a.

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# Builds test:

DownloadFile.o : $(USER_DIR)/DownloadFile.cpp $(USER_DIR)/DownloadFile.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/DownloadFile.cpp


Server.o : $(USER_DIR)/Server.cpp $(USER_DIR)/Server.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/Server.cpp


Thread.o : $(USER_DIR)/Server.cpp $(USER_DIR)/Thread.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/Thread.cpp

ViewContentDir.o : $(USER_DIR)/ViewContentDir.cpp $(USER_DIR)/ViewContentDir.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/ViewContentDir.cpp

ServerExeption.o : $(USER_DIR)/ServerExeption.cpp $(USER_DIR)/ServerExeption.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/ServerExeption.cpp

WebServerTest.o : $(USER_DIR_TEST)/WebServerTest.cpp $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR_TEST)/WebServerTest.cpp

$(TESTS) : DownloadFile.o Server.o Thread.o ViewContentDir.o ServerExeption.o WebServerTest.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

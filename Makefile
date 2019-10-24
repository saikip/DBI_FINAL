### Remember to add new mains to the MAINS variable
CC := g++ -pthread -std=c++0x
DEBUG = no
WARNINGS = no

ifeq ($(DEBUG), yes)
	FLAGS += -g
else
	FLAGS += -O2
endif

ifeq ($(WARNINGS), yes)
	FLAGS += -Wall
else
	FLAGS += -w
endif

CCFLAGS = $(FLAGS) -Wno-deprecated

# Vars
SRCDIR := src
BUILDDIR := build
LIBDIR := lib
INCLUDEDIR := include
TARGETDIR := bin

MKDIR_P = mkdir -p

SRCEXT := cc
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
ALL_OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall
LIB := -L lib
INC := -I include
PARSING := $(BUILDDIR)/y.tab.o $(BUILDDIR)/lex.yy.o $(BUILDDIR)/yyfunc.tab.o $(BUILDDIR)/lex.yyfunc.o $(BUILDDIR)/yysql.tab.o $(BUILDDIR)/lex.yysql.o
MAINS := $(BUILDDIR)/main.o $(BUILDDIR)/test.o $(BUILDDIR)/entryPoint1.o $(BUILDDIR)/entryPoint2a.o $(BUILDDIR)/entryPoint2b.o $(BUILDDIR)/entryPoint3.o $(BUILDDIR)/entryPoint4_1.o $(BUILDDIR)/entryPoint4_2.o

# Objects excluding main
OBJECTS := $(filter-out $(MAINS),$(ALL_OBJECTS)) $(PARSING)

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread -w
CPPFLAGS += -isystem $(GTEST_DIR)/include -isystem $(GMOCK_DIR)/include

# Bison/Flex stuff
tag = -i

ifdef linux
tag = -n
flex_lib = -lfl
sed = sed -n src/y.tab.c -e "s/  __attribute__ ((__unused__))$$/# ifndef __cplusplus\n  __attribute__ ((__unused__));\n# endif/“
endif

# Check if on OS X
ifeq "$(OSTYPE)" "darwin"
tag = -n
endif

###### Main Build ######
# Build all
.PHONY: all
all: dirs main entryPoint1 entryPoint2a entryPoint2b entryPoint3 entryPoint4_1 entryPoint4_2 test

# Create directories
.PHONY: dirs
dirs: $(TARGETDIR)

$(TARGETDIR):
	$(MKDIR_P) $(TARGETDIR)

# Build main
.PHONY: main
main: $(OBJECTS) $(BUILDDIR)/main.o
	$(CC) $(CCFLAGS) -o $(TARGETDIR)/main $^ $(lfl)

# Build entryPoint1
.PHONY: entryPoint1
entryPoint1: $(OBJECTS) $(BUILDDIR)/entryPoint1.o
	$(CC) $(CCFLAGS) -o $(TARGETDIR)/entryPoint1 $^ $(lfl)
	
# Build entryPoint2a
.PHONY: entryPoint2a
entryPoint2a: $(OBJECTS) $(BUILDDIR)/entryPoint2a.o
	$(CC) $(CCFLAGS) -o $(TARGETDIR)/entryPoint2a $^ $(lfl)
	
# Build entryPoint2b
.PHONY: entryPoint2b
entryPoint2b: $(OBJECTS) $(BUILDDIR)/entryPoint2b.o
	$(CC) $(CCFLAGS) -o $(TARGETDIR)/entryPoint2b $^ $(lfl)
	
# Build entryPoint3
.PHONY: entryPoint3
entryPoint3: $(OBJECTS) $(BUILDDIR)/entryPoint3.o
	$(CC) $(CCFLAGS) -o $(TARGETDIR)/entryPoint3 $^ $(lfl)
	
# Build entryPoint4_1
.PHONY: entryPoint4_1
entryPoint4_1: $(OBJECTS) $(BUILDDIR)/entryPoint4_1.o
	$(CC) $(CCFLAGS) -o $(TARGETDIR)/entryPoint4_1 $^ $(lfl)

# Build entryPoint4_2
.PHONY: entryPoint4_2
entryPoint4_2: $(OBJECTS) $(BUILDDIR)/entryPoint4_2.o
	$(CC) $(CCFLAGS) -o $(TARGETDIR)/entryPoint4_2 $^ $(lfl)

# Compile cc files
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CCFLAGS) $(INC) -c -o $@ $<

# Compile Flex/Bison files
$(BUILDDIR)/y.tab.o: $(SRCDIR)/Parser.y
	(cd $(SRCDIR);yacc -d Parser.y)
	$(sed)
	(mv $(SRCDIR)/y.tab.h $(INCLUDEDIR))
	g++ $(CCFLAGS) $(INC) -c -o $@ $(SRCDIR)/y.tab.c

$(BUILDDIR)/yyfunc.tab.o: $(SRCDIR)/ParserFunc.y
	(cd $(SRCDIR);yacc -p "yyfunc" -b "yyfunc" -d ParserFunc.y)
	(mv $(SRCDIR)/yyfunc.tab.h $(INCLUDEDIR))
	g++ $(CCFLAGS) $(INC) -c -o $@ $(SRCDIR)/yyfunc.tab.c
	#yacc -p "yyfunc" -b "yyfunc" -d ParserFunc.y
	#sed $(tag) yyfunc.tab.c -e "s/  __attribute__ ((__unused__))$$/# ifndef __cplusplus\n  __attribute__ ((__unused__));\n# endif/"
	
$(BUILDDIR)/yysql.tab.o: $(SRCDIR)/ParserSQL.y
	(cd $(SRCDIR);yacc -p "yysql" -b "yysql" -d ParserSQL.y)
	(mv $(SRCDIR)/yysql.tab.h $(INCLUDEDIR))
	g++ $(CCFLAGS) $(INC) -c -o $@ $(SRCDIR)/yysql.tab.c 
$(BUILDDIR)/lex.yy.o: $(SRCDIR)/Lexer.l
	(cd $(SRCDIR);lex  Lexer.l)
	gcc $(CCFLAGS) $(INC) -c -o $@ $(SRCDIR)/lex.yy.c

$(BUILDDIR)/lex.yyfunc.o: $(SRCDIR)/LexerFunc.l
	(cd $(SRCDIR);lex -Pyyfunc LexerFunc.l)
	gcc $(CCFLAGS) $(INC) -c -o $@ $(SRCDIR)/lex.yyfunc.c
	
$(BUILDDIR)/lex.yysql.o: $(SRCDIR)/LexerSQL.l
	(cd $(SRCDIR);lex -Pyysql LexerSQL.l)
	gcc $(CCFLAGS) $(INC) -c -o $@ $(SRCDIR)/lex.yysql.c

	
########## Testing ##########
# Test Vars
TESTDIR := test
TEST_SOURCES := $(shell find $(TESTDIR) -type f -name *.$(SRCEXT))
ALL_TEST_OBJECTS := $(patsubst $(TESTDIR)/%,$(BUILDDIR)/%,$(TEST_SOURCES:.$(SRCEXT)=.o))
TESTS := $(BUILDDIR)/test.o

# Objects excluding Test
TEST_OBJECTS := $(filter-out $(TESTS),$(ALL_TEST_OBJECTS)) $(PARSING)

# GoogleTest Vars
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
GTEST_DIR = lib/gtest-1.7.0
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
								$(GTEST_DIR)/include/gtest/internal/*.h
GMOCK_DIR = lib/gmock-1.7.0
GMOCK_HEADERS = $(GMOCK_DIR)/include/gmock/*.h \
								$(GMOCK_DIR)/include/gmock/internal/*.h \
								$(GTEST_HEADERS)
GMOCK_SRCS_ = $(GMOCK_DIR)/src/*.cc $(GMOCK_HEADERS)
GTESTLIBS := $(LIBDIR)/gtest_main.a $(LIBDIR)/gmock_main.a
GTEST := $(LIBDIR)/gtest.a
GTEST_MAIN := $(LIBDIR)/gtest_main.a
GMOCK := $(LIBDIR)/gmock.a
GMOCK_MAIN := $(LIBDIR)/gmock_main.a


###### Test Builds ######
.PHONY: test
test: $(OBJECTS) $(TEST_OBJECTS) $(BUILDDIR)/test.o $(GTESTLIBS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $(TARGETDIR)/$@

# Compile tests cc files
$(BUILDDIR)/%.o: $(TESTDIR)/%.$(SRCEXT) $(GTEST_HEADERS)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) $(INC) -c -o $@ $<

# Create GTest and GMock libs
$(BUILDDIR)/gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
		-o $@ $(GTEST_DIR)/src/gtest-all.cc

$(BUILDDIR)/gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
		-o $@ $(GTEST_DIR)/src/gtest_main.cc

$(LIBDIR)/gtest.a : $(BUILDDIR)/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(LIBDIR)/gtest_main.a : $(BUILDDIR)/gtest-all.o $(BUILDDIR)/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

$(BUILDDIR)/gmock-all.o : $(GMOCK_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(CXXFLAGS) \
	-c -o $@ $(GMOCK_DIR)/src/gmock-all.cc

$(BUILDDIR)/gmock_main.o : $(GMOCK_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(CXXFLAGS) \
	-c -o $@ $(GMOCK_DIR)/src/gmock_main.cc

$(LIBDIR)/gmock.a : $(BUILDDIR)/gmock-all.o $(BUILDDIR)/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(LIBDIR)/gmock_main.a : $(BUILDDIR)/gmock-all.o $(BUILDDIR)/gtest-all.o $(BUILDDIR)/gmock_main.o
	$(AR) $(ARFLAGS) $@ $^

###### Clean ######
.PHONY: clean
clean:
	rm -f $(ALL_OBJECTS)
	rm -f $(ALL_TEST_OBJECTS)
	rm -f $(TARGETDIR)/*
	rm -f $(SRCDIR)/y.tab.c
	rm -f $(SRCDIR)/lex.yy.c
	rm -f $(SRCDIR)/y.tab.h
	rm -f $(BUILDDIR)/lex.yy.o
	rm -f $(BUILDDIR)/y.tab.o
	rm -f $(SRCDIR)/yyfunc.tab.c
	rm -f $(SRCDIR)/lex.yyfunc.c
	rm -f $(SRCDIR)/yyfunc.tab.h
	rm -f $(BUILDDIR)/lex.yyfunc.o
	rm -f $(BUILDDIR)/yyfunc.tab.o
	rm -f $(SRCDIR)/yysql.tab.c
	rm -f $(SRCDIR)/lex.yysql.c
	rm -f $(SRCDIR)/yysql.tab.h
	rm -f $(BUILDDIR)/lex.yysql.o
	rm -f $(BUILDDIR)/yysql.tab.o	
	

.PHONY: deep-clean
deep-clean: clean
	rm -f *.o
	rm -f *.out
	rm -f $(BUILDDIR)/*.o
	rm -f $(BUILDDIR)/*.out
	rm -f $(LIBDIR)/*.a

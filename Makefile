NAME = webserv
CXX = clang++
CXXFLAGS =  -Wall -Wextra -Werror -std=c++98 -I$(INCLUDES_PATH)
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes

# **************************************************************************** #

SAFE_MKDIR = mkdir -p

COPY = cp -p

REMOVE = rm -rf

OBJECTS_PATH = $(PWD)/objects
SOURCES_PATH = $(PWD)/sources
INCLUDES_PATH = $(PWD)/includes
LIBS_PATH = $(PWD)/libs
ARCHIVES_PATH = $(PWD)/archives

# ****************************************************************************
#

HEADER_FILE = algorithm.hpp

HEADER = $(addprefix $(INCLUDES_PATH)/,$(HEADER_FILE))

SOURCE_FILES =	$(shell find ./sources -type f \( -iname "*.cpp" ! -name "main*" \) )

SOURCES = $(addprefix $(SOURCES_PATH)/,$(SOURCE_FILES))

OBJECTS = $(addprefix $(OBJECTS_PATH)/,$(subst .cpp,.o,$(SOURCE_FILES)))

TARGET = bin/$(NAME)

# **************************************************************************** #
#

ifeq (test,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "run"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif

# **************************************************************************** #


CPP_FILES = $(shell find $(SOURCES_PATH) -name "*.cpp")
OBJ_FILES = $(CPP_FILES:$(SOURCES_PATH)/%.cpp=$(OBJECTS_PATH)/%.o)

.PHONY: all run valgrind re fclean clean test $(NAME)

all: $(NAME)

$(NAME): $(OBJ_FILES)
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o bin/$@ $^

$(OBJECTS_PATH)/%.o: $(SOURCES_PATH)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

test:
	@mkdir -p bin
	@$(CXX) $(CXXFLAGS) -I $(INCLUDES_PATH) $(SOURCE_FILES) tests/$(RUN_ARGS).cpp -o bin/$(RUN_ARGS)
	@./bin/$(RUN_ARGS)

parsers:
	@mkdir -p bin
	@$(CXX) $(CXXFLAGS) -I $(INCLUDES_PATH) ./sources/parsers/default/parser.cpp ./tests/$@.cpp -o bin/$@
	@./bin/$@

clean:
	rm -rf $(OBJECTS_PATH) $(NAME)

fclean: clean
	@$(REMOVE) $(TARGET)
	@rm -r bin

valgrind: all
	@$(VALGRIND) ./$(TARGET)

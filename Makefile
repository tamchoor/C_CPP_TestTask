NAME_SERV = server
NAME_CL = client

CC = clang++ -std=c++14
# FLAGS = -Wall -Werror -Wextra -g
FLAGS = -Wall -Werror -Wextra -g -fsanitize=address
# FLAGS = -Wall -Werror -Wextra 

# LIBRARIES =  -lpthread -L/Users/$(USER)/Desktop/boost_1_80_0/stage/lib -lboost_system -lboost_thread  -lboost_thread -lboost_filesystem -lboost_log_setup -lboost_log
# INCLUDES = -I/Users/$(USER)/Desktop/boost_1_80_0

SOURCES_SERV =  server.cpp 
SOURCES_CL =  client.cpp 

OBJS_S = $(SOURCES_SERV:%.cpp=%.o)
OBJS_C = $(SOURCES_CL:%.cpp=%.o)
OBJS = $(OBJS_C) $(OBJS_S)

GREEN = \033[0;32m
RED = \033[0;31m
CROSS = \033[9m
MARK = \033[7m
RESET = \033[0m

.PHONY: all clean fclean re

all: $(NAME_CL) $(NAME_SERV) 

ALL_OBJS_DIR = $(DIR_SRC)

$(NAME_SERV):
# @$(CC) $(FLAGS) $(INCLUDES)  $(LIBRARIES) $(OBJS_S) -o $(NAME_SERV)
	$(CC) -I/Users/anastasiadementeva/Desktop/boost_1_80_0 -std=c++14 -lpthread server.cpp -o $(NAME_SERV) -L/Users/anastasiadementeva/Desktop/boost_1_80_0/stage/lib -lboost_system -lboost_thread  -lboost_thread -lboost_filesystem -lboost_log_setup -lboost_log
# @echo "\n$(MARK) $(NAME_SERV): $(GREEN)object files were created$(RESET)"
	@echo "$(MARK) $(NAME_SERV): $(GREEN)$(NAME_SERV) was created$(RESET)"

$(NAME_CL): $(OBJS_C) 
	@$(CC) $(FLAGS) $(OBJS_C) -o $(NAME_CL)
	@echo "\n$(MARK) $(NAME_CL): $(GREEN)object files were created$(RESET)"
	@echo "$(MARK) $(NAME_CL): $(GREEN)$(NAME_CL) was created$(RESET)"


$(OBJS_C)%.o : %.cpp
	$(CC) $(FLAGS) -c $< -o $@

# $(OBJS_S)%.o : %.cpp
# 	$(CC) $(FLAGS) $(LIBRARIES) $(INCLUDES) -I/Users/$(USER)/Desktop/boost_1_80_0 -c $< -o $@

clean:
	@rm -rf *.o
	@echo "$(NAME): $(RED) $(CROSS)object $(RED) files were deleted$(RESET)"

fclean: clean
	@rm -rf $(NAME_CL) $(NAME_SERV)
	@echo "$(CROSS) $(NAME) : $(RED)$(NAME_SERV) and $(NAME_CL) was deleted$(RESET)"

re: fclean all
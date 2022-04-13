# compile flag
CXX = c++
CXXFLAGS = -std=c++98

# exec name
NAME = $(WEBSERVER)
WEBSERVER = webserver

# file name
SOURCE = utils.cpp
WEBSERVER_MAIN = webserv_get.cpp
HEADER = socket.hpp
OBJECT = $(SOURCE:.cpp=.o)

# dependency
$(NAME): $(OBJECT) $(HEADER) $(WEBSERVER_MAIN)
	$(CXX) $(CXXFLAGS) $(WEBSERVER_MAIN) $(OBJECT) -o $(WEBSERVER)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# rules
all: $(NAME)
clean:
	rm -rf $(OBJECT)
fclean:
	rm -rf $(OBJECT) $(NAME)
re: fclean all

.PHONY: all clean fclean re
# compile flag
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

# exec name
NAME = $(SERVER) $(CLIENT)
SERVER = server
CLIENT = client

# file name
SOURCE = utils.cpp
SERVER_MAIN = server.cpp
CLIENT_MAIN = client.cpp
HEADER = socket.hpp
OBJECT = $(SOURCE:.cpp=.o)

# dependency
$(NAME): $(OBJECT) $(HEADER)
	$(CXX) $(CXXFLAGS) $(SERVER).cpp $(OBJECT) -o $(SERVER)
	$(CXX) $(CXXFLAGS) $(CLIENT).cpp $(OBJECT) -o $(CLIENT)
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
# compile flag
CXX = c++
CXXFLAGS = -std=c++98

# exec name
NAME = $(SERVER) $(CLIENT)
SERVER = server
CLIENT = client

# file name
SOURCE = utils.cpp
SERVER_MAIN = kqueue_server.cpp
CLIENT_MAIN = kqueue_client.cpp
HEADER = socket.hpp
OBJECT = $(SOURCE:.cpp=.o)

# dependency
$(NAME): $(OBJECT) $(HEADER) $(SERVER_MAIN) $(CLIENT_MAIN)
	$(CXX) $(CXXFLAGS) $(SERVER_MAIN) $(OBJECT) -o $(SERVER)
	$(CXX) $(CXXFLAGS) $(CLIENT_MAIN) $(OBJECT) -o $(CLIENT)
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
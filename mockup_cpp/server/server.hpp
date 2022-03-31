/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jekim <jekim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 19:04:11 by jekim             #+#    #+#             */
/*   Updated: 2022/03/31 23:43:11 by jekim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <list>
#include <sys/socket.h>
#include <iostream>

# define SERVER_ERR -1

class Exception_reporter
{
    public :
        virtual void report(){ std::cerr << "Error" << std::endl; };
};

class connection_pool
{
    public :
        typedef std::list<int>::iterator       iterator;


        void push_fd(int fd) { connection_fd_pool.push_back(fd); };
        void delete_fd(int fd) { connection_fd_pool.remove(fd); };
        bool is_valid_fd(int fd) {
            iterator it = std::find(connection_fd_pool.begin(), connection_fd_pool.end(), fd);
            return (it == connection_fd_pool.end()) ? false : true;
        }

        class ConnectionPoolMaxSizeException : public Exception_reporter
        {
            virtual void report (void)
            {
                std::cerr << "Error : socket_connection_pool overflowed" << std::endl;
            };
        };

    private :
        connection_pool() : max(OPEN_MAX) {};
        ~connection_pool() { connection_fd_pool.empty();};
        connection_pool(connection_pool& other) {};
        std::list<int> connection_fd_pool;
        int max;
};

class server
{
    private :
        server(){
            this->server_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
            if (this->server_fd == SERVER_ERR)
                throw SocketCreateError(); 
        };
        server(server& other) {};
        ~server() {};
        server& operator= (server& other) {};
        int server_fd;

    public :
        server& getInstance()
        {
            static server s;
            return s;
        };
        class SocketCreateError : public Exception_reporter
        {
            virtual void report (void)
            {
                std::cerr << "Error : occured an error while the socket creation" << std::endl;
            }
        };
};

#endif // SERVER_HPP

// #include "../headers/Command.hpp"
#include "../headers/Privmsg.hpp"
#include "../headers/Server.hpp"

int Server::PRIVMSG(int poll_fd)
{
	if (send(poll_fd, const_cast<char*>(_line[2].c_str()), _line[2].size(), 0) == -1)
		perror("send");
	return (45676);
}

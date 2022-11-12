
#include "../headers/Server.hpp"

// void Server::errorMessage( User user, int numReply, std::string arg)
// {
// 	std::map<int, std::string> err;
// 	std::ostringstream s;

//     if (*(arg.end() - 1) == '\n')
//         arg = arg.substr(0, arg.length() - 1);
//     s << ":mfirc " << numReply << " * " << arg << err[numReply] << "\r\n";

//     std::string msg = s.str();

//     if ( send(user.getFd(), &msg[0], msg.size(), 0) == -1 )
//         throw errorExcute(strerror(errno));
// }

#include "../headers/headers.hpp"
//#include "../headers/Command.hpp"
#include "../headers/Server.hpp"

int	Server::PASS(int poll_fd)
{
	if (_authentified[_User] == 1)
	{
		if (send(poll_fd, "Bad Usage: PASS already good !\n\r", 32, 0) == -1)
			perror("send");
		_line.clear();
		_IP_tmp.clear();
		return (2000);
	}
	if (_pass.compare(_line[1]) == 0)
	{
		if (send(poll_fd, "Good PASS !\n\r", 13, 0) == -1)
			perror("send");
//              authentification réussi, pour le pass en tout cas
		_line.clear();
		_tmp_authentified[_User] = 1;
		_Identities[_User].push_back(_IP_tmp);
	//	User_Count++;
		_IP_tmp.clear();
		return(3);
	}
	else
	{
//		voir comment doivent etre géré les numerics
		std::cout << "le mot de passe invalide est : " << _line[1] << std::endl;
		if (send(poll_fd, "Bad Password: Casse toi pov' con !\n\r", 36, 0) == -1)
			perror("send");
		_line.clear();
		_IP_tmp.clear();
		return (400);
	}	
}

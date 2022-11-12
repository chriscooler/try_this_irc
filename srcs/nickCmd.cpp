#include "../headers/headers.hpp"
// #include "../headers/Command.hpp"
#include "../headers/Server.hpp"

int	Server::NICK(int poll_fd)
{

	if (_line[1].length() > 0 && _line[2].empty() 
	&& _Identities[_User][1].empty())
	{
		_Identities[_User].push_back(_line[1]);
		_Nicks.push_back(_line[1]);
		if (send(poll_fd, "All Good: Your nick is set !\n\r", 29, 0) == -1)
			perror("send");
		_line.clear();
		_nick_already_set[_User] = 1;
		_tmp_authentified[_User] = 2;
		return (20);
	}
	else if (_line[1].length() > 0 && _line[2].empty() 
	&& _Identities[_User][1].empty() == 0
	&& _Identities[_User][1].compare(_line[1]) != 0)
	{
		_Identities[_User][1].erase(0, 
			_Identities[_User][1].size());
		_Identities[_User][1] = _line[1];
		_Nicks[_User].erase(0, _Nicks[_User].size() );
		_Nicks[_User] = _line[1];
		if (send(poll_fd, "All Good: Your nick have changed !\n\r", 35, 0) == -1)
			perror("send");
		_line.clear();
		return (60);
	}
	else if (_line[1].length() > 0 && _line[2].empty() 
	&& _Identities[_User][1].compare(_line[1]) == 0)
	{
		if (send(poll_fd, "Bad Param': Same nick\n\r", 23, 0) == -1)
			perror("send");
		_line.clear();
		return (76);
	}
// Peut etre ajouter sécu
	else
	{
		if (send(poll_fd, "Bad param's: You're nick is not set !\n\r", 39, 0) == -1)
			perror("send");
		_line.clear();
	}
	return (0);
}

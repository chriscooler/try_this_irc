#include "../headers/headers.hpp"
#include "../headers/Command.hpp"
#include "../headers/parse_message.hpp"
#include "../headers/Server.hpp"

int	Server::command_check(int poll_fd)
{
//	cette fonction nous permettra de séparer les instructions d'un message envoyé		par un client afin de repérer la commande et ces paramètres ainsi que de repérer 	les potentielles erreurs --- Suite de receive_message

// il faudra créer une fonction fill_cmd ou quelque chose du genre
	int i = 0;


	while (!_all_commands[i].empty())
	{	
		if (_line[0] == _all_commands[i])  
		{
			if (_line[1].size() > 0)
			{
				if (_line[0] == _all_commands[0]
				&& _line[1].empty() == 0
				&& _line[2].empty() != 0
				&& (_tmp_authentified[_User] == 0 |
				 _authentified[_User] == 1))
				{
					PASS(poll_fd);
					return (0);
				}
				if (_line[0] == _all_commands[1]
				&& _line[1].empty() == 0
				&& _line[2].empty() != 0
				&& (_tmp_authentified[_User] == 1
				| _authentified[_User] == 1))
				{
					NICK(poll_fd);
					return (0);
				}
/* condition supposé de user :*/ if (_line[0] == _all_commands[2]
				&& _line[1].empty() == 0
				&& _line[2].empty() == 0
				&& _line[3].empty() == 0
				&& _line[4].empty() != 0
				&& (_tmp_authentified[_User] == 2
				| _authentified[_User] == 1))
				{
					std::cout << "coucou" << std::endl;
					USER(poll_fd);
					return (0);
				}

// une fois pass, nick et user de fait, authentified = 1

				if ((_line[0] == _all_commands[3]
				&& _line[1].empty() == 0
				&& _line[2].empty() != 0
				&& _authentified[_User] == 1) |
				(_line[0] == _all_commands[3]
				&& _line[1].empty() == 0
				&& _line[2].size() > 0
				&& _line[3].empty() != 0
				&& _authentified[_User] == 1))
				{
					JOIN(poll_fd);
					return (0);
				}
			//	authentified[User] = 1;   ---   Usage temporaire
				if (_line[0] == _all_commands[4]
				&& _line[1].empty() == 0
				&& _line[2].empty() == 0
				&& _line[3].empty() != 0
				&& _authentified[_User] == 1)
				{
					PRIVMSG(poll_fd);
					return (0);
				}
				
				if (_authentified[_User] == 0)
				{
					if (send(poll_fd, "Bad Usage: You're not authorized !\n\r", 37, 0) == -1)
						perror("send");
					_line.clear();
					_IP_tmp.clear();
					return (565);
				}
			}
			else
			{
				if (send(poll_fd, "Bad Param': On refait bien la, oh !\n\r", 37, 0) == -1)
					perror("send");
				_line.clear();
				_IP_tmp.clear();
				return (598);
			}
		}
		i++;
	}
	_msg = "Bad Command: Faites un effort, voyons.\n\r";
	_error = "send";
	if (send(poll_fd, _msg.c_str() , 40, 0) == -1)
		perror(_error.c_str());
	_line.clear();
	_IP_tmp.clear();
	return (458);	
}

int	Server::receive_message(int poll_fd)
{
	char		buf[9999];	

	if (recv(poll_fd, buf, 9999, 0) == -1)
	{
		perror("send :");
	}
	for (int it = 0; ((buf[it] != '\n' | buf[it] != '\r') && it <= 512) ; 			it++)
	{
		if (buf[it] == ':' && (buf[it + 1] != ' ' | buf[it + 1] != '\n'
			| buf[it + 1] != '\r'))
		{
		_msg = "Bad Message: Deux points, c'est zéro.\n\r";
			if (send(poll_fd, _msg.c_str(), 39, 0) == -1)
				perror("send");
			_word.clear();
			_line.clear();
			return (12);
		}	
		_word.push_back(buf[it]);
		if (buf[it + 1] == ' ')
		{
			while (buf[it + 1] == ' ')
				it++;
	//		word.push_back('\0'); Inutile de conclure par NULL
			_line.push_back(_word);
			_word.clear();
			if (buf[it + 2] == '\n' | buf[it + 2] == '\r')
				break;
		}
		if (buf[it + 1] == ':')
		{
			if (send(poll_fd, "Bad Message: Deux points, c'est zéro.\n\r", 39, 0) == -1)
				perror("send");
			_word.clear();
			_line.clear();
			_IP_tmp.clear();
			return (11);
		}	
		if (buf[it + 1] == '\n' | buf[it + 1] == '\r')
		{
			_line.push_back(_word);
			_line.push_back("\0"); // NULL pour savoir quand fin vector
			_word.clear();
		//	memset(&tmp, 0, sizeof buf); NE SURTOUT PAS REINITIALISER LE BUF
//  LA SOCKET SE FERMERAIT 
			break;
		}
		else if (buf[it + 1] == '\0')
			it += 2;
		if (it > 512)
		{
			if (send(poll_fd, "Message too long: C'est plus que Levis quand même\n\r", 50, 0) == -1)
				perror("send");
			_word.clear();
			_line.clear();
			return (19);
		}	
	}
	command_check(poll_fd);
	return (0);
}

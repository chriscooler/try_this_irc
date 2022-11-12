#include "../headers/headers.hpp"
#include "../headers/Join.hpp"
#include "../headers/Server.hpp"

int	Server::JOIN(int poll_fd)
{
	std::vector<std::string > tmp_Channel;
	std::vector<std::string > tmp_Key;
	std::string tmp;
	int	it = 0;
	int	o = 0;
	//int	tmp_Channel_Check[10];

// vérif si le 1er caractère est conforme
	if (_line[1][0] != '#' && _line[1][0] != '&' &&
		_line[1][0] != '+' && _line[1][0] != '!')
	{
		if (send(poll_fd, "Bad Param: +, !, # or & are not there\r\n",
		39, 0) == -1)
			perror("send");
		_line.clear();
		return (83);
	}

// vérif si la ligne est plus grande que 50 caracteres
	if (_line.size() > 50)
	{
		if (send(poll_fd, "Bad Param: chan name too long\r\n", 31, 0)
			== -1)
			perror("send");
		_line.clear();
		return (862);
	}

// vérif si plusieurs channels sont donnés et stockage
	for ( int i = 0; _line[1][i]; i++)
	{
		if (_line[1][i] == ' ' | _line[1][i] == 7)
		{
			if (send(poll_fd, "Bad Param: chan name contain wrong characters\r\n", 48, 0) == -1)
				perror("send");
			_line.clear();
			return (882);
		}
		tmp.push_back(_line[1][i]);
		if ( _line[1][i + 1] == '\0'
		| _line[1][i + 1] == ',' )
		{
			tmp_Channel.push_back(tmp);
			tmp.clear();
		}
		if (_line[1][i + 1] == ',' &&
		(_line[1][i + 2] == '#' | _line[1][i + 2] == '&'
		| _line[1][i + 2] == '+' | _line[1][i + 2] == '!'))
		{
			i++;
			it++;
		}
		else if (_line[1][i + 1] == ',' &&
	(_line[1][i + 2] != '#' | _line[1][i + 2] != '&'
	| _line[1][i + 2] != '+' | _line[1][i + 2] != '!'))
		{
			if (send(poll_fd, "Bad Param: chan name contain wrong characters\r\n", 48, 0) == -1)
				perror("send");
			_line.clear();
			return (882);
		}
	}
	tmp_Channel.push_back("\0");
	tmp.clear();
	for (int i = 0; i <= it; i++)
		tmp_Key.push_back("\0");
	it = 0;
// vérif des keys
	for ( int i = 0; _line[2][i] && 
	_line[2][i] != '\n' | _line[2][i] != '\r'; i++)
	{
		if (_line[2][i] == ' ' | _line[2][i] == 7)
		{
			if (send(poll_fd, "Bad Param: key contain wrong characters\r\n", 41, 0) == -1)
				perror("send");
			_line.clear();
			return (883);
		}
		tmp.push_back(_line[2][i]);
		it++;
		if ( _line[2][i + 1] == '\0'
		| _line[2][i + 1] == ',' )
		{
			tmp_Key[o].replace(0, it, tmp);
			std::cout << "key = "<< tmp_Key[o] << std::endl;
			tmp.clear();
		}
//		tmp_Key[it] = _line[2][i];
		if (_line[2][i + 1] == ',' &&
		_line[2][i + 2] == ',')
		{
			it = 0;
			i++;
			o++;
		}
	}
	o = 0;
// vérif si channel existe déja
	for ( int i = 0; _Channel_Count > 0 && i <= _Channel_Count
	&& tmp_Channel[o].compare("\0") != 0; i++)
	{
		// si on repere un channel déja existant
		if ( i < _Channel_Count && 
		tmp_Channel[o] == _Channels[i][0] &&
		tmp_Key[o] == _Channels[i][1])
// l'idée, c'est que si key n'existe pas, on mets '\0'
		{
// On part du postulat qu\'on a toujours le meme nombre d'info et que l'ordre desdonnées utilisateurs (ip, nick, user) n'a pas d'importance
			if (_Identities[_User][0] != 
			_Channels_Users[_Channels[i][0]][0])
			{
				_Channels_Users
				[_Channels[i][0]].push_back
				(_Identities[_User][0]);
				if (send(poll_fd,
				"JOIN (existing channel) complete\n\r", 34, 0)
				== -1)
					perror("send");
				it =
				_Channels_Users
				[_Channels[i][0]].size();
				for (int z =
				_Identities[_User].size(); 
				z >= it; it++)
				{
					_Channels_Users
					[_Channels[i][0]].push_back
					(_Identities[_User][it]);
				}
			}
// le if enregistre l'ip de l'user actuel si il ne l'a pas déja
// probleme : Je n'enregistre pas le nick et le user 

			/*tmp = _line[0];
			tmp += ' ';
			tmp += tmp_Channel[o];
			if (send(poll_fd, static_cast<void *>(&tmp), tmp.length(), 0) == -1)
				perror("send");
			tmp.clear();*/
			else if (_Identities[_User][0] == 
			_Channels_Users[_Channels[i][0]][0])
			{
				if (send(poll_fd,
				"You are already connected\n\r", 27, 0) == -1)
					perror("send");
			}
			
			if (_Channels[i][2].compare("\0") != 0)
// 			Channels[i][1] == key
// 			Channels[i][2] == topic 
			{
				if (send(poll_fd, const_cast<char*>(_Channels[i][2].c_str()),
				_Channels[i][2].size(), 0) == -1)
					perror("send");
			}
			else
			{
				if (send(poll_fd, "RPL No Topic\r\n", 14,
				0) == -1)
					perror("send");
			}
			o++;
			if (o == (int)tmp_Channel.size())
			{
				tmp_Channel.clear();
				tmp_Key.clear();
				_line.clear();
				return (1239);
			}
		}
		else if ( i < _Channel_Count &&
		tmp_Channel[o] == _Channels[i][0] &&
		tmp_Key[o] != _Channels[i][1])
		{
			if (send(poll_fd, "Bad param: Key is wrong\r\n",
			25, 0) == -1)
				perror("send");
			tmp_Channel.clear();
			tmp_Key.clear();
			_line.clear();
			return (5874);	
		}
		else if ( i == _Channel_Count &&
		o <= (int)tmp_Channel.size() )
		{
			_Channels[i].push_back(tmp_Channel[o]);
			_Channels[i].push_back("\0");
			_Channels[i].push_back("\0");
			tmp = _line[0];
			tmp += ' ';
			tmp += tmp_Channel[o];
			if (send(poll_fd, const_cast<char*>(tmp.c_str()), tmp.size(), 0) == -1)
				perror("send");
			tmp.clear();
			if (send(poll_fd, "RPL No Topic\r\n", 14, 0) == -1)
				perror("send");
			it =
			_Channels_Users[_Channels[i][0]].size()+ 1;
			for (int z = _Identities[_User].size();
			z > it; it++)
			{
				
				_Channels_Users
				[_Channels[i][0]].push_back
				(_Identities[_User][it]);
				if (send(poll_fd,
				"JOIN (known channel) complete\n\r", 31, 0)
				== -1)
					perror("send");
			}
			_Channel_Count++;
			o++;
			i = -1;
		}
	}
// Continuer le traitement de JOIN - Ici, gérer cas création due a channel 
// non trouvé
	if ( _Channel_Count == 0 )
	{
		for (int i = 0; i < (int)tmp_Channel.size() &&
		tmp_Channel[i].compare("\0") != 0; i++)
		{
			if (tmp_Key[i].compare("\0") != 0)
			{
				if (send(poll_fd, "Bad param: Key is wrong\r\n",
				25, 0) == -1)
					perror("send");
				tmp_Channel.clear();
				tmp_Key.clear();
				_line.clear();
				return(5874);	
			}
			_Channels[i].push_back(tmp_Channel[i]);
			_Channels[i].push_back(tmp_Key[i]);
			_Channels[i].push_back("\0");
			tmp = _line[0];
			tmp += ' ';
			std::cout << tmp_Channel[i] << std::endl;
			tmp += tmp_Channel[i];
			if (send(poll_fd, const_cast<char*>(tmp.c_str()), tmp.size(), 0) == -1)
				perror("send");
			tmp.clear();
			if (send(poll_fd, "RPL No Topic\r\n", 14, 0) == -1)
				perror("send");
			_Channels_Users
			[_Channels[i][0]].push_back
			(_Identities[_User][0]);
			_Channel_Count++;
		}
	}
	tmp_Channel.clear();
	tmp_Key.clear();
	_line.clear();
	return (0);
}

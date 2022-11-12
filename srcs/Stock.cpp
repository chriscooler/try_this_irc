#include "../headers/Server.hpp"

// ::( void ){ return; }

// ::~( void ){ return; }

// ::(  const & ){ return; }

//  & ::operator=(  const & cp )
// {
// 	this->_line = cp._line;
// 	this->_all_commands = cp._all_commands;
// 	this->Identities = cp.Identities;
// 	this->Usernames = cp.Usernames;
// 	this->Nicks = cp.Nicks;
// 	this->Channels = cp.Channels;
// 	this->full_command = cp.full_command;
// 	this->pass = cp.pass;
// 	this->word = cp.word;
// 	this->_IP_tmp = cp._IP_tmp;
// 	this->User = cp.User;
// 	this->Channel_Count = cp.Channel_Count;
// 	for (int i = 0; i <= this->User; i++)
// 	{
// 		this->nick_already_set[i] = cp.nick_already_set[i];
// 		this->_authentified[i] = cp._authentified[i];
// 		this->tmp_authentified[i] = cp.authentified[i];
// 	}
// 	return *this;
// }

void Server::Init_Values( int argc, char **argv )
{
	if ( argc != 3 )
		std::cerr << "Usage: ./ircserv <port> <password> \n" << std::endl;
	this->_port = atoi(Check_Port(argv[1]));
//	check_password(argv[2]);
	this->_pass = argv[2];

	std::string tmp = "PASS"; // all_commands[0]
	this->_all_commands.push_back(tmp);
	tmp.clear();

	tmp = "NICK"; // all_commands[1]
	this->_all_commands.push_back(tmp);
	tmp.clear();

	tmp = "USER"; // all_commands[2]
	this->_all_commands.push_back(tmp);
	tmp.clear();

	tmp = "JOIN"; // all_commands[3]
	this->_all_commands.push_back(tmp);
	tmp.clear();

	tmp = "PRIVMSG"; // _all_commands[4]
	this->_all_commands.push_back(tmp);
	tmp.clear();

	this->_full_command["PASS"].push_back("PASS");
	this->_full_command["PASS"].push_back("arguement");

	this->_full_command["NICK"].push_back("NICK");
	this->_full_command["NICK"].push_back("arguement");

	this->_full_command["USER"].push_back("USER");
	this->_full_command["USER"].push_back("username");
	this->_full_command["USER"].push_back("hostname");
	this->_full_command["USER"].push_back("servername");
	this->_full_command["USER"].push_back("realname");

	this->_full_command["JOIN"].push_back("JOIN");
	this->_full_command["JOIN"].push_back("arguement"); // param A
	this->_full_command["JOIN"].push_back("arguement"); // param B
//	Le param A est le nom du chan
//	Le param B est le mot de pass (key) du chan [optionnel]
//	JOIN peut posséder des params A et B multiples, le minimum est 1 param

	this->_full_command["PRIVMSG"].push_back("PRIVMSG");
	this->_full_command["PRIVMSG"].push_back("arguement");
	this->_full_command["PRIVMSG"].push_back("arguement");

	this->_User = 0;
	this->_User_Count = 0;
	this->_Channel_Count = 0;
	this->_nick_already_set[this->_User] = 0;
	this->_authentified[this->_User] = 0;
	this->_tmp_authentified[this->_User] = 0;
}

char	*Server::Check_Port( char *port )
{
// est-ce qu'on rajoute une limite de la securite sur les args donnés ?
// on pourrait rajouter le nombre de caracteres, une limite au port

	int     i;

	if (!this->Is_Number(port))
		std::cerr << "Usage: <port> must be numeric" << std::cout;
	i = atoi( port );
	if ( i < 0 || i > 65535 )
		std::cerr << "Usage: <port> can't be 0, negative or more than 65535" << std::cout;
// peut être faire une fonction qui vérifie si les ports sont utilisés ou pas
	return (port);	
}

bool Server::Is_Number(const std::string s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

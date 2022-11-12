#include "../headers/headers.hpp" 
#include "../headers/parse_message.hpp"
#include "../headers/Server.hpp" 

using namespace std;

Server::Server() {}
// Server::Server(Stock *test){ (void)test;}
Server::~Server() {}

void Server::errorMessage( User user, int numReply, std::string arg)
{
	std::map<int, std::string> err;
	std::ostringstream s;

    if (*(arg.end() - 1) == '\n')
        arg = arg.substr(0, arg.length() - 1);
    s << ":mfirc " << numReply << " * " << arg << err[numReply] << "\r\n";

    std::string msg = s.str();

    if ( send(user.getFd(), &msg[0], msg.size(), 0) == -1 )
        throw errorExcute(strerror(errno));
}

void sigchld_handler( /*int s*/ )
{
	// waitpid() might overwrite errno, so we save and restore it:
	int saved_errno = errno;

	//std::cout << s << std::endl;
	while(waitpid(-1, NULL, WNOHANG) > 0);

	errno = saved_errno;
}

// get sockaddr, IPv4 or IPv6:
void *Server::get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
	return &(((struct sockaddr_in*)sa)->sin_addr);

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void Server::Check_ID(char *s)
{
// a arranger !
	cout << "Passse Check ID" << endl;
	if (_Identities.empty() && _User == 0)
		_IP_tmp = s;

/*	if (User == 0 && tmp_authentified == 2
	&& Identities[User][1] != Nicks[User] &&
	Identities[User][0] == s) */

// Je tente de trouver une maniere de reperer qu/'un autre User ayant la meme adresse IP se connecte mais c/'est chaud, peut etre juste ignorer
		
	else if (_Identities[_User][0].compare(s) != 0)
	{
		for (int i = 0; i < _User_Count; i++)
		{
			if (_Identities[i][0].compare(s) == 0 &&
			_Identities[i][1] == _Nicks[_User]/* &&
			Identities[i][2] == Usernames[User][0]*/)
// Attendre User.cpp avant d'utiliser cette condition
			{
				_User = i;
				break;
			}
			else if (_Identities[i + 1][0].empty() != 0)
			{
				_User = i;
				_IP_tmp.clear();
				_IP_tmp = s;
			}
		}
	}
	cout << "Sortie Check ID" << endl;
}

void Server::message_error(std::string const &str)
{
    std::cerr << "Function failed: " << str << std::endl;
    exit(EXIT_FAILURE);
}

void	Server::step_zero(char *port, char *pwd)
{
	cout << "Entree Step [0]" << endl;
	this->_portStr = port;
    this->_port = atoi(port);
    this->_pass = pwd;

	//std::cout << "before step1"<< std::endl;
	step_one_preparation();
	//std::cout << "after step1"<< std::endl;
	step_two_ListeningOnSocket();
	for(;;)
    {  
        step_three_select();
        // run through the existing connections looking for data to read
        for (_i = 0; _i <= _fdmax; _i++)
        {
			//std::cout << "before FD_isSET step_four"<< std::endl;
            if (FD_ISSET(_i, &_read_fds))
            {	//std::cout << "before _1 = _sockfd: step_four"<< std::endl;
                if (_i == _sockfd)
				{  //when we are on current socket
					//std::cout << "before step_four"<< std::endl;
                	step_four_acceptNewConnection();
					//std::cout << "after step_four"<< std::endl;
				}
                else
                {
					// handle data from a client
                    _nbytesReceived = recv(_i, _buf, sizeof _buf, 0);
                    //_buf[_nbytesReceived] = '\0';
					cout << "Passe ELSE" << endl;
                    cout << "Before [4] buf: " << _buf << "\nnBytes: "<< _nbytesReceived << endl;
                    if (_nbytesReceived <= 0)
                        step_five_removeInactiveUser();
                  	else 
				  	{
                        _buf[_nbytesReceived] = '\0';
						//to do verifier authenication of User
                        cout << "Passe ICI" << endl;
						step_six_sendMsgToOthers();
						
					}
				}
            }
        }

    }
    return ;
}

void	Server::step_one_preparation()
{
	int yes=1;
cout << "Entree Step [1]" << endl;
	std::cout << "Le pass est censé être : " << _pass << std::endl;
	memset(&_hints, 0, sizeof _hints);
	_hints.ai_family = AF_UNSPEC;
	_hints.ai_socktype = SOCK_STREAM;
	_hints.ai_flags = AI_PASSIVE; // use my IP
	if ((getaddrinfo(NULL, _portStr , &_hints, &_servinfo)) != 0) 
        message_error("getaddrinfo");
	// loop through all the results and bind to the first we can
	for(_p = _servinfo; _p != NULL; _p = _p->ai_next)
	{
		if ((_sockfd = socket(_p->ai_family, _p->ai_socktype, _p->ai_protocol)) == -1) 
		{
			//message_error("server: socket");
			continue;
		}

		if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) 
		{
			(void)_sockfd;
			//message_error("setsockopt");
		}
		if (bind(_sockfd, _p->ai_addr, _p->ai_addrlen) == -1)
		{
			close(_sockfd);
			//message_error("server: bind");
			continue;
		}
        	break;
    }
	freeaddrinfo(_servinfo); // all done with this structure	
}

void    Server::step_two_ListeningOnSocket()
{
	cout << "Entree Step [2]" << endl;
	_fdmax = _sockfd; 
	std::cout << "Principal FD _serverSocket: [" << _sockfd << "]" << std::endl;
	if (_p == NULL) 
	{
        message_error("selectserver: failed to bind");
	}
	if (listen(_sockfd, BACKLOG) == -1)
	{
		message_error("listen");
	}

	//_sa.sa_handler = sigchld_handler(); // reap all dead processes
	sigemptyset(&_sa.sa_mask);
	_sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &_sa, NULL) == -1)
	{
		message_error("sigaction");
	}
	printf("server: waiting for connections...\n");
	memset(&_popoll, 0, sizeof _popoll);
	_popoll.events = POLLIN;

	 FD_ZERO(&_master);    // clear the master and temp sets
     FD_ZERO(&_read_fds);
     // add the listener to the master set
     FD_SET(_sockfd, &_master);
}

void   Server::step_three_select()
{
	cout << "Entree Step [3]" << endl;
    _read_fds = _master; // copy it
	int sel;
	sel = select(_fdmax+1, &_read_fds, NULL, NULL, NULL);
	cout << "Entree Step [3 *] sel: " << sel << endl;
	if (sel == -1) {
        message_error("select");
    }
	cout << "Entree Step [3 **]" << endl;
}

void Server::step_four_acceptNewConnection( )
{
	cout << "Entree Step [4]" << endl;
	_sin_size = sizeof _their_addr;
	// new_fd[User] = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
	// if (new_fd[User] == -1) 
	// {
	// 	perror("accept");
	// }
	_nFd = accept(_sockfd, (struct sockaddr *)&_their_addr, &_sin_size);
	if (_nFd == -1) 
	{
		perror("accept");
		return ;
	}

        FD_SET( _nFd, &_master); // add to master set
        if (_nFd > _fdmax) 
        {    // keep track of the max
            _fdmax = _nFd ;
        }
        printf("selectserver: new connection from %s on ""socket %d\n",
		inet_ntop(_their_addr.ss_family,
			get_in_addr((struct sockaddr*)&_their_addr),
                _remoteIP, INET6_ADDRSTRLEN), _nFd);
	
	//Check_ID(_remoteIP);
}

void Server::step_five_removeInactiveUser()
{
	cout << "Entree Step [5]" << endl;
    if (_nbytesReceived == 0)
    {
        // connection closed
        printf("selectserver: socket %d hung up\n", _i);
    } 
    else 
    {
        message_error("recv");
    }
    close(_i); // bye!    
    FD_CLR(_i, &_master); // remove from master set
}

int Server::step_six_sendMsgToOthers( )
{
	cout << "Entree Step [6]" << endl;
	//_popoll.fd = _new_fd[User];
	std::cout << "ETAPE 6   BUF: " << _buf /*Server: Bienvenue, veuillez taper votre pass.)"*/ << std::endl;


	// if (poll(&_popoll, 1, 10000000) == 1)
	// {
	// 	cout << " 6 -> POLL" <<endl;
	// 	try
	// 	{
	// 		receive_message(_newfd[_User]);
	// 	}
	// 	catch (const std::exception & e)
	// 	{
	// 		std::cerr << _remoteIP << e.what() << std::endl;
	// 		return (464);
	// 	}
	// }
	// 	close(_newfd[_User]);
	// 	close(_popoll.fd);
		return (0);
}

int Server::server(char *port, char *pwd)
{
	step_zero(port, pwd);
	return (0);
}

// int Server::server( *)
// {
	//int yes=1;
	//int rv;
	//int sockfd;
	//struct addrinfo hintsservinfop;
	//struct pollfd popoll;
	//struct sigaction sa;

	//nt new_fd[BACKLOG];  // listen on sock_fd, new connection on new_fd
	// struct sockaddr_storage their_addr; // connector's address information
	// socklen_t sin_size;
	//char s[INET6_ADDRSTRLEN];
	

/*
	std::cout << "Le pass est censé être : " << pass << std::endl;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
			p->ai_protocol)) == -1) 
		{
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
		sizeof(int)) == -1) 
		{
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sockfd);
			perror("server: bind");
			continue;
		}

        	break;
    	}

	freeaddrinfo(servinfo); // all done with this structure
*/
	// if (p == NULL)
	// {
	// 	fprintf(stderr, "server: failed to bind\n");
	// 	exit(1);
	// }

	// if (listen(sockfd, BACKLOG) == -1)
	// {
	// 	perror("listen");
	// 	exit(1);
	// }

	// sa.sa_handler = sigchld_handler; // reap all dead processes
	// sigemptyset(&sa.sa_mask);
	// sa.sa_flags = SA_RESTART;
	// if (sigaction(SIGCHLD, &sa, NULL) == -1)
	// {
	// 	perror("sigaction");
	// 	exit(1);
	// }

	// printf("server: waiting for connections...\n");

	// memset(&popoll, 0, sizeof popoll);
	// popoll.events = POLLIN;
//	while(1)
	//{   main accept() loop
		// sin_size = sizeof their_addr;
		// new_fd[User] = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		// if (new_fd[User] == -1) 
		// {
		// 	perror("accept");
		// 	continue;
		// }

		// inet_ntop(their_addr.ss_family,
		// get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
		// printf("server: got connection from %s\n", s);
		// Check_ID(, s);
		// popoll.fd = new_fd[User]; 
		// probleme : le test du pdf ne marche pas ! 
		// std::cout << "(Bienvenue sur le serveur, veuillez taper votre pass.)" \
		// << std::endl;
		// if (poll(&popoll, 1, 10000000) == 1)
		// {
		// 	try
		// 	{
		// 		receive_message(new_fd[User], );
		// 	}
		// 	catch (const std::exception & e)
		// 	{
		// 		std::cerr << s << e.what() << std::endl;
		// 		return (464);
		// 	}
		// }
//		close(new_fd[User]);
// 		close(popoll.fd);
// 	}
// 	return 0;
// }


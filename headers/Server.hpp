#ifndef SERVER_HPP
# define SERVER_HPP

#include "User.hpp"
#include "Command.hpp"
#include "error.hpp"
#include <map>

#define BACKLOG 10   // how many pending connections queue will hold

using namespace std;
class User;

class Server
{
    private:

    std::map<int, User *> _listUser;
    std::map<int, User*>::iterator _it;  
    std::map<int, User*>::iterator _ite; // _ite = _listUser.end();



	std::vector<std::string> _line;
	std::vector<std::string> _all_commands;
	std::map<int, std::vector<std::string> > _Channels;
	// Channels[i][0] = Nom du channel
	// Channels[i][1] = key
	// Channels[i][2] = topic
	
	std::map<std::string, std::vector<std::string> > _Channels_Users;
	// A coupler avec nom de channel en 1er argument 
	
	std::map<int, std::vector<std::string> > _Identities; 
	// Identities[User][0] = IP
	// Identities[User][1] = Nick
	// Identities[User][2] = Username

	std::vector<std::string> _Nicks; // a coupler avec User
	std::map<int, std::vector<std::string> > _Usernames; // a coupler avec User
	std::map<std::string, std::vector<std::string> > _full_command;
	std::string _pass;
	std::string _word;
	std::string _IP_tmp;
	int	_nick_already_set[100];
	int	_tmp_authentified[100];
	int	_authentified[100];
	int	_User;
	int	_Channel_Count; // Channel_Count commencera par 1 pour dire qu'1 	channel a été crée, nous dit comebien de channels sont crées
	int	_User_Count; // Pareil qu'au dessus

    std::string _msg;
    std::string _error;

/////////init socket infos
	std::string			    _portString;
    struct sigaction        _sa;
	struct addrinfo		    _hints;
	struct addrinfo		    *_servinfo;
	struct addrinfo		    *_p;
    struct pollfd           _popoll;
	int					    _sockfd;
    int                     _rv;
    char                    *_portStr;
    int                     _port;
    char                    _buf[256];  // buffer for client data
    int		                _nbytesReceived;
    int                     _nFd;

///////// The Select needs for Fd Mgmt
	fd_set		_master;		// master file descriptor list
	fd_set		_read_fds;	// temp file descriptor list for select()

///////// keep track of the biggest file descriptor
	int		_fdmax;			// maximum file descriptor number
	//int		_newfd;		// newly accept()ed socket descriptor
    //int     _newfd[BACKLOG];
	int		_i, _j;			// Fd Mgmt

///////// Addressage Clients
    //char                       _s[INET6_ADDRSTRLEN];
	char						_remoteIP[INET6_ADDRSTRLEN];
	//struct sockaddr_storage 	_remoteaddr; // client address
    struct sockaddr_storage     _their_addr; 
    socklen_t                   _sin_size;


/////// map regroupant les user present sur le server, avec first-> int fd du user et second-> un pointeur vers le user
    std::map<int, User*> _user_list;
    std::map<int, User*>::iterator _Uit; //it_user _uit = _user_list.begin();
    std::map<int, User*>::iterator _Uite; //it_end = _Uit = _user_end();
    
    public:

    Server();
    Server( *test);
    ~Server();
	char *Check_Port( char *port );
	bool Is_Number( const std::string s );


    int	command_check(int poll_fd);
    int	receive_message(int poll_fd);
    int server(char *port, char *pwd);

    void message_error(std::string const &str);

    void step_zero(char *port, char *pwd);
    void step_one_preparation();
    void step_two_ListeningOnSocket();
    void step_three_select();
    void step_four_acceptNewConnection();
    void step_five_removeInactiveUser();
    int  step_six_sendMsgToOthers();
    void    Check_ID( char *s);

//////////   Commandes
    int		PASS(int poll_fd);
    int		NICK(int poll_fd);
    int		JOIN(int poll_fd);
    int		USER(int poll_fd);
    int		PRIVMSG(int poll_fd);

    void    Init_Values( int argc, char **argv );
    int     server();
    void    sigchld_handler( void );
    void    *get_in_addr(struct sockaddr *sa);
    void    errorMessage( User user, int numReply, std::string arg);
    std::string	ft_addback(std::vector<std::string> str, std::string add, int begin=0);

    bool    fd_registered(int fd);
    User    *getUserbyfd(int fd);
    void    ft_finish_registration(int fd);
 };

 class errorExcute : public std::exception {

    private:
        const char * _strerror;

    public:
        errorExcute(const char * str) : _strerror(str) {};
        const char * what() const throw() {return _strerror; };
};
#endif

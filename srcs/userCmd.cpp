/*
    https://www.techbull.com/techbull/guide/internet/irccommande.html#users
	http://www.iprelax.fr/irc/irc_rfcus4.php

   Command: USER
   Parameters: <username> <hostname> <servername> <realname>

   The USER message is used at the beginning of connection to specify
   the username, hostname, servername and realname of s new user.  It is
   also used in communication between servers to indicate new user
   arriving on IRC, since only after both USER and NICK have been
   received from a client does a user become registered.

   Between servers USER must to be prefixed with client's NICKname.
   Note that hostname and servername are normally ignored by the IRC
   server when the USER command comes from a directly connected client
   (for security reasons), but they are used in server to server
   communication.  This means that a NICK must always be sent to a
   remote server when a new user is being introduced to the rest of the
   network before the accompanying USER is sent.

   It must be noted that realname parameter must be the last parameter,
   because it may contain space characters and must be prefixed with a
   colon (':') to make sure this is recognised as such.

   Since it is easy for a client to lie about its username by relying
   solely on the USER message, the use of an "Identity Server" is
   recommended.  If the host which a user connects from has such a
   server enabled the username is set to that as in the reply from the
   "Identity Server".

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED

   Examples:

   USER guest tolmoon tolsun :Ronnie Reagan

                                   ; User registering themselves with a
                                   username of "guest" and real name
                                   "Ronnie Reagan".

   :testnick USER guest tolmoon tolsun :Ronnie Reagan
                                   ; message between servers with the
                                   nickname for which the USER command
                                   belongs to
*/

#include "../headers/User.hpp"
#include "../headers/Server.hpp"
// #include "../headers/Command.hpp"
// #include "../headers/User.hpp"
// #include "../headers/tools.hpp"
// #include "../headers/Server.hpp"

// bool    check_password( User &user,  *)
// {
//     if (user.getPasswd() == pass )
//         return true;

//     std::string         error;
//     std::ostringstream   s;

//     if (user.getPasswd() == "")
//         error = "Log in Server needs password ";
//     else
//         error = "Wrong password";
//     s << "Error: "<< user.getUsername() << "@" << getHost() << "[" << error << "]\r\n";

//     return false;
// }

User::User( void ) 
{  
	//this->_fd = -1;
	this->_nick = "";
	this->_passwd = "";
	this->_userName = "";
	this->_hostName = "";
	this->_serverName = "";
	this->_realName = "";
	this->_isSet = false;
	// this->_mode = "";
	// this->_ping_status = false;
}

User::User( int fd )
{
	this->_fd = fd;
 	this->_nick = "";
 	this->_passwd = "";
 	this->_userName = "";
 	this->_hostName = "127.0.0.1";
 	this->_serverName = "";
 	this->_realName = "";
 	this->_isSet = false;
 	//this->_isaccepted = false;
 	this->_mode = "";
 	this->_ping_status = false;
 }

// User::User( int fd, std::string nick, std::string userName, std::string hostName,
// 	std::string serverName, std::string realName, std::string mode, bool ping_status )
// {
// 	this->_fd = fd;
// 	this->_nick = nick;
// 	this->_userName = userName;
// 	this->_hostName = hostName;
// 	this->_serverName = serverName;
// 	this->_realName = realName;
// 	this->_isSet = false;
// 	this->_mode = mode;
// 	this->_ping_status = ping_status;
// }

User::User( User const &src ) { *this = src; }
User::~User( void ) {}
User	&User::operator=( User const &rhs )
{
	//_fd = rhs._fd;
	_nick = rhs._nick;
	_passwd = rhs._passwd;
	_userName = rhs._userName;
	_hostName = rhs._hostName;
	_serverName = rhs._serverName;
	_realName = rhs._realName;
	_mode = rhs._mode;
	_ping_status = rhs._ping_status;
	_isSet = rhs._isSet;
	return (*this);
}


//**********************************************//
//*                 getters                    *//
//**********************************************//

int	const			    &User::getFd( void ) const { return _fd; }
bool const			    &User::getIsSet( void ) const { return _isSet; }
//bool const				&User::isConnected(void) { return _connected; }
// bool const				&User::getPingStatus( void ) const { return _ping_status; }
std::string const 		&User::getPasswd( void ) const { return _passwd; }
std::string const		&User::getUsername( void ) const { return _userName; }
std::string const		&User::getHostname( void ) const { return _hostName; }
std::string const		&User::getServername( void ) const { return _serverName; }
std::string const		&User::getRealName( void ) const { return _realName; }
std::string const	    &User::getNick() const { return _nick; }




//**********************************************//
//*                 setters                    *//
//**********************************************//

//void	User::setFd( int fd) {_fd = fd; }
void	User::setNick( std::string nick ) { _nick = nick; }
void 	User::setUsername( std::string username ) { _userName = username;}
void	User::setHostname( std::string hostname ) { _hostName = hostname;}
void	User::setServername( std::string servername ) { _serverName = servername;}
void	User::setRealname( std::string realname ) { _realName = realname; }
void	User::setIsSet( bool isset ) { _isSet = isset;}



int Server::USER(int poll_fd)
{
    User user;

    if (_line.size() < 4)
    {
        errorMessage(poll_fd, ERR_NEEDMOREPARAMS, ": Need more parameters ");
        return (1);
    }
    if (user.getIsSet())
    {
        errorMessage(poll_fd, ERR_ALREADYREGISTRED, user.getNick());
        return (1);
    }
    
    user.setUsername(_line[1]);
	user.setHostname(_line[2]);
	user.setServername(_line[3]);
    //user.setRealname(ft_addback(_all_commands, " ", 3));
	user.setIsSet(true);
    return (0);
}



/*
void    USER( *, User &user)
//void USER(Server &server, User &user, std::vector<std::string> arg)
{
    if (_line.size() < 4)
    {
        errorMessage(user, ERR_NEEDMOREPARAMS, ":Need more parameters");
        return ;
    }
    if (user.getIsSet()){
		errorMessage(user, ERR_ALREADYREGISTRED, user.getNick());
		return ;
	}
    user.setUsername(_line[1]);
	user.setHostname(_line[2]);
	user.setServername(_line[3]);
    user.setRealname(ft_addback(all_commands, " ", 3));
	user.setIsSet(true);

	if (!user.getNick().empty())
	{
		if (pass != "")
			if (!check_password(user, ))
				return;
		
		std::cout <<  "User #" << user.getFd() << " registred as " << user.getNick() << std::endl;
		//messageoftheday(server, user);
	}
}
*/

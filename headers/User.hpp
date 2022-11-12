#ifndef USER_HPP
# define USER_HPP

# include "headers.hpp"
# include "error.hpp"
# include "tools.hpp"

//class ;

class User
{
    private:
        int                         _fd;
        bool		                _isSet;			// If USER command is been used
        std::string				    _userName;
		std::string					_hostName;
		std::string					_serverName;
		std::string					_realName;
        std::string				    _nick;
		std::string					_mode;
		std::string					_passwd;
		// time_t 					*_last_act;
		bool						_ping_status;
		

    public:

        User( void );
        User( int fd );
		User( int fd, std::string nick, std::string username, std::string hostname,
			std::string servername, std::string realname, std::string mode, bool ping_status );
		User( User const &src );
		~User( void );
        User				&operator=( User const &rhs );

        //getters
        int	const				    &getFd( void ) const;
        bool const				    &getIsSet( void ) const;
		std::string const			&getNick( void ) const;
        std::string const			&getUsername( void ) const;
		std::string const			&getHostname( void ) const;
		std::string const			&getServername( void ) const;
		std::string const			&getRealName( void ) const;
		std::string const			&getPasswd( void ) const;

        //setters

		//void			setFd( int fd );
		void			setPasswd( std::string passwd );
		void 			setUsername( std::string username );
		void			setHostname( std::string hostname );
		void			setServername( std::string servername );
		void			setRealname( std::string realname );
		void			setNick( std::string nick);
		void			setIsSet( bool isset );
};

#endif

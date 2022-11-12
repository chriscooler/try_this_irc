#ifndef ERROR_HPP
# define ERROR_HPP

// ERRORS

# define ERR_NOSUCHNICK			401
# define ERR_NOSUCHSERVER		402
# define ERR_NOSUCHCHANNEL		403
# define ERR_CANNOTSENDTOCHAN	404
# define ERR_TOOMANYCHANNELS	405
# define ERR_WASNOSUCHNICK		406 // "<nickname> :There was no such nickname"
# define ERR_TOOMANYTARGETS		407
# define ERR_NOORIGIN			409
# define ERR_NORECIPIENT		411
# define ERR_NOTEXTTOSEND		412
# define ERR_NOTOPLEVEL			413 // "<mask> :No toplevel domain specified"
# define ERR_WILDTOPLEVEL		414 // "<mask> :Wildcard in toplevel domain"
# define ERR_UNKNOWNCOMMAND		421 // "<command> :Unknown command"
# define ERR_NOMOTD				422
# define ERR_NOADMININFO		423 // "<server> :No administrative info available"
# define ERR_NONICKNAMEGIVEN	431
# define ERR_ERRONEUSNICKNAME	432
# define ERR_NICKNAMEINUSE		433
# define ERR_NOTONCHANNEL		442
# define ERR_USERONCHANNEL		443
# define ERR_NOTREGISTERED		451
# define ERR_NEEDMOREPARAMS		461
# define ERR_ALREADYREGISTRED	462
# define ERR_PASSWDMISMATCH		464
# define ERR_KEYSET				467
# define ERR_CHANNELISFULL		471
# define ERR_UNKNOWNMODE		472
# define ERR_INVITEONLYCHAN		473
# define ERR_BANNEDFROMCHAN		474
# define ERR_BADCHANNELKEY		475
# define ERR_BADCHANMASK		476
# define ERR_CHANOPRIVSNEEDED	482
# define ERR_NOOPERHOST			491
# define ERR_UMODEUNKNOWNFLAG	501
# define ERR_USERSDONTMATCH		502


// class errorExcute : public std::exception {

//     private:
//         const char * _strerror;

//     public:
//         errorExcute(const char * str) : _strerror(str) {};
//         const char * what() const throw() {return _strerror; };
// };

//class User;

//void	errorMessage( User user, int numReply, std::string arg);

#endif

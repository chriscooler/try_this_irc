#include "headers/headers.hpp"
#include "headers/error.hpp"
using namespace std;

//#define _port  "1100"   // port we're listening on

class Server
{
    private:
/////////init socket infos
	std::string			_portString;
	struct addrinfo		_hints;
	struct addrinfo		*_ai;
	struct addrinfo		*_p;
	int					_listener;
   // int                 _rv;
   char                 *_portStr;
    int                 _port;
    char                _buf[256];  // buffer for client data
    int		            _nbytesReceived;
    char                *_pwd;

///////// The Select needs for Fd Mgmt
	fd_set		_master;		// master file descriptor list
	fd_set		_read_fds;	// temp file descriptor list for select()

///////// keep track of the biggest file descriptor
	int		_fdmax;			// maximum file descriptor number
	int		_newfd;			// newly accept()ed socket descriptor
	int		_i, _j;			// Fd Mgmt

///////// Addressage Clients
	char						_remoteIP[INET6_ADDRSTRLEN];
	struct sockaddr_storage 	_remoteaddr; // client address
	socklen_t					_addrlen;

    public:

    void message_error(std::string const &str);

    void step_zero(char *port, char *pwd);
    void step_one_preparation();
    void step_two_ListeningOnSocket();
    void step_three_select();
    void step_four_acceptNewConnection();
    void step_five_removeInactiveUser();
    void step_six_sendMsgToOthers()  ;

};
//Server::~Server() {}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void    Server::step_zero(char *port, char *pwd)
{
    this->_portStr = port;
    this->_port = atoi(port);
    this->_pwd = pwd;
    cout << "nbyteR: " << _nbytesReceived << endl;
    _nbytesReceived = 0;

    step_one_preparation();
    step_two_ListeningOnSocket();
    for(;;)
    {  
        step_three_select();
        // run through the existing connections looking for data to read
        for (_i = 0; _i <= _fdmax; _i++)
        {
            if (FD_ISSET(_i, &_read_fds))
            {
                if (_i == _listener)  //when we are on current socket
                step_four_acceptNewConnection();
                else
                {// handle data from a client
                    _nbytesReceived = recv(_i, _buf, sizeof _buf, 0);
                    //_buf[_nbytesReceived] = '\0';
                    //cout << "Before [4] buf: " << _buf << "\nnBytes: "<< _nbytesReceived << endl;
                    if (_nbytesReceived <= 0)
                        step_five_removeInactiveUser();
                    else {
                        _buf[_nbytesReceived] = '\0';
                        step_six_sendMsgToOthers()  ;
                    }
                }
            }
        }

    }
    return ;
}
void Server::message_error(std::string const &str)
{
    std::cerr << "Function failed: " + str << std::endl;
    exit(EXIT_FAILURE);
}

void   Server::step_one_preparation()
{
    int yes=1;

    memset(&_hints, 0, sizeof _hints);
    _hints.ai_family = AF_UNSPEC;
    _hints.ai_socktype = SOCK_STREAM;
    _hints.ai_flags = AI_PASSIVE;
    if ((getaddrinfo(NULL, _portStr , &_hints, &_ai)) != 0) 
        message_error("getaddrinfo");
    for(_p = _ai; _p != NULL;_p = _p->ai_next) 
    {
        _listener = socket(_p->ai_family, _p->ai_socktype, _p->ai_protocol);
        if (_listener < 0) { 
            continue;
        }
        // lose the pesky "address already in use" error message
       
        setsockopt(_listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        if (bind(_listener, _p->ai_addr, _p->ai_addrlen) < 0) {
            close(_listener);
            continue;
        }
        break;
    }
    // if we got here, it means we didn't get bound
    if (_p == NULL) {
        message_error("selectserver: failed to bind");
    }
    freeaddrinfo(_ai); // all done with this
}

void    Server::step_two_ListeningOnSocket()
{
    _fdmax = _listener;  //listen this one;
    std::cout << "Principal FD _serverSocket: [" << _listener << "]" << std::endl;
    // listen
    if (listen(_listener, 10) == -1) {
        message_error("listen");
    }
    std::cout << "Listening on port " << _port << std::endl;
	std::cout << "Waiting for connections ..." << std::endl;

    FD_ZERO(&_master);    // clear the master and temp sets
    FD_ZERO(&_read_fds);
    // add the listener to the master set
    FD_SET(_listener, &_master);
}

void   Server::step_three_select()
{
    _read_fds = _master; // copy it
    if (select(_fdmax+1, &_read_fds, NULL, NULL, NULL) == -1) {
        message_error("select");
    }
}

void Server::step_four_acceptNewConnection()
{
    std::cout << "Run connection[4] --> _buf: " << _buf << std::endl;
                    // handle new connections
    _addrlen = sizeof _remoteaddr;
    _newfd = accept(_listener, (struct sockaddr *)&_remoteaddr, &_addrlen);
    if (_newfd == -1)
    {
        perror("accept");
    }
    else 
    {
        FD_SET(_newfd, &_master); // add to master set
        if (_newfd > _fdmax) 
        {    // keep track of the max
            _fdmax = _newfd;
        }
        printf("selectserver: new connection from %s on ""socket %d\n",
        inet_ntop(_remoteaddr.ss_family,
            get_in_addr((struct sockaddr*)&_remoteaddr),
                _remoteIP, INET6_ADDRSTRLEN),_newfd);

         //creer User
         User *usr = new User();

    }
   

}

void Server::step_five_removeInactiveUser()
{
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

void Server::step_six_sendMsgToOthers()    //faut modifier avec les geustions a envoyer uniquement l'user de channel
{
    cout << "Nbr Bytes received: "<< _nbytesReceived <<endl;
    cout << "sizeof _buf: " << sizeof _buf << endl;
    cout << "_buf: " << _buf << "***"<< endl;
    //_buf[_nbytesReceived] = '\0';
    // we got some data from a client
    for(_j = 0; _j <= _fdmax; _j++) 
    {
        // send to everyone!
        if (FD_ISSET(_j, &_master)) 
        {
            // except the listener and ourselves
            if (_j != _listener && _j != _i) 
            {
/**test**/
std::cout << "fd_who_send_message [" << _i << "]" << std::endl;
std::cout << "message is [" << _buf << "]" << std::endl;
std::cout << "Which fd also get msg[" << _j << "]" << std::endl;
/**test**/
std::cout << "_j= " << _j << std::endl;
                if (send(_j, _buf, _nbytesReceived, 0) == -1) 
                {
                    perror("step 6 send");
                }
            }
        }
    // END handle data from client
    // END got new incoming connection
    // END looping through file descriptors
    // END for(;;)--and you thought it would never end!
    }
    return ;
}

int main(int ac, char **av)
{
    Server server;

    if (ac == 3)
    {
        server.step_zero(av[1], av[2]);
    }
    return (0);
}

/*
int main(void)
{
    fd_set master;    // master file descriptor list
    fd_set read_fds;  // temp file descriptor list for select()
    int fdmax;        // maximum file descriptor number

    int listener;     // listening socket descriptor
    int newfd;        // newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // client address
    socklen_t addrlen;

    char buf[256];    // buffer for client data
    int nbytes;

    char remoteIP[INET6_ADDRSTRLEN];

    int yes=1;        // for setsockopt() SO_REUSEADDR, below
    int i, j, rv;

    struct addrinfo hintsaip;

    FD_ZERO(&master);    // clear the master and temp sets
    FD_ZERO(&read_fds);

    // get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, _port , &hints, &ai)) != 0) {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }
    
    for(p = ai; p != NULL; p = p->ai_next) 
    {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) { 
            continue;
        }
        // lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }
        break;
    }

    // if we got here, it means we didn't get bound
    if (p == NULL) {
        fprintf(stderr, "selectserver: failed to bind\n");
        exit(2);
    }

    freeaddrinfo(ai); // all done with this

    // listen
    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(3);
    }

    // add the listener to the master set
    FD_SET(listener, &master);

    // keep track of the biggest file descriptor
    fdmax = listener; // so far, it's this one

    // main loop
    for(;;) {
        read_fds = master; // copy it
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }
        // run through the existing connections looking for data to read
        for(i = 0; i <= fdmax; i++) 
        {
            if (FD_ISSET(i, &read_fds)) { // we got one!!
                if (i == listener) {
                    // handle new connections
                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);
                    if (newfd == -1) {
                        perror("accept");
                    }
                    else 
                    {
                        FD_SET(newfd, &master); // add to master set
                        if (newfd > fdmax) {    // keep track of the max
                            fdmax = newfd;
                        }
                        printf("selectserver: new connection from %s on "
                            "socket %d\n",
                            inet_ntop(remoteaddr.ss_family,
                                get_in_addr((struct sockaddr*)&remoteaddr),
                                remoteIP, INET6_ADDRSTRLEN),
                            newfd);
                    }
                } 
                else {
                    // handle data from a client
                    if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
                        // got error or connection closed by client
                        if (nbytes == 0) {
                            // connection closed
                            printf("selectserver: socket %d hung up\n", i);
                        } else {
                            perror("recv");
                        }
                        close(i); // bye!
                        FD_CLR(i, &master); // remove from master set
                    } 
                    else {
                        // we got some data from a client
                        for(j = 0; j <= fdmax; j++) {
                            // send to everyone!
                            if (FD_ISSET(j, &master)) {
                                // except the listener and ourselves
                                if (j != listener && j != i) {
std::cout << "fd_who_send_message [" << i << "]" << std::endl;
std::cout << "message is [" << buf << "]" << std::endl;
std::cout << "Which fd also get msg[" << j << "]" << std::endl;
            

                                    if (send(j, buf, nbytes, 0) == -1) {
                                        perror("send");
                                    }
                                }
                            }
                        }
                    }
                } // END handle data from client
            } // END got new incoming connection
        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!
    
    return 0;
}*/

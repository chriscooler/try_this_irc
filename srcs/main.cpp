#include "../headers/headers.hpp"
#include <cstdlib>
#include "../headers/Server.hpp"

/*void	check_password( std::string pass_tmp ) -- Secu a ajouter 
{

}*/

int main(int argc, char **argv)
{

// On verfie qu'on ait assez d'arguments, que l'argv1, le port, n'est pas de 
// lettre, puis on les stocks 
	Server *ser = new Server();
	try{
		ser->Init_Values(argc, argv);
	}
	catch (const std::exception & e)
	{
		std::cerr << "exception found: " << e.what() << std::endl;
		return ( 1 );
	}
	//std::cout << "Port = " << .port << " && Pass = " << .pass << std::endl;
	ser->server(argv[1], argv[2]);
	delete ser;
	return (0);
}

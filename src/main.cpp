#include <MainClass.hpp>

using namespace std;

int	main(void){
	MainClass	mainClass;

	try {
		mainClass.run();
	} catch(const exception& e){
		cerr << e.what() << '\n';
		return (1);
	}
	return (0);
}

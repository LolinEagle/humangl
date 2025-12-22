#include <MainClass.hpp>

using namespace std;

int	main(int ac, char **av){
	if (ac < 3){
		cout << "Usage: ./humangl <color> <texture>\n\n" <<
				"Color\n" <<
				"1: color\n" <<
				"2: gray\n" <<
				"3: white\n\n" <<
				"Texture\n" <<
				"1: LolinEagle\n" <<
				"2: Stone\n" <<
				"3: Wood texture" << endl;
		return (0);
	}

	int color, texture;

	// Color
	if (strlen(av[1]) != 1 || av[1][0] < '1' || av[1][0] > '3'){
		cout << '\"' << av[1] << "\" is not a valid color" << endl;
		return (0);
	} else {
		color = av[1][0] - '1';
	}

	// Texture
	if (strlen(av[2]) != 1 || av[2][0] < '1' || av[2][0] > '3'){
		cout << '\"' << av[2] << "\" is not a valid texture" << endl;
		return (0);
	} else {
		texture = av[2][0] - '1';
	}

	MainClass	mainClass(color, texture);

	try {
		mainClass.run();
	} catch(const exception& e){
		cerr << e.what() << '\n';
		return (1);
	}
	return (0);
}

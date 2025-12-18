#include <MainClass.hpp>

using namespace std;

int main(int ac, char **av) {
	if (ac < 2) {
		cout << "Usage: ./humangl <scene> <color> <texture>\n\n"
			 << "Scene\n"
			 << "1: 42\n"
			 << "2: Cube\n"
			 << "3: Teapot\n"
			 << "4: Teapot2\n"
			 << "5: All objects\n\n"
			 << "Color\n"
			 << "1: color(default)\n"
			 << "2: gray\n"
			 << "3: white\n\n"
			 << "Texture\n"
			 << "1: LolinEagle (default)\n"
			 << "2: Stone\n"
			 << "3: Wood texture" << endl;
		return (0);
	}
	if (strlen(av[1]) != 1 || av[1][0] < '1' || av[1][0] > '5') {
		cout << '\"' << av[1] << "\" is not a valid scene" << endl;
		return (0);
	}

	int scene, color, texture;

	scene = av[1][0] - '1';
	if (ac < 3) {
		color = 0;
	} else if (strlen(av[2]) != 1 || av[2][0] < '1' || av[2][0] > '3') {
		color = 0;
		cout << '\"' << av[2] << "\" is not a valid color" << endl;
	} else {
		color = av[2][0] - '1';
	}
	if (ac < 4) {
		texture = 0;
	} else if (strlen(av[3]) != 1 || av[3][0] < '1' || av[3][0] > '3') {
		texture = 0;
		cout << '\"' << av[3] << "\" is not a valid texture" << endl;
	} else {
		texture = av[3][0] - '1';
	}

	MainClass mainClass(scene, color, texture);

	try {
		mainClass.run();
	} catch (const exception &e) {
		cerr << e.what() << '\n';
		return (1);
	}
	return (0);
}

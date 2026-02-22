#include <MainClass.hpp>

using namespace std;

bool	isNotValid(string av, char min, char max){
	return (av.length() != 1 || av[0] < min || av[0] > max);
}

void	ErrorMsg(string av, string str){
	cout << '\"' << av << "\" is not a valid " << str << endl;
}

int		main(int ac, char **av){
	// Usage
	if (ac < 2){
		cout << "Usage: ./humangl <scene> [color] [texture]\n\n" <<
				"Scene\n1: HumanGL\n2: Scop\n\n" <<
				"Color\n1: Color (default)\n2: Gray\n3: White\n\n" <<
				"Texture\n1: LolinEagle (default)\n2: Stone\n3: Wood" << endl;
		return (0);
	}
	
	// Scene
	if (isNotValid(av[1], '1', '2')){
		ErrorMsg(av[1], "scene");
		return (0);
	}

	int scene = av[1][0] - '1', color = 0, texture = 0;

	// Color
	if (ac > 2){
		if (isNotValid(av[2], '1', '3')) ErrorMsg(av[1], "color");
		else color = av[2][0] - '1';
	}

	// Texture
	if (ac > 3){
		if (isNotValid(av[3], '1', '3')) ErrorMsg(av[1], "texture");
		else texture = av[3][0] - '1';
	}

	MainClass	mainClass(scene, color, texture);

	try {
		mainClass.run();
	} catch(const exception& e){
		cerr << e.what() << '\n';
		return (1);
	}
	return (0);
}

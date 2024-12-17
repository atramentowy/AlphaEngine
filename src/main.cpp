#include "application.h"

int main() {
	try {
		Application app;
		app.Init();
		app.Run();
	} catch (const std::exception& e) {
		std::cerr<<"App error: "<<e.what()<<std::endl;
		return -1;
	}
	return 0;
}
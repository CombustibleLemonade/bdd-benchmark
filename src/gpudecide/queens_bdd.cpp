#include "../queens.cpp"

#include "adapter.h"

#include <iostream>

int
main(int argc, char** argv)
{
	gpudecide::bdd bdd(10);
	
	// return 0;
	return run_queens<gpudecide_bdd_adapter>(argc, argv);
}

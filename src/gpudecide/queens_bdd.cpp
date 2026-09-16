#include "../queens.cpp"

#include "adapter.h"

#include <iostream>

int
main(int argc, char** argv)
{
	return run_queens<gpudecide_bdd_adapter>(argc, argv);
}

#include <time.h>
#include <iostream>
#include "Simulator.h"

using namespace std;

void main()
{
	Restaurant restuarant;

	restuarant.PrepareOpen();
	restuarant.Open();
	restuarant.Close();

	return;
}
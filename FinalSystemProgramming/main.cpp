#include <time.h>
#include <iostream>
#include "Restaurant.h"
#include <time.h>

using namespace std;

void main()
{
	Restaurant restuarant;

	restuarant.PrepareOpen();
	restuarant.Open();
	restuarant.Close();

	return;
}
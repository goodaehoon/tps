#include "Precompiled.h"
#include "SamdaNet.h"
#include "Dispatcher.h"

int main(int argc, char* argv[])
{
	Dispatcher disp;

	SamdaNet netservice;
	if (netservice.open("127.0.0.1", 9999, &disp) == false)
		return 1;

	printf("server start!\n");

	getchar();

	netservice.stop();

	return 0;
}


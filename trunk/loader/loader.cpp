#include "launchClient.h"

#include <stdio.h>
int main(int argc, char** argv) {
	ClientLauncher launcher;
	printf("%s\n", argv[0]);
	launcher.launch(argv[1], argv[2]);
	return 0;
}


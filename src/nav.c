#include "config.h"
#include <unistd.h>
#include <limits.h>


int main(int argc, char *argv[])
{
	Snippets snip = {0};
	if(argc < 2) {
		printf("usage: nav [shortcut]");
		return 0;
	}
	snip.snippet = argv[1];
	getSnippetDir(&snip);
	return 0;
}

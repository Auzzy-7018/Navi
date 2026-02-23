#include "config.h"	
// usage:
// navi -a snippet pwd
// navi -a /manuel/input/file/path
int main(int argc, char *argv[])
{
	if(argc < 2 ) {
		printf("incorrect ammount of aruguments.\n");
		printf("usage:\n");
		printf("navi -a snippet pwd\n");
		printf("navi -a /manuel/input/filepath\n");
		printf("navi -l\n");
		printf("navi -r snippet\n");
		return 0;
	}
	char *command = argv[1];
	char *snippet = argv[2];
	char *directory = argv[3];
	char cwd[PATH_MAX];
	char yesno[3];
	Snippets snip;
	snip.snippet = snippet;
	if(strcmp(command,"-a") == 0 ) { // used -a to represent adding snippets 
			if(strcmp(directory, "pwd") == 0) { 

				if(getcwd(cwd, sizeof(cwd)) != NULL) {
					printf("%s\n", cwd);
					snip.dirPath  = cwd;

					if(snippetExist(&snip)) {
						printf("snippet already exists would you like to overwrite it? (y/n):");
						fgets(yesno, sizeof(yesno), stdin);
						yesno[strcspn(yesno, "\n")] = '\0';
						if(strcmp(yesno,"y") == 0) {
							updateSnippet(&snip);
						} else {
							return 0;
						}

					} else {
						saveSnippet(&snip);
					}
				} else {
					perror("cwd");
				}
					
			} else {
				if(dirExist(directory)) {
					snip.dirPath = directory;			
					if(snippetExist(&snip)) {
						printf("snippet already exists would you like to overwrite it? (y/n):");
						fgets(yesno, sizeof(yesno), stdin);
						yesno[strcspn(yesno, "\n")] = '\0';
						if(strcmp(yesno,"y") == 0) {
							updateSnippet(&snip);
						} else {
							return 0;
						}

					} else {
						saveSnippet(&snip);
					}
				} else {
					printf("the path you provided does not exist.\n");
				}
		}
	}
	else if(strcmp(command, "-l") == 0 ) { //used -l to represents listing 
		listSnippets();				
	}
	else if (strcmp(command, "-r") == 0) { //used -r to represents removing snippets out of the list.
		removeSnippet(&snip);		
	}

	return 0;
}	

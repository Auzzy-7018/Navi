#ifndef CONFIG_H
#define CONFIG_H
#define CONFIG_FILE "/.config/navi/navi.conf"
#define CONFIG_TEMPFILE "/.config/navi/navitemp.conf"
//Includes

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include <stdbool.h>

//Structs
typedef struct {
	char *snippet;
	char *dirPath;
}Snippets;
void getSnippetDir(Snippets *snippet);
bool saveSnippet(Snippets *snip);
bool updateSnippet(Snippets *snip);
bool dirExist(const char *path);
bool removeSnippet(Snippets *snippet);
void listSnippets();
bool snippetExist(Snippets *snip);
#endif


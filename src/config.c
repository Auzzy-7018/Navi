#include "config.h"
#include "var.h"


struct Snippets {
	char *snippet;
	char *dirPath;
};
bool saveSnippet(Snippets *snip) {
	const char *home = getenv("HOME");
	char path[PATH_MAX];
	bool saved = true;
	snprintf(path,sizeof(path), "%s/.config/navi/navi.conf", home);
	
	FILE *f = fopen(path, "a");
	
	if(!f) {
		perror("fopen");
		saved = false;
	}
	fprintf(f,"%s->%s\n",snip->snippet, snip->dirPath);
	fclose(f);
	
	return saved;
}
bool updateSnippet(Snippets *snip) {
	bool snipUpdated = false;
	const char *home = getenv("HOME");
	char path[PATH_MAX];	
	char tempPath[PATH_MAX];
	snprintf(path,sizeof(path), "%s/.config/navi/navi.conf", home);
	snprintf(tempPath,sizeof(tempPath), "%s/.config/navi/navitemp.conf", home);
	FILE *f = fopen(path,"r");
	FILE *ft = fopen(tempPath,"w");
	char *line = NULL;
	size_t cap = 0;
	var Tokens;
	var_init(&Tokens);
	bool deleted = false;
	while(getline(&line,&cap,f) != -1) {
		if(!ft) {
			perror("fopen");

		}		

		var_split(&Tokens, line, "->");
		if(strcmp(Tokens.get(&Tokens,0),snip->snippet) ==0) {
			fprintf(ft,"%s->%s\n",snip->snippet, snip->dirPath);
			Tokens.destroy(&Tokens);
			snipUpdated = true;
			continue;
		}
		fprintf(ft,"%s",line);
		Tokens.destroy(&Tokens);
	}
	fclose(f);
	fclose(ft);
	remove(path);
	rename(tempPath, path);

	return snipUpdated;
}
bool removeSnippet(Snippets *snip) {
	const char *home = getenv("HOME");
	char path[PATH_MAX];	
	char tempPath[PATH_MAX];
	snprintf(path,sizeof(path), "%s/.config/navi/navi.conf", home);
	snprintf(tempPath,sizeof(tempPath), "%s/.config/navi/navitemp.conf", home);
	FILE *f = fopen(path,"r");
	FILE *ft = fopen(tempPath,"w");
	char *line = NULL;
	size_t cap = 0;
	var Tokens;
	var_init(&Tokens);
	bool deleted = false;
	while(getline(&line,&cap,f) != -1) {
		if(!ft) {
			perror("fopen");
		}		

		var_split(&Tokens, line, "->");
		if(strcmp(Tokens.get(&Tokens,0),snip->snippet) ==0) {
			deleted = true;
			Tokens.destroy(&Tokens);
			continue;
		}
		fprintf(ft,"%s",line);
		Tokens.destroy(&Tokens);
	}
	fclose(f);
	fclose(ft);
	remove(path);
	rename(tempPath, path);

	return deleted;
}
bool snippetExist(Snippets *snip) {
	bool snipExist = false;
	const char *home = getenv("HOME");
	char path[PATH_MAX];
	snprintf(path,sizeof(path), "%s/.config/navi/navi.conf", home);
	FILE *f = fopen(path,"r");
	if(!f) {
		perror("fopen");
	}
	char *line = NULL;
	size_t cap = 0;
	var Tokens;
	var_init(&Tokens);
	while(getline(&line,&cap,f) != -1) {
		var_split(&Tokens, line, "->");
		if(strcmp(Tokens.get(&Tokens,0),snip->snippet) ==0) {
			snipExist = true;
			Tokens.destroy(&Tokens);
			break;
		}
		Tokens.destroy(&Tokens);
	}
	fclose(f);

	return snipExist;
}
bool dirExist(const char *path) {
	struct stat info;
	if(stat(path, &info) !=0) {
		return false;
	}

	return S_ISDIR(info.st_mode);
}
void listSnippets() {
	const char *home = getenv("HOME");
	char path[PATH_MAX];
	snprintf(path,sizeof(path), "%s/.config/navi/navi.conf", home);
	FILE *f = fopen(path,"r");
	if(!f) {
		perror("fopen");
	}
	char *line = NULL;
	size_t cap = 0;
	while(getline(&line,&cap,f) != -1) {
		printf("%s\n", line);	
	}
}
void getSnippetDir(Snippets *snip) {
	const char *home = getenv("HOME");
	char path[PATH_MAX];
	snprintf(path,sizeof(path), "%s/.config/navi/navi.conf", home);
	FILE *f = fopen(path,"r");
	if(!f) {
		perror("fopen");
	}
	char *line = NULL;
	size_t cap = 0;
	var Tokens;
	var_init(&Tokens);
	while(getline(&line,&cap,f) != -1) {
		var_split(&Tokens, line, "->");
		if(strcmp(Tokens.get(&Tokens,0),snip->snippet) ==0) {
			snip->dirPath = Tokens.get(&Tokens,1);
			printf("cd %s", snip->dirPath);
		}
		
		Tokens.destroy(&Tokens);
	}
	fclose(f);
}


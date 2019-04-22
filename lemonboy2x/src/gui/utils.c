
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#include "defs.h"
#include "utils.h"

#include <SDL/SDL.h>

#define DIRSEP "/"

/** Compare two entries in dirinfo.
Returns -1 if df[a] less than df[b] and so on.
Case insensitive, directories are always less than files.
 Warning: this function does not check limits of df. */
static int cmp_dirinfo(int a, int b, struct dirinfo *df){
	if(df->type[a]==TYPE_EMPTY) return -1;
	if(df->type[a]==TYPE_DIR){
		if(df->type[b]==TYPE_DIR)
			return strcasecmp(df->file[a], df->file[b]);
		else
			return -1;
	}else{
		if(df->type[b]==TYPE_DIR)
			return 1;
		else
			return strcasecmp(df->file[a], df->file[b]);
	}
}

/** Returns a dirinfo structure with the contents of a directory.
 The returned structure is sorted according to cmp_dirinfo.
 - max is the maximum number of items in dirinfo
 - if include_dirs, directories are included in the list
 */
struct dirinfo * list_dir(char *dirname, unsigned int max, int include_dirs){
	struct dirinfo *df=NULL;
	struct dirent *dp;
	struct stat statbuf;
	char *cname=NULL; // complete name of a file, including directory
	DIR *dir;
	int nf=0;
	int i=0, j=0, min=0;
	
	// creates the dirinfo structure
	df = malloc(sizeof(struct dirinfo));
	df->file = malloc(max*sizeof(char **));
	df->type = calloc(sizeof(unsigned char), max); // we use calloc: TYPE_EMPTY=0
	
	errno=0;
	
	if((dir = opendir(dirname))==NULL){
#ifdef DEBUG
		printf(__FILE__ "Error reading dir %s: %s\n", dirname, strerror(errno));
#endif
		return NULL;
	}
	
	if(df->file == NULL || df->type == NULL){
		DPRINT(__FILE__ ": Error allocating memory");
		return NULL;
	}
	
	nf = 0; // number of read files
	while (nf<max && (dp = readdir(dir)) != NULL) {
		// ignore '.'
		if(dp->d_name[0] == '.' && dp->d_name[1]==0) continue;
		// ignore files that start with '.[a-z]'
		if(dp->d_name[0] == '.' && dp->d_name[1]!='.') continue;
		
		// save in cname the complete path to the file
		if((cname = (char *)malloc(sizeof(char) * (strlen(dirname) + strlen(dp->d_name) + 3))) == NULL){
			DPRINT(__FILE__ "Error allocating memory");
			continue;
		}
		sprintf(cname, "%s" DIRSEP "%s", dirname, dp->d_name);
		
		// get stats fot the file
		if(stat(cname, &statbuf) == -1){
#ifdef DEBUG
			printf("Error reading info for %s: %s\n", dp->d_name, strerror(errno));
			continue;
#endif
		}
		
		if(S_ISDIR(statbuf.st_mode)){
			if(include_dirs == INCLUDE_DIRS){
				// include dirs only if include_dirs
				df->file[nf] = cname;
				df->type[nf] = TYPE_DIR;
				nf += 1;
			}
		}else{
			// include always files
			df->file[nf] = cname;
			df->type[nf] = TYPE_FILE;
			nf += 1;
		}
	}
	df->num_files = nf;
	closedir(dir);
	
	// Sort the list using Bubble Sort
	// We can't use qsort since it need an array of structs, and not a struct with arrays. My fault.
	// TODO: change to something better?
	for(i=0; i<nf-1; i++){
		min=i;
		for(j=i+1; j<nf; j++) if(cmp_dirinfo(j, min, df)<0) min=j;
		if(min!=i){
			j = df->type[i];
			df->type[i] = df->type[min];
			df->type[min] = j;
			cname = df->file[i];
			df->file[i] = df->file[min];
			df->file[min] = cname;
		}
	}

	return df;
}


/** Free a dirinfo structure. Do not free the structure pointer itself */
void free_dirinfo(struct dirinfo *df){
	int i;
	if(df){
		for(i=0; df->type[i]!=TYPE_EMPTY; i++) FREE(df->file[i]);
		FREE(df->file);
		FREE(df->type);
	}
}

/** Converts an entry in dirinfo into a human readable string */
char * human_readable(struct dirinfo *df, int i){
	char *s=NULL;
	char *ss=NULL;
	if(df->type[i] == TYPE_DIR){
		// if it is a directory, let's add special marks
		s = only_rom_name(df->file[i]);
		// change '..' (returned as '.' by only_rom_name) to '..'
		if(s[0] == '.' && s[1] == 0){
			FREE(s);
			if((s = strdup(".."))==NULL){
				return NULL;
			}
		}
		// add '\\' marks to directories
		if((ss = malloc(sizeof(char) * (strlen(s) + 2)))==NULL){
			return NULL;
		}
		sprintf(ss, "`%s", s);	// the special mark for directories
		FREE(s);
		return ss;
	} else {
		// if it is a file, just remove the directory and suffix
		return only_rom_name(df->file[i]);
	}
}

/**  Returns a new pointer to the name of a file.
	/test/hello.rom returns hello
	/test/hello returns hello
	/test/hello.rom.ram returns hello.rom */
char *only_rom_name(char *rn){
	// TODO: Is this the same that base()? Not sure
	char *s=NULL;
	int i, in, fi, n;
	if(rn==NULL) return NULL;
	n=strlen(rn);
	in=0;
	fi=-1;
	for(i=n-1; i>=0; i--){
		if(fi==-1 && rn[i]=='.') fi=i;
		if(rn[i]=='/') {in=i+1; break; }
	}
	if(fi<0) fi=n;
	if(in>n) in=n;
	if((s=calloc(fi-in+1, sizeof(char)))==NULL) return NULL;
	strncpy(s, rn+in, fi-in);
#ifdef DEBUG
	//printf("Only rom name: #%s# -> #%s# (%d, %d)\n", rn, s, in, fi);
#endif
	return s;
}

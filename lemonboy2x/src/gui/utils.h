
#ifndef _UTILS_H
#define _UTILS_H

#define LISTINFO_MAX_FILES 1024

#define TYPE_FILE 1
#define TYPE_DIR 2
#define TYPE_EMPTY 0

#define INCLUDE_DIRS 1
#define NO_DIRS 0

/** Information about a directory */
struct dirinfo{
	char **file;			// name of the files inside the directory (including the directory name)
	unsigned char *type;		// types of the files inside the directory (0=file, 1=dir, 0xff=empty)
	int num_files;			// number of files in the structure
	unsigned int max_length;	// max length of the name of a file
};

/** Returns a dirinfo structure with the contents of a directory.
 The returned structure is sorted according to cmp_dirinfo.
 - max is the maximum number of items in dirinfo
 - if include_dirs, directories are included in the list
 */
struct dirinfo * list_dir(char *dir, unsigned int max, int include_dirs);
/** Free a dirinfo structure. Do not free the structure pointer itself */
void free_dirinfo(struct dirinfo *df);
/**  Returns a new pointer to the name of a file.
	/test/hello.rom returns hello
	/test/hello returns hello
	/test/hello.rom.ram returns hello.rom */
char *only_rom_name(char *);
/** Converts an entry in dirinfo into a human readable string */
char * human_readable(struct dirinfo *df, int i);

#endif


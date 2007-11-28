#ifndef GTB_IO_UTIL_INCLUDED
#define GTB_IO_UTIL_INCLUDED

#ifndef GTB_NAMESPACE_INCLUDED
#include "namespace.hpp"
#endif

#ifndef GTB_ERRORS_INCLUDED
#include "errors.hpp"
#endif

#ifndef GTB_CXX_STRING_INCLUDED
#include <string>
#define GTB_CXX_STRING_INCLUDED
#endif

#ifndef GTB_C_STDIO_INCLUDED
#include <stdio.h>
#define GTB_C_STDIO_INCLUDED
#endif

GTB_BEGIN_NAMESPACE


std::string get_file_base_name(const char *file_name);
std::string get_file_base_name(const char *file_name, const char *suffix);
std::string get_file_extension(const char *file_name);

bool file_exists(const char *file_name);
bool dir_exists(const char *dir_name);

ino_t get_file_inode(const char *file_name);
ino_t get_file_inode(int fd);

off_t get_file_size(const char *file_name);
off_t get_file_size(int fd);

void make_dir(const char *name);

bool read_line(FILE *fp, std::string &s);
bool read_line_drop_newline(FILE *fp, std::string &s);

bool name_has_extension(const char *name, const char *ext);


GTB_END_NAMESPACE


#define GTB_FOPEN(fp, file_name, mode)				\
do {								\
	fp = fopen(file_name, mode);				\
	if (fp == NULL) {					\
		GTB_ERROR3("can't open \"%s\" in \"%s\" mode",	\
			   file_name, mode);			\
	}							\
} while (0)


#endif // GTB_IO_UTIL_INCLUDED

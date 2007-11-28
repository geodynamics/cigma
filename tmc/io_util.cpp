#ifndef GTB_IO_UTIL_INCLUDED
#include "io_util.hpp"
#endif

#ifndef GTB_ASSERTIONS_INCLUDED
#include "assertions.hpp"
#endif

#ifndef GTB_C_ERRNO_INCLUDED
#include <errno.h>
#define GTB_C_ERRNO_INCLUDED
#endif

#include "gtb_config.h"

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifndef GTB_C_ASSERT_INCLUDED
#include <assert.h>
#define GTB_C_ASSERT_INCLUDED
#endif

#ifndef GTB_C_CTYPE_INCLUDED
#include <ctype.h>
#define GTB_C_CTYPE_INCLUDED
#endif

GTB_BEGIN_NAMESPACE


std::string get_file_base_name(const char *file_name)
{
	assert(NULL != file_name);
	unsigned n = strlen(file_name);

	// degenerate case
	if (0 == n) {
		return "";
	}

	// find right end
	unsigned r = n - 1;
	while ((r > 0) && ('/' == file_name[r])) {
		r--;
	}

	// find left end
	unsigned l = r;
	while ((l > 0) && ('/' != file_name[l])) {
		l--;
	}
	if (('/' == file_name[l]) && (l < r)) {
		l++;
	}

	// copy base name
	std::string s;
	for (unsigned i = l; i <= r; i++) {
		GTB_CHECK(i < n);
		s += file_name[i];
	}
	return s;
}


std::string get_file_base_name(const char *file_name, const char *suffix)
{
	assert(NULL != file_name);
	assert(NULL != suffix);
	int n = strlen(file_name);

	// degenerate case
	if (0 == n) {
		return "";
	}

	// find right end
	int r = n - 1;
	while ((r > 0) && ('/' == file_name[r])) {
		r--;
	}

	// check for suffix
	int m = strlen(suffix);
	if (r - (m - 1) >= 0) {
		const char *p = file_name + r - (m - 1);
		if (strcmp(p, suffix) == 0) {
			if (m == n) {
				return "";
			}
			assert(r >= m);
			r -= m;
		}
	}

	// find left end
	int l = r;
	while ((l > 0) && ('/' != file_name[l])) {
		l--;
	}
	if (('/' == file_name[l]) && (l < r)) {
		l++;
	}

	// copy base name
	assert(l >= 0);
	assert(r >= l);
	std::string s;
	for (int i = l; i <= r; i++) {
		GTB_CHECK(i < n);
		s += file_name[i];
	}
	return s;
}


std::string get_file_extension(const char *file_name)
{
	assert(NULL != file_name);
	unsigned n = strlen(file_name);

	// degenerate case
	if (0 == n) {
		return "";
	}

	// check for directory
	if ('/' == file_name[n - 1]) {
		fprintf(stderr, "%s is a directory\n", file_name);
		exit(EXIT_FAILURE);
	}

	// find '.'
	unsigned r = n - 1;
	unsigned l = n - 1;
	while ((l > 0) && ('.' != file_name[l])) {
		l--;
	}

	// no extension case
	if ('.' != file_name[l]) {
		return "";
	}

	// skip '.'
	l++;

	// copy extension
	std::string s;
	for (unsigned i = l; i <= r; i++) {
		GTB_CHECK(i < n);
		s += file_name[i];
	}
	return s;
}


bool file_exists(const char *file_name)
{
	int old_errno = errno;
	bool ret;
	struct stat buf;
	if ((stat(file_name, &buf) == 0) && S_ISREG(buf.st_mode)) {
		ret = true;
	} else {
		ret = false;
	}
	errno = old_errno;
	return ret;
}


bool dir_exists(const char *dir_name)
{
	int old_errno = errno;
	bool ret;
	struct stat buf;
	if ((stat(dir_name, &buf) == 0) && S_ISDIR(buf.st_mode)) {
		ret = true;
	} else {
		ret = false;
	}
	errno = old_errno;
	return ret;
}


ino_t get_file_inode(const char *file_name)
{
	struct stat buf;
	if (stat(file_name, &buf) != 0) {
		GTB_ERROR2("failed to stat %s\n", file_name);
	}
	if (!S_ISREG(buf.st_mode)) {
		GTB_ERROR2("%s is not a regular file\n", file_name);
	}
	return buf.st_ino;
}


ino_t get_file_inode(int fd)
{
	struct stat buf;
	if (fstat(fd, &buf) != 0) {
		GTB_ERROR("failed to stat file\n");
	}
	if (!S_ISREG(buf.st_mode)) {
		GTB_ERROR("not a regular file\n");
	}
	return buf.st_ino;
}


off_t get_file_size(const char *file_name)
{
	struct stat buf;
	if (stat(file_name, &buf) != 0) {
		GTB_ERROR2("failed to stat %s\n", file_name);
	}
	if (!S_ISREG(buf.st_mode)) {
		GTB_ERROR2("not a regular %s\n", file_name);
	}
	return buf.st_size;
}


off_t get_file_size(int fd)
{
	struct stat buf;
	if (fstat(fd, &buf) != 0) {
		GTB_ERROR("failed to stat file\n");
	}
	if (!S_ISREG(buf.st_mode)) {
		GTB_ERROR("not a regular file\n");
	}
	return buf.st_size;
}


void make_dir(const char *name)
{
#ifdef WIN32
	if (mkdir(name) != 0) {
		GTB_ERROR("failed to make dir");
	}
#else
	mode_t mode = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;
	if (mkdir(name, mode) != 0) {
		GTB_ERROR2("failed to make dir %s", name);
	}
#endif
}


bool read_line(FILE *fp, std::string &s)
{
	s = "";
	char buf[100];
	int n = 0;
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		s += buf;
		n++;
		int len = strlen(buf);
		if ((len > 0) && (buf[len - 1] == '\n')) {
			break;
		}
	}
	return n > 0;
}


bool read_line_drop_newline(FILE *fp, std::string &s)
{
	s = "";
	char buf[100];
	int n = 0;
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		n++;
		int len = strlen(buf);
		if ((len > 0) && (buf[len - 1] == '\n')) {
			buf[len - 1] = '\0';
			s += buf;
			break;
		} else {
			s += buf;
		}
	}
	return n > 0;
}


bool name_has_extension(const char *name, const char *ext)
{
	bool ret = false;
	int l1 = strlen(name);
	int l2 = strlen(ext);
	if (l2 < l1) {
		bool differ = false;
		int i, j;
		for (i = (l1 - l2), j = 0;
		     (i < l1) && (j < l2);
		     i++, j++) {
			if (toupper(name[i]) != toupper(ext[j])) {
				differ = true;
				break;
			}
		}
		if (!differ) {
			ret = true;
		}
	}
	return ret;
}


GTB_END_NAMESPACE

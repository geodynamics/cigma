#ifndef __PATH_UTILS_H__
#define __PATH_UTILS_H__

bool HdfExtension(const char *ext);
bool TextExtension(const char *ext);
bool VtkExtension(const char *ext);
bool LibExtension(const char *ext);

bool FileExists(const char *filename);
bool IsHdfFile(const char *filename);
bool IsVtkFile(const char *filename);
bool IsLibFile(const char *filename);


#endif

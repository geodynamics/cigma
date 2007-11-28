// http://www.codeproject.com/string/stringsplit.asp
#ifndef __STRINGUTILS_H__
#define __STRINGUTILS_H__

#include <string>
#include <vector>

using namespace std;

int SplitString(const string& input,
                const string& delimiter,
                vector<string>& results,
                bool includeEmpties = true);

#endif // __STRINGUTILS_H__

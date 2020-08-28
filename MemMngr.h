#pragma once

#if defined(DEBUG) || defined(_DEBUG) || defined(__debug)

//#define new new(__FILE__, __LINE__) do not work -> find solutin at https://stackoverflow.com/questions/619467/macro-to-replace-c-operator-new?answertab=active#tab-top

extern const char* __file__;
extern int __line__;
#define new (__file__=__FILE__,__line__=__LINE__) && 0 ? NULL : new


#endif
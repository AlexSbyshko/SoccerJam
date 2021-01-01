#if defined _module_included
 #endinput
#endif
#define _module_included

typedef InitFuncDel = function void ()

enum struct Module
{
    InitFuncDel InitFunc
}
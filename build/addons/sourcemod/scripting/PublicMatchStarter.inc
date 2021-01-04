#if defined _PublicMatchStarter_included
 #endinput
#endif
#define _PublicMatchStarter_included

typedef StartPublicMatchDel = function void ()

enum struct PublicMatchStarter
{
    StartPublicMatchDel StartPublicMatchFunc
    
    void StartPublicMatch()
    {
        Call_StartFunction(INVALID_HANDLE, this.StartPublicMatchFunc)
        Call_Finish()
    }
}
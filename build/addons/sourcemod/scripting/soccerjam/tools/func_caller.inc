//functag public I_void_void()
//functag public I_void_i(i)

typedef I_void_void = function Action ()
typedef I_void_i = function Action (int args)

I_Call_void_void(I_void_void:func)
{
	if (func != INVALID_FUNCTION)
	{
		Call_StartFunction(INVALID_HANDLE, func)
		Call_Finish()
	}
}
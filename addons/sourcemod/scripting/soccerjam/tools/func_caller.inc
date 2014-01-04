functag public I_void_void()
functag public I_void_i(i)

I_Call_void_void(I_void_void:function)
{
	if (function != INVALID_FUNCTION)
	{
		Call_StartFunction(INVALID_HANDLE, function)
		Call_Finish()
	}
}
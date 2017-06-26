#include "stdglib.h"
#include "glib.h"

char glb_TraceTxtBuf[256];

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
	}
	return 1;   // ok
}

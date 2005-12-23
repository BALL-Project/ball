// libBALL.cpp : Definiert den Einstiegspunkt für die DLL-Anwendung.
//

// Selten verwendete Teile der Windows-Header nicht einbinden
#define WIN32_LEAN_AND_MEAN		
// Windows-Headerdateien:
#include <windows.h>


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

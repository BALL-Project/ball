// libBALL.cpp : Definiert den Einstiegspunkt für die DLL-Anwendung.
//

#include "stdafx.h"
#include "libBALL.h"
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

// Dies ist das Beispiel einer exportierten Variablen
LIBBALL_API int nlibBALL=0;

// Dies ist das Beispiel einer exportierten Funktion
LIBBALL_API int fnlibBALL(void)
{
	return 42;
}

// Dies ist der Konstruktor einer Klasse, die exportiert wurde.
// Siehe libBALL.h für die Klassendefinition
ClibBALL::ClibBALL()
{ 
	return; 
}

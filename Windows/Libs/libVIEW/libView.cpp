// libView.cpp : Definiert den Einstiegspunkt für die DLL-Anwendung.
//

#include "stdafx.h"
#include "libView.h"
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
LIBVIEW_API int nlibView=0;

// Dies ist das Beispiel einer exportierten Funktion
LIBVIEW_API int fnlibView(void)
{
	return 42;
}

// Dies ist der Konstruktor einer Klasse, die exportiert wurde.
// Siehe libView.h für die Klassendefinition
ClibView::ClibView()
{ 
	return; 
}

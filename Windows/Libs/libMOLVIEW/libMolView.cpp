// libMolView.cpp : Definiert den Einstiegspunkt für die DLL-Anwendung.
//

#include "stdafx.h"
#include "libMolView.h"
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
LIBMOLVIEW_API int nlibMolView=0;

// Dies ist das Beispiel einer exportierten Funktion
LIBMOLVIEW_API int fnlibMolView(void)
{
	return 42;
}

// Dies ist der Konstruktor einer Klasse, die exportiert wurde.
// Siehe libMolView.h für die Klassendefinition
ClibMolView::ClibMolView()
{ 
	return; 
}

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Folgender ifdef-Block ist die Standardmethode zum Erstellen von Makros, die das Exportieren 
// aus einer DLL vereinfachen. Alle Dateien in der DLL werden mit dem LIBBALL_EXPORTS-Symbol
// kompiliert, das in der Befehlszeile definiert wurde. Das Symbol darf nicht für ein Projekt definiert werden,
// das diese DLL verwendet. Alle anderen Projekte, deren Quelldateien diese Datei beinhalten, erkennen 
// LIBBALL_API-Funktionen als aus einer DLL importiert, während die DLL mit diesem Makro
// definierte Symbole als exportiert ansieht.
#ifdef LIBBALL_EXPORTS
#define LIBBALL_API __declspec(dllexport)
#else
#define LIBBALL_API __declspec(dllimport)
#endif

// Diese Klasse wird aus libBALL.dll exportiert
class LIBBALL_API ClibBALL {
public:
	ClibBALL(void);
	// TODO: Fügen Sie hier Ihre Methoden ein.
};

extern LIBBALL_API int nlibBALL;

LIBBALL_API int fnlibBALL(void);

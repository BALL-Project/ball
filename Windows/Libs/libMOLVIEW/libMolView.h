// Folgender ifdef-Block ist die Standardmethode zum Erstellen von Makros, die das Exportieren 
// aus einer DLL vereinfachen. Alle Dateien in der DLL werden mit dem LIBMOLVIEW_EXPORTS-Symbol
// kompiliert, das in der Befehlszeile definiert wurde. Das Symbol darf nicht für ein Projekt definiert werden,
// das diese DLL verwendet. Alle anderen Projekte, deren Quelldateien diese Datei beinhalten, erkennen 
// LIBMOLVIEW_API-Funktionen als aus einer DLL importiert, während die DLL mit diesem Makro
// definierte Symbole als exportiert ansieht.
#ifdef LIBMOLVIEW_EXPORTS
#define LIBMOLVIEW_API __declspec(dllexport)
#else
#define LIBMOLVIEW_API __declspec(dllimport)
#endif

// Diese Klasse wird aus libMolView.dll exportiert
class LIBMOLVIEW_API ClibMolView {
public:
	ClibMolView(void);
	// TODO: Fügen Sie hier Ihre Methoden ein.
};

extern LIBMOLVIEW_API int nlibMolView;

LIBMOLVIEW_API int fnlibMolView(void);

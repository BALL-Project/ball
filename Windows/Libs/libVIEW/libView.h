// Folgender ifdef-Block ist die Standardmethode zum Erstellen von Makros, die das Exportieren 
// aus einer DLL vereinfachen. Alle Dateien in der DLL werden mit dem LIBVIEW_EXPORTS-Symbol
// kompiliert, das in der Befehlszeile definiert wurde. Das Symbol darf nicht für ein Projekt definiert werden,
// das diese DLL verwendet. Alle anderen Projekte, deren Quelldateien diese Datei beinhalten, erkennen 
// LIBVIEW_API-Funktionen als aus einer DLL importiert, während die DLL mit diesem Makro
// definierte Symbole als exportiert ansieht.
#ifdef LIBVIEW_EXPORTS
#define LIBVIEW_API __declspec(dllexport)
#else
#define LIBVIEW_API __declspec(dllimport)
#endif

// Diese Klasse wird aus libView.dll exportiert
class LIBVIEW_API ClibView {
public:
	ClibView(void);
	// TODO: Fügen Sie hier Ihre Methoden ein.
};

extern LIBVIEW_API int nlibView;

LIBVIEW_API int fnlibView(void);

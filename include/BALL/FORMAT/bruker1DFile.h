// $Id: bruker1DFile.h,v 1.1 2000/09/14 12:23:23 oliver Exp $

#include <vector>
#include <glib.h>

#include <BALL/SYSTEM/file.h>
#include <BALL/DATATYPE/string.h>

#include "BrukerParameter.h"

namespace BALL
{

	class Bruker1D : public BALL::File
	{
	 public:
		/**
		 * Konstruktor. Wichtig: name ist hier der Name des Bruker-*Verzeichnisses*
		 */
		Bruker1D( const String& name, OpenMode open_mode = IN|BINARY );

		/**
		 * Copy-Konstruktor.
		 */
		Bruker1D( const Bruker1D& file );

		/**
		 * Destruktor.
		 */
		~Bruker1D();

		/**
		 * Einlesen eines 1D-Spektrums nach dat.
		 */
		void read( vector<double>& dat );

	 protected:
		BrukerParameter *pars_;
		Size min_, max_;
	};
}

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_FORMAT_PUBCHEMDOWNLOADER_H
#define BALL_FORMAT_PUBCHEMDOWNLOADER_H

#ifndef BALL_DATATYPE_STRING_H
# include <BALL/DATATYPE/string.h>
#endif

namespace BALL
{
	/**
	 * \brief Downloads PubChem entries
	 *
	 * This class provides a simplistic, blocking interface to the PubChem REST
	 * query interface. It downloads the obtained records in SDF format and
	 */
	class BALL_EXPORT PubChemDownloader
	{
		public:
			/**
			 * Default constructor
			 */
			PubChemDownloader();

			/**
			 * Download an SD file corresponding to the Pubchem record
			 * identified by name.
			 *
			 * @param query The name of the substance that should be received.
			 * @param filename The path to the file into which the results should be saved.
			 *
			 * @return false if an error was encountered during download.
			 */
			bool downloadSDFByName(const String& query, const String& filename);

			/**
			 * Download an SD file corresponding to the Pubchem record
			 * identified by Pubchem CID
			 * 
			 * @param cid The CID of the substance that should be received.
			 * @param filename The path to the file into which the results should be saved.
			 *
			 * @return false if an error was encountered during download.
			 */
			bool downloadSDFByCID(int cid, const String& filename);

		private:
			/// Holds the base URL for the pubchem rest interface
			const String pubchem_rest_url_;

			/**
			 * Helper that does the actual download work.
			 *
			 * @param query The query defining the record
			 * @param type The type of the query
			 * @param filename The path to the resulting SD File
			 *
			 * @return false if an error was encountered during download
			 */
			bool downloadSDF_(const String& query, const String& type, const String& filename);
	};
}

#endif // BALL_FORMAT_PUBCHEMDOWNLOADER_H
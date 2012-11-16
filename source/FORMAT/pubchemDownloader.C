// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/pubchemDownloader.h>
#include <BALL/SYSTEM/simpleDownloader.h>

namespace BALL
{
	PubChemDownloader::PubChemDownloader()
		: pubchem_rest_url_("http://pubchem.ncbi.nlm.nih.gov/rest/pug/compound/")
	{
	}

	bool PubChemDownloader::downloadSDFByName(const String& query, const String& filename)
	{ 
		return downloadSDF_(query, "name", filename);
	}

	bool PubChemDownloader::downloadSDFByCID(int cid, const String& filename)
	{
		return downloadSDF_(String(cid), "cid", filename);
	}

	bool PubChemDownloader::downloadSDF_(const String& query, const String& type, const String& filename)
	{ 
		SimpleDownloader entrez_dl(pubchem_rest_url_ + type + "/" + query + "/SDF");

		return entrez_dl.downloadToFile(filename) == 0;
	}
}
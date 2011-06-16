// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_FORMAT_PUBCHEMDOWNLOADER_H
#define BALL_FORMAT_PUBCHEMDOWNLOADER_H

#ifndef BALL_SYSTEM_SIMPLEDOWNLOADER_H
# include <BALL/SYSTEM/simpleDownloader.h>
#endif

#include <QDomDocument>

namespace BALL
{
	class PubChemDownloader
	{
		public:
			PubChemDownloader();
			virtual ~PubChemDownloader() {};

			bool downloadSDF(const String& query, const String& filename);

		protected:
			QDomDocument pollPubChem_(const QString& request_id);
			void buildSimpleTree_(const String& names);
			void addTextNode_(const String& value);

			String esearch_base_url_;
			String pug_base_url_;

			SimpleDownloader dl_;
			QDomDocument request_;
			QDomElement last_node_;
			QDomElement current_node_;
	};
}

#endif // BALL_FORMAT_PUBCHEMDOWNLOADER_H


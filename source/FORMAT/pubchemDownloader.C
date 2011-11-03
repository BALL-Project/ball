// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/pubchemDownloader.h>

namespace BALL
{
	PubChemDownloader::PubChemDownloader()
		: esearch_base_url_("http://eutils.ncbi.nlm.nih.gov/entrez/eutils/esearch.fcgi?db=pccompound&usehistory=y&retmax=2&term="),
			pug_base_url_("http://pubchem.ncbi.nlm.nih.gov/pug/pug.cgi"),
		 	dl_(pug_base_url_),
		  request_("Request")
	{
		connect(&ftp_, SIGNAL(dataTransferProgress(qint64, qint64)),
						this,  SLOT(dataTransferProgress(qint64, qint64)));

		connect(&ftp_, SIGNAL(done(bool)), 
		        this, SLOT(downloadFinished(bool)));
	}

	void PubChemDownloader::dataTransferProgress(qint64 a, qint64 b)
	{
		// for some reason, connecting the signals directly did not work as expected... 
		emit(downloadProgress(a, b)); 
	} 
	
	bool PubChemDownloader::downloadSDF(const String& query, const String& filename, bool blocking) 
	{ 
		std::vector<char> response; 

		SimpleDownloader entrez_dl(esearch_base_url_+query); 
		entrez_dl.downloadToBuffer(response);

		if (response.size() == 0)
		{
			emit downloadFinished(outfile_.fileName());
			return false;
		}
		response.push_back('\0');

		QDomDocument response_dom("Response");
		response_dom.setContent(QString::fromLatin1(&response[0]));

		QString query_key = response_dom.elementsByTagName("QueryKey").at(0).firstChild().nodeValue();
		QString web_env   = response_dom.elementsByTagName("WebEnv").at(0).firstChild().nodeValue();

		// the search has finished. now let's try to download the data.
		if ( (query_key != "") && (web_env != ""))
		{
			current_node_ = request_.createElement("PCT-Data");
			request_.appendChild(current_node_);

			buildSimpleTree_("PCT-Data_input|PCT-InputData|PCT-InputData_download|PCT-Download");
			QDomElement uid_node = current_node_;
			
			buildSimpleTree_("PCT-Download_uids|PCT-QueryUids|PCT-QueryUids_entrez|PCT-Entrez");
			QDomElement entrez_node = current_node_;
			
			buildSimpleTree_("PCT-Entrez_db");
			addTextNode_("pccompound");
			current_node_ = entrez_node;

			buildSimpleTree_("PCT-Entrez_query-key");
			addTextNode_((String)query_key);
			current_node_ = entrez_node;

			buildSimpleTree_("PCT-Entrez_webenv");
			addTextNode_((String)web_env);

			current_node_ = uid_node;
			buildSimpleTree_("PCT-Download_format");
			current_node_.setAttribute("value", "sdf");

			current_node_ = uid_node;
			buildSimpleTree_("PCT-Download_compression");
			current_node_.setAttribute("value", "none");

			dl_.uploadStringToBuffer(request_.toString().toAscii().data(), response);
			response.push_back('\0');
			response_dom.setContent(QString::fromLatin1(&response[0]));

			QDomNodeList waiting_list = response_dom.elementsByTagName("PCT-Waiting_reqid");

			if (!waiting_list.isEmpty())
			{
				response_dom = pollPubChem_(waiting_list.at(0).firstChild().nodeValue());
			}

			QDomNodeList download_url_list = response_dom.elementsByTagName("PCT-Download-URL_url");

			if (!download_url_list.isEmpty())
			{
				QUrl download_url(download_url_list.at(0).firstChild().nodeValue());
				if (!download_url.isValid())
				{
					emit downloadFinished(outfile_.fileName());
					return false;
				}
				
				// blocking downloads can be handled by the simple downloader
				// non-blocking ones are more complicated... at the moment, this
				// will only work for ftp downloads
				if (blocking || download_url.scheme() != "ftp")
				{
					entrez_dl.setURL(download_url);
					entrez_dl.downloadToFile(filename);
				}
				else
				{
					// ok, we have to do it the hard way
					outfile_.setFileName(filename.c_str());
					outfile_.open(QIODevice::WriteOnly);

					ftp_.connectToHost(download_url.host(), download_url.port(21));
					ftp_.login();
					ftp_.get(download_url.path(), &outfile_);
					ftp_.close();
				}
			}
			else
			{
				emit downloadFinished(outfile_.fileName());
				return false;
			}
		} 
		else 
		{
			emit downloadFinished(outfile_.fileName());
			return false;
		}

		return true;
	}

	QDomDocument PubChemDownloader::pollPubChem_(const QString& request_id)
	{
		QDomDocument response_dom("Response");

		request_.clear();
		current_node_ = request_.createElement("PCT-Data");
		request_.appendChild(current_node_);
		buildSimpleTree_("PCT-Data_input|PCT-InputData|PCT-InputData_request|PCT-Request");

		QDomElement request_node = current_node_;
		buildSimpleTree_("PCT-Request_reqid");
		addTextNode_((String)request_id);
		current_node_ = request_node;
		buildSimpleTree_("PCT-Request_type");
		current_node_.setAttribute("value", "status");

		std::vector<char> response;
		QDomNodeList waiting_list;

		do 
		{
			dl_.uploadStringToBuffer(request_.toString().toAscii().data(), response);
			response.push_back('\0');
			response_dom.setContent(QString::fromLatin1(&response[0]));

			waiting_list = response_dom.elementsByTagName("PCT-Waiting_reqid");

		} while (!waiting_list.isEmpty());

		return response_dom;
	}

	void PubChemDownloader::buildSimpleTree_(const String& names)
	{
		std::vector<String> split;
		names.split(split, "|");

		for (Position i=0; i<split.size(); ++i)
		{
			last_node_ = current_node_;
			current_node_ = request_.createElement(split[i].c_str());
			last_node_.appendChild(current_node_);
		} 
	}

	void PubChemDownloader::addTextNode_(const String& value)
	{
		QDomText text = request_.createTextNode(value.c_str());
		current_node_.appendChild(text);
	}

	void PubChemDownloader::downloadFinished(bool error)
	{
		emit downloadFinished(outfile_.fileName());
		outfile_.close();
	}
}

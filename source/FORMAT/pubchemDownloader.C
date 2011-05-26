// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/pubchemDownloader.h>

namespace BALL
{
	PubChemDownloader::PubChemDownloader()
		: dl_("http://pubchem.ncbi.nlm.nih.gov/pug/pug.cgi"),
		  request_("Request")
	{
	/*	QDomElement root = request_.createElement("PCT-Data");
		request_.appendChild(root);

		current_node_ = root;

		buildSimpleTree_("PCT-Data_input|PCT-InputData|PCT-InputData_query|PCT-Query|PCT-Query_type");
		buildSimpleTree_("PCT-QueryType|PCT-QueryType_css|PCT-QueryCompoundCS");

		QDomElement query_compound_cs = current_node_;

		buildSimpleTree_("PCT-QueryCompoundCS_query|PCT-QueryCompoundCS_query_data");
		addTextNode_("C1=CC(=CC=C1C[C@@H](C(=O)O)N)O");
//		addTextNode_("caffeine");

		current_node_ = query_compound_cs;

		buildSimpleTree_("PCT-QueryCompoundCS_type|PCT-QueryCompoundCS_type_identical|PCT-CSIdentity");
		current_node_.setAttribute("value", "same-stereo-isotope");
		addTextNode_("5");
		current_node_ = query_compound_cs;

		buildSimpleTree_("PCT-QueryCompoundCS_results");
		addTextNode_("2000000");

		std::cout << request_.toString().toAscii().data() << std::endl;
		dl_.uploadStringToBuffer(request_.toString().toAscii().data(), response);
		*/

		std::vector<char> response;

		SimpleDownloader entrez_dl("http://eutils.ncbi.nlm.nih.gov/entrez/eutils/esearch.fcgi?db=pccompound&usehistory=y&retmax=0&term=caffeine");
		entrez_dl.downloadToBuffer(response);

		QDomDocument response_dom("Response");
		response_dom.setContent(QString::fromLatin1(&response[0]));

		QString query_key = response_dom.elementsByTagName("QueryKey").at(0).firstChild().nodeValue();
		QString web_env = response_dom.elementsByTagName("WebEnv").at(0).firstChild().nodeValue();

/*		QDomNodeList waiting_list = response_dom.elementsByTagName("PCT-Waiting_reqid");

		if (!waiting_list.isEmpty())
		{
			response_dom = pollPubChem_(waiting_list.at(0).firstChild().nodeValue());
		}

		// ok, apparently, the search has finished... let's see if it was successful
		QString status = response_dom.elementsByTagName("PCT-Status-Message_status").at(0).firstChild().attributes().namedItem("value").nodeValue();

		if (status == "success")
		{
*/
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

			std::cout << "bla: " << std::endl;
			std::cout << request_.toString().toAscii().data() << std::endl;

			dl_.uploadStringToBuffer(request_.toString().toAscii().data(), response);
			response_dom.setContent(QString::fromLatin1(&response[0]));

			QDomNodeList waiting_list = response_dom.elementsByTagName("PCT-Waiting_reqid");

			if (!waiting_list.isEmpty())
			{
				response_dom = pollPubChem_(waiting_list.at(0).firstChild().nodeValue());
			}
			std::cout << response_dom.toString().toAscii().data() << std::endl;

			QDomNodeList download_url_list = response_dom.elementsByTagName("PCT-Download-URL_url");

			if (!download_url_list.isEmpty())
			{
				entrez_dl.setURL((String)download_url_list.at(0).firstChild().nodeValue());
				std::cout << entrez_dl.getURL() << std::endl;

				entrez_dl.downloadToFile("test.sdf");
			}
		} 
		else 
		{
			std::cout << "Query failed!" << std::endl;
		}

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
			response_dom.setContent(QString::fromLatin1(&response[0]));

			waiting_list = response_dom.elementsByTagName("PCT-Waiting_reqid");

			std::cout << "Waiting..." << std::endl;
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
}

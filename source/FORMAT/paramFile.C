/// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/paramFile.h>

using namespace BALL;
using namespace std;


ParamFile::ParamFile(const String& filename, File::OpenMode open_mode)
{
	filename_ = filename;
	open_mode_ = open_mode;

	if(open_mode==File::MODE_IN)
	{
		mode_read_ = true;
		file_  = new QFile(filename_.c_str());
		if(!file_->open(QIODevice::ReadOnly | QIODevice::Text))
		{
			throw(Exception::GeneralException(__FILE__, __LINE__,"ParamFile::ParamFile","cannot open file for input"));
		}
		xmlIn_ = new QXmlStreamReader(file_);
	}
	if(open_mode==File::MODE_OUT)
	{
		mode_read_ = false;
		file_  = new QFile(filename_.c_str());
		if(!file_->open(QIODevice::WriteOnly| QIODevice::Text))
		{
			throw(Exception::GeneralException(__FILE__, __LINE__,"ParamFile::ParamFile","cannot create output file"));
		}
		xmlOut_ = new QXmlStreamWriter(file_);
		xmlOut_->setAutoFormatting(true);

		xmlOut_->writeStartDocument("1.0");
	}
}


ParamFile::~ParamFile()
{
	close();
// 	if(open_mode_==File::MODE_OUT) delete xmlOut_;
// 	else delete xmlIn_;
// 	delete file_;
}


void ParamFile::close()
{
	if(open_mode_==File::MODE_OUT)
	{
		xmlOut_->writeEndDocument();
		xmlOut_->writeEndElement(); // write end of PARAMETERS section
		file_->close();
	}
}



void ParamFile::writeSection(String section_name, String section_description, String version, const String& section_helptext,
		                         const String& category, const list<pair<String,ParameterDescription> >& descriptions,
														 const map<String,list<String> >& values)
{
	if (mode_read_)
	{
		throw BALL::Exception::GeneralException(__FILE__,__LINE__,"ParamFile::writeSection() error","Writing is not possible, file is opened in read mode!");
	}

	xmlOut_->writeStartElement("tool");
	xmlOut_->writeAttribute("status", "internal");
	xmlOut_->writeStartElement("name");
	xmlOut_->writeCharacters(section_name.c_str());
	xmlOut_->writeEndElement();
	xmlOut_->writeStartElement("version");
	xmlOut_->writeCharacters(version.c_str());
	xmlOut_->writeEndElement();
	xmlOut_->writeStartElement("description");
	xmlOut_->writeCharacters(section_description.c_str());
	xmlOut_->writeEndElement();
	xmlOut_->writeStartElement("manual");
	xmlOut_->writeCDATA(section_helptext.c_str());
	xmlOut_->writeEndElement();

	xmlOut_->writeStartElement("docurl");
	xmlOut_->writeEndElement();
	xmlOut_->writeStartElement("category");
	xmlOut_->writeCharacters(category.c_str());
	xmlOut_->writeEndElement();
	xmlOut_->writeStartElement("type");
	xmlOut_->writeEndElement();

	xmlOut_->writeStartElement("PARAMETERS");
	xmlOut_->writeAttribute("version","1.3");
	xmlOut_->writeStartElement("NODE");
	xmlOut_->writeAttribute("name","1");
	String s = "Instance '1' section for '"+section_name+"'";
	xmlOut_->writeAttribute("description",s.c_str());

	String previous_section = "";
	Size open_sections = 0;

	for(list<pair<String,ParameterDescription> >::const_iterator d_it=descriptions.begin();
		d_it!=descriptions.end(); d_it++)
	{
		map<String,list<String> >::const_iterator value_it = values.find(d_it->first);

		const String& section = d_it->second.category;
		if (section != previous_section)
		{
			if (previous_section != "")
			{
				xmlOut_->writeEndElement(); // </NODE>
				open_sections--;
			}
			if (section != "")
			{
				xmlOut_->writeStartElement("NODE");
				xmlOut_->writeAttribute("name", section.c_str());
				String description = "Parameters for section '";
				description += section;
				description += "'";
				xmlOut_->writeAttribute("description", description.c_str());
				open_sections++;
			}
			previous_section = section;
		}

		String type = "";
		bool is_list = false;
		String tag = "";
		if(d_it->second.type==INFILE)
		{
			tag = "input file";
			type = "string";
		}
		else if(d_it->second.type==OUTFILE)
		{
			tag = "output file";
			type = "string";
		}
		else if(d_it->second.type==STRING)
		{
			type = "string";
		}
		else if (d_it->second.type==INT)
		{
			type = "int";
		}
		else if (d_it->second.type==DOUBLE)
		{
			type = "float";
		}
		if(d_it->second.type==INFILELIST)
		{
			tag = "input file";
			type = "string";
			is_list = true;
		}
		if(d_it->second.type==OUTFILELIST)
		{
			tag = "output file";
			type = "string";
			is_list = true;
		}
		if(d_it->second.type==STRINGLIST)
		{
			type = "string";
			is_list = true;
		}
		else if (d_it->second.type==INTLIST)
		{
			type = "int";
			is_list = true;
		}
		else if (d_it->second.type==DOUBLELIST)
		{
			type = "float";
			is_list = true;
		}
		else if (d_it->second.type==GALAXY_OPT_OUTDIR)
		{
			type = "string";
			tag = "galaxy_opt_outdir";
		}
		else if (d_it->second.type==GALAXY_OPT_OUTID)
		{
			type = "string";
			tag = "galaxy_opt_outid";
		}


		if (d_it->second.mandatory)
		{
			if (tag != "")
			{
				tag += ", ";
			}
			tag += "mandatory";
		}

		if (d_it->second.advanced)
		{
			if (tag != "")
			{
				tag += ", ";
			}
			tag += "advanced";
		}

		if(!is_list)
		{
			xmlOut_->writeStartElement("ITEM");
		}
		else
		{
			xmlOut_->writeStartElement("ITEMLIST");
		}

		xmlOut_->writeAttribute("name",d_it->first.c_str());
		xmlOut_->writeAttribute("type",type.c_str());
		if (d_it->second.hidden)
			xmlOut_->writeAttribute("hidden", "true");
		xmlOut_->writeAttribute("description",d_it->second.description.c_str());
		if(tag!="")
		{
			xmlOut_->writeAttribute("tags",tag.c_str());
		}

		if (!d_it->second.allowed_values.empty())
		{
			String restr = "";
			for (list<String>::const_iterator it=d_it->second.allowed_values.begin();
				it!=d_it->second.allowed_values.end(); it++)
			{
				if (restr != "")
				{
					if (type == "string") restr += ", ";
					else restr += ":";
				}
				restr += *it;
			}
			xmlOut_->writeAttribute("restrictions",restr.c_str());
		}

		if (!d_it->second.supported_formats.empty())
		{
			String formats = "";
			for (list<String>::const_iterator it=d_it->second.supported_formats.begin();
				it!=d_it->second.supported_formats.end(); it++)
			{
				if (formats != "")
				{
					formats += ", ";
				}
				formats += *it;
			}
			xmlOut_->writeAttribute("supported_formats", formats.c_str());
		}

		if (d_it->second.output_format_source != "")
		{
			xmlOut_->writeAttribute("output_format_source", d_it->second.output_format_source.c_str());
		}

		if(value_it==values.end()) // no value has been set for this parameter
		{
			xmlOut_->writeAttribute("value","");
		}
		else
		{
			if(!is_list)
			{
				xmlOut_->writeAttribute("value",value_it->second.front().c_str());
			}
			else
			{
				for(list<String>::const_iterator l_it=value_it->second.begin(); l_it!=value_it->second.end(); l_it++)
				{
					xmlOut_->writeStartElement("LISTITEM");
					xmlOut_->writeAttribute("value",l_it->c_str());
					xmlOut_->writeEndElement();
				}
			}
		}

		xmlOut_->writeEndElement(); // end ITEM section

	}

	for (Size i = 0; i < open_sections; i++)
	{
		xmlOut_->writeEndElement(); // </NODE>
	}
	xmlOut_->writeEndElement(); // </NODE>
	xmlOut_->writeEndElement(); // </PARAMETERS>
	xmlOut_->writeEndElement(); // </tool>
}


void ParamFile::readSection(String& section_name, String& section_description, String& version,  String& section_helptext,
		                        String& category, list<pair<String,ParameterDescription> >& descriptions,
														map<String,list<String> >& values,bool overwrite_existing)
{
	if (!mode_read_)
	{
		throw BALL::Exception::GeneralException(__FILE__,__LINE__,"ParamFile::readSection() error","Reading is not possible, file is opened in write mode!");
	}

	section_name = "";
	section_description = "";
	version = "";
	section_helptext = "";
	category = "";

	int inside_par_node = 0;
	String section = "";

	ParameterDescription pd;
	list<String> values_current_par;

	typedef list<pair<String,ParameterDescription> >::iterator ListIterator;
	map<String, ListIterator> description_map;
	for(ListIterator d_it=descriptions.begin(); d_it!=descriptions.end(); d_it++)
	{
		description_map.insert(make_pair(d_it->first, d_it));
	}

	while(!xmlIn_->atEnd() && !xmlIn_->hasError())
	{
		QXmlStreamReader::TokenType token = xmlIn_->readNext();
		String tagname = xmlIn_->name().toString().toStdString();

		if (token == QXmlStreamReader::StartElement)
		{
			if (!inside_par_node)
			{
				if(tagname=="name")
				{
					section_name = xmlIn_->readElementText().toStdString();
				}
				else if(tagname=="version")
				{
					version = xmlIn_->readElementText().toStdString();
				}
				else if(tagname=="description")
				{
					section_description = xmlIn_->readElementText().toStdString();
				}
				else if(tagname=="category")
				{
					category = xmlIn_->readElementText().toStdString();
				}
				else if(tagname=="manual")
				{
					section_helptext = xmlIn_->readElementText().toStdString();
				}
			}

			QXmlStreamAttributes attrs = xmlIn_->attributes();
			String category = "";
			if (tagname=="NODE")
			{
				inside_par_node++;
				if (inside_par_node > 1)
				{
					section = attrs.value("name").toString().toStdString();
				}
			}

			if (inside_par_node && (tagname=="ITEM" || tagname=="ITEMLIST") )
			{
				pd.name = attrs.value("name").toString().toStdString();
				pd.description = attrs.value("description").toString().toStdString();
				pd.category = section;
				pd.hidden = false;
				if (attrs.hasAttribute("hidden"))
				{
					pd.hidden = (attrs.value("hidden").toString() == "true");
				}
				String type = attrs.value("type").toString().toStdString();
				bool is_list = 0;
				if (tagname=="ITEM")
				{
					if(type=="int")
					{
						pd.type=INT;
					}
					if (type=="float")
					{
						pd.type=DOUBLE;
					}
					if (type=="string")
					{
						if (attrs.hasAttribute("tags"))
						{
							String tags = attrs.value("tags").toString().toStdString();
							if (tags.hasSubstring("input file"))
							{
								pd.type=INFILE;
							}
							else if (tags.hasSubstring("output file"))
							{
								pd.type=OUTFILE;
							}
							else if (tags.hasSubstring("galaxy_opt_outdir"))
							{
								pd.type=GALAXY_OPT_OUTDIR;
							}
							else if (tags.hasSubstring("galaxy_opt_outid"))
							{
								pd.type=GALAXY_OPT_OUTID;
							}
							else
							{
								pd.type=STRING;
							}
						}
						else
						{
							pd.type=STRING;
						}
					}
				}
				else if (tagname=="ITEMLIST")
				{
					if (type=="int")
					{
						pd.type=INTLIST;
						is_list = 1;
					}
					if (type=="float")
					{
						pd.type=DOUBLELIST;
						is_list = 1;
					}
					if (type=="string")
					{
						if (attrs.hasAttribute("tags"))
						{
							String tags = attrs.value("tags").toString().toStdString();
							if (tags.hasSubstring("input file"))
							{
								pd.type=INFILELIST;
							}
							else if (tags.hasSubstring("output file"))
							{
								pd.type=OUTFILELIST;
							}
							else
							{
								pd.type=STRINGLIST;
							}
						}
						else
						{
							pd.type=STRINGLIST;
						}
						is_list = 1;
					}
				}

				if (attrs.hasAttribute("tags"))
				{
					String tags = attrs.value("tags").toString().toStdString();
					if (tags.hasSubstring("mandatory"))
					{
						pd.mandatory = true;
					}
					if (tags.hasSubstring("advanced"))
					{
						pd.advanced = true;
					}
					if (tags.hasSubstring("hidden"))
					{
						pd.hidden = true;
					}

				}

				if (attrs.hasAttribute("restrictions"))
				{
					String restr = attrs.value("restrictions").toString().toStdString();
					vector<String> v;
					String sep = ":";
					if (type=="string" || type=="stringlist") sep = ",";
					restr.splitQuoted(v, sep.c_str());
					for (Size i=0; i<v.size(); i++)
					{
						pd.allowed_values.push_back(v[i].trim());
					}
				}

				if (attrs.hasAttribute("supported_formats"))
				{
					String formats = attrs.value("supported_formats").toString().toStdString();
					vector<String> v;
					String sep = ",";
					formats.splitQuoted(v, sep.c_str());
					for (Size i=0; i<v.size(); i++)
					{
						pd.supported_formats.push_back(v[i].trim());
					}
				}

				if (attrs.hasAttribute("output_format_source"))
				{
					String source = attrs.value("output_format_source").toString().toStdString();
					pd.output_format_source = source;
				}

				if (!is_list)
				{
					String value = attrs.value("value").toString().toStdString();
					if(value!="")
					{
						values_current_par.push_back(value);
					}
				}
			}

			if (tagname=="LISTITEM")
			{
				String value = attrs.value("value").toString().toStdString();
				if(value!="")
				{
					values_current_par.push_back(value);
				}
			}
		}

		if(token == QXmlStreamReader::EndElement)
		{
			if(inside_par_node)
			{
				if(tagname=="NODE")
				{
					inside_par_node--;
				}
				else if(tagname=="ITEM" || tagname=="ITEMLIST")
				{
					map<String, ListIterator>::iterator search_it = description_map.find(pd.name);
					if (overwrite_existing)
					{
						if (search_it!=description_map.end())
						{
							descriptions.erase(search_it->second);
							description_map.erase(search_it);
						}
						descriptions.push_back(make_pair(pd.name, pd));
						description_map.insert(make_pair(pd.name, --descriptions.end()));
					}
					else if(search_it==description_map.end())
					{
						descriptions.push_back(make_pair(pd.name, pd));
						description_map.insert(make_pair(pd.name, --descriptions.end()));
					}

					if(values_current_par.size()>0 && (overwrite_existing || values.find(pd.name)==values.end()))
					{
						values.insert(make_pair(pd.name, values_current_par));
					}
					pd = ParameterDescription();
					values_current_par.clear();
				}
			}
		}
	}
	if(xmlIn_->hasError())
	{
		throw(Exception::ParseError(__FILE__, __LINE__,"ParamFile::readSection",String(xmlIn_->errorString().toLatin1().data())));
	}
}


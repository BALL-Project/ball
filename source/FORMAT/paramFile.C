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
	xmlOut_->writeAttribute("version", version.c_str());
	xmlOut_->writeAttribute("name", section_name.c_str());
	xmlOut_->writeAttribute("category", category.c_str());
	// left commented out because the attribute is optional anyway
	//xmlOut_->writeAttribute("docurl", "");

	xmlOut_->writeStartElement("description");
	xmlOut_->writeCharacters(section_description.c_str());
	xmlOut_->writeEndElement();

	xmlOut_->writeStartElement("manual");
	xmlOut_->writeCDATA(section_helptext.c_str());
	xmlOut_->writeEndElement();

	xmlOut_->writeStartElement("executableName");
	xmlOut_->writeCharacters(section_name.c_str());
	xmlOut_->writeEndElement();

	xmlOut_->writeStartElement("PARAMETERS");
	xmlOut_->writeAttribute("version","1.3");
	xmlOut_->writeStartElement("NODE");
	xmlOut_->writeAttribute("name","1");
	String s = "Instance '1' section for '"+section_name+"'";
	xmlOut_->writeAttribute("description",s.c_str());

	String previous_section = "";
	Size open_sections = 0;

	String string_array[2];

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

		String parameter_name = d_it->first.c_str();
		// only parameters with a category (section) need their name to be deconstructed
		if (!section.isEmpty())
		{
			Size n_elements = ParameterUtils::parseNestedParameterName(parameter_name, string_array);
			// sanity check... in case the parameter is not properly formatted, write some warnings and just move on
			if (n_elements != 2)
			{
				Log.warn() << "Parameter " << parameter_name << " has been registered under section " << section << " but is not properly formatted." << endl;
				Log.warn() << " Expected format is [section]:[parameter_name]";
			}
			else
			{
				// we do not need the category component at this point
				parameter_name = string_array[1];
			}
		}

		String type = "";
		bool is_list = false;
		String tag = "";
		if(d_it->second.type==INFILE)
		{
			type = "input-file";
		}
		else if(d_it->second.type==OUTFILE)
		{
			type = "output-file";
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
		else if(d_it->second.type==INFILELIST)
		{
			type = "input-file";
			is_list = true;
		}
		else if(d_it->second.type==OUTFILELIST)
		{
			type = "output-file";
			is_list = true;
		}
		else if(d_it->second.type==STRINGLIST)
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

		if(is_list)
		{
			xmlOut_->writeStartElement("ITEMLIST");
		}
		else
		{
			xmlOut_->writeStartElement("ITEM");
		}

		xmlOut_->writeAttribute("name", parameter_name.c_str());
		xmlOut_->writeAttribute("type", type.c_str());

		if (d_it->second.hidden)
		{
			if (tag != "")
			{
				tag += ", hidden";
			}
			else
			{
				tag += "hidden";
			}
		}

		xmlOut_->writeAttribute("description", d_it->second.description.c_str());

		if (d_it->second.mandatory)
		{
			xmlOut_->writeAttribute("required", "true");
		}

		if (d_it->second.advanced)
		{
			xmlOut_->writeAttribute("advanced", "true");
		}

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

		// no value has been set for this parameter
		if(value_it == values.end())
		{
			// write an empty default value only if
			// we are not handling a list (default value for a list is not supported)
			if (!is_list)
			{
				xmlOut_->writeAttribute("value","");
			}
		}
		else
		{
			if(!is_list)
			{
				xmlOut_->writeAttribute("value", value_it->second.front().c_str());
			}
			else
			{
				for(list<String>::const_iterator l_it=value_it->second.begin(); l_it!=value_it->second.end(); l_it++)
				{
					xmlOut_->writeStartElement("LISTITEM");
					xmlOut_->writeAttribute("value", l_it->c_str());
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


void ParamFile::readSection(String& tool_name, String& section_description, String& version,  String& section_helptext,
		                        String& category, list<pair<String,ParameterDescription> >& descriptions,
														map<String,list<String> >& values,bool overwrite_existing)
{
	if (!mode_read_)
	{
		throw BALL::Exception::GeneralException(__FILE__,__LINE__,"ParamFile::readSection() error","Reading is not possible, file is opened in write mode!");
	}

	tool_name = "";
	section_description = "";
	version = "";
	section_helptext = "";
	category = "";

	int inside_par_node = 0;
	String current_section = "";

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
			QXmlStreamAttributes attrs = xmlIn_->attributes();

			if (tagname == "tool")
			{
				tool_name = attrs.value("name").toString().toStdString();
				version = attrs.value("version").toString().toStdString();
				category = attrs.value("category").toString().toStdString();
			}

			if (tagname == "description")
			{
				section_description = xmlIn_->readElementText().toStdString();
			}

			if(tagname == "manual")
			{
				section_helptext = xmlIn_->readElementText().toStdString();
			}

			if (tagname=="NODE")
			{
				inside_par_node++;
				if (inside_par_node > 1)
				{
					current_section = attrs.value("name").toString().toStdString();
				}
			}

			if (inside_par_node && (tagname=="ITEM" || tagname=="ITEMLIST") )
			{
				// ITEM and ITEMLIST elements have the "tags" attribute; extract the tags
				std::set<String> tags = getTags(attrs);

				pd.name = attrs.value("name").toString().toStdString();
				if (!current_section.isEmpty() && current_section != "1")
				{
					// we are reading a nested parameter and we need to format it like [section]:[parameter]
					// unless we are handling the top section ("conveniently" named "1")
					pd.name = ParameterUtils::buildNestedParameterName(current_section, pd.name);
				}

				pd.description = attrs.value("description").toString().toStdString();
				pd.category = "Bioinformatics";

				if (attrs.hasAttribute("category"))
				{
					pd.category = attrs.value("category").toString().toStdString();
				}

				String type = attrs.value("type").toString().toStdString();
				bool is_list = false;
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
						pd.type=STRING;
						if (tags.find("galaxy_opt_outdir") != tags.end())
						{
							pd.type=GALAXY_OPT_OUTDIR;
						}
						else if (tags.find("galaxy_opt_outid") != tags.end())
						{
							pd.type=GALAXY_OPT_OUTID;
						}
					}
				}
				else if (tagname=="ITEMLIST")
				{
					// we know it's a list, let's see what kind of list we're dealing with
					is_list = true;

					if (type=="int")
					{
						pd.type=INTLIST;						
					}
					else if (type=="float" || type=="double")
					{
						pd.type=DOUBLELIST;
					}
					else if (type=="input-file")
					{
						pd.type=INFILELIST;
					}
					else if (type=="output-file")
					{
						pd.type=OUTFILELIST;
					}
					else if (type=="string")
					{
						pd.type=STRINGLIST;
					}
					else
					{
						throw BALL::Exception::GeneralException(__FILE__, __LINE__,
																"ParamFile::readSection() error",
																"Parameter type " + type + " not recognized");
					}
				}

				pd.mandatory = false;
				if (attrs.hasAttribute("required"))
				{
					String required = attrs.value("required").toString().toStdString();
					required.trim().toLower();
					if (required == "true")
					{
						pd.mandatory = true;
					}
				}

				pd.advanced = false;
				if (attrs.hasAttribute("advanced"))
				{
					String advanced = attrs.value("advanced").toString().toStdString();
					advanced.trim().toLower();
					if (advanced == "true")
					{
						pd.advanced = true;
					}
				}

				pd.hidden = false;
				if (tags.find("hidden") != tags.end())
				{
					pd.hidden = true;
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

std::set<String> ParamFile::getTags(QXmlStreamAttributes& attributes)
{
	set<String> tagSet;

	if (attributes.hasAttribute("tags"))
	{
		String tagsAttributeValue = attributes.value("tags").toString().toStdString();
		if (tagsAttributeValue != "")
		{
			// separate the value of the attribute by commas
			std::vector<String> tags;
			int nTags = tagsAttributeValue.split(tags, ",");
			for (int i = 0; i < nTags; i++)
			{
				tagSet.insert(tags[i].trim());
			}
		}
	}

	return tagSet;
}


String ParameterUtils::buildNestedParameterName(const String& category, const String& parameter_name)
{
	return category + ":" + parameter_name;
}


Size ParameterUtils::parseNestedParameterName(const String& parameter_name, String string_array[])
{
	return parameter_name.split(string_array, 2, ":");
}

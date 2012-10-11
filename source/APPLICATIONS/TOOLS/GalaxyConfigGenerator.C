/// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/KERNEL/molecule.h>
#include <BALL/FORMAT/commandlineParser.h>
#include "version.h"

using namespace BALL;
using namespace std;

int main(int argc, char* argv[])
{
	CommandlineParser parpars("GalaxyConfigGenerator", "generate galaxy tool-config files", VERSION, String(__DATE__));
	parpars.registerParameter("i", "input par-file", INFILE, true);
	parpars.registerParameter("o", "output xml-file", OUTFILE, true);
	parpars.registerParameter("ignore", "tool-parameter to be ignored, i.e. those for which no galaxy config entry should be created", STRINGLIST);
	parpars.registerParameter("s", "suite_config xml-file", OUTFILE);
	String man = "This tool can be used to create galaxy tool-configuration files.\nAs input, please supply a parameter-file as written by '-write_par' by all of our tools.";
	parpars.setToolManual(man);
	parpars.setSupportedFormats("o","xml");
	parpars.setSupportedFormats("s", "xml");
	parpars.setSupportedFormats("i", "xml");
	parpars.parse(argc, argv);

	// Use this map to store old galaxy tool-keys.
	// If a tool was renamed, the key should still be the same! Otherwise executing old workflows will not work any more.
	map<String,String> old_toolkeys;
	old_toolkeys["IMGDock"] = "imeedydock";


	ParamFile paramfile(parpars.get("i"), ios::in);

	String tool_name;
	String tool_description;
	String tool_version;
	String manual;
	String category;
	list<pair<String,ParameterDescription> > parameter_descriptions;
	typedef list<pair<String,ParameterDescription> >::iterator ListIterator;
	map<String, ListIterator> parameter_map;
	map<String,list<String> > parameter_values;

	set<String> parameters_to_ignore;
	if (parpars.has("ignore"))
	{
		list<String> ignore_list = parpars.getList("ignore");
		for (list<String>::iterator it = ignore_list.begin(); it != ignore_list.end(); it++)
		{
			parameters_to_ignore.insert(*it);
		}
	}

	paramfile.readSection(tool_name, tool_description, tool_version, manual, category, parameter_descriptions, parameter_values);

	for (list<pair<String,ParameterDescription> >::iterator it=parameter_descriptions.begin();
		it!=parameter_descriptions.end(); it++)
	{
		parameter_map.insert(make_pair(it->first, it));
	}

	// write tool-name, id, short description to tool_suite.xml file (if specified)
	if (parpars.has("s"))
	{
		QFile file(parpars.get("s").c_str());
		if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
		{
			cerr << "[Error:] the specified suite-file cannot be opened!" << endl;
			exit(1);
		}
		QXmlStreamWriter xml(&file);
		xml.setAutoFormatting(true);
		xml.writeStartElement("tool");
		String s(tool_name.c_str());
		s.toLower();
		map<String,String>::iterator search_it = old_toolkeys.find(tool_name);
		if (search_it == old_toolkeys.end())
		{
			xml.writeAttribute("id",s.c_str());
		}
		else
		{
			xml.writeAttribute("id",search_it->second.c_str());
		}
		xml.writeAttribute("name", tool_name.c_str());
		xml.writeAttribute("version", tool_version.c_str());
		xml.writeStartElement("description");
		xml.writeCharacters(tool_description.trim().c_str());
		xml.writeEndElement();
		xml.writeEndElement();
	}

	QFile file(parpars.get("o").c_str());
	if(!file.open(QIODevice::WriteOnly| QIODevice::Text))
	{
		cerr << "[Error:] the specified output-file can not be created!" << endl;
		exit(1);
	}

	// Before we write stuff, we need to separate the output files from the input-options.
	list<String> outputs;
	list<String> inputs;
	list<String> input_files;
	list<String> input_parameters;
	String command = tool_name;
	command += " \n";
	for (list<pair<String,ParameterDescription> >::iterator it=parameter_descriptions.begin();
		it!=parameter_descriptions.end(); it++)
	{
		if (it->first == "par" || it->first == "write_par" || it->first == "help" || it->first == "write_ini" || (tool_name=="PocketDetector" && it->first=="mol_out") || parameters_to_ignore.find(it->first) != parameters_to_ignore.end() || it->second.advanced)
		{
			continue;
		}

		// Galaxy does not support a dynamic number of output files, yet.
		// Anne: Au contraire: see BondOrderAssigner
		if (tool_name == "LigandFileSplitter" && it->first == "o")
		{
			// use a pre-set number of output-files, i.e. 15
			list<String> l;
			l.push_back("15");
			parameter_values["no"] = l;
			for (Size i=2; i < 16; i++)
			{
				ParameterDescription desc2 = it->second;
				desc2.name = String("o") + String(i);
				parameter_descriptions.push_back(make_pair(desc2.name, desc2));
				list<pair<String,ParameterDescription> >::iterator it2 = parameter_descriptions.end();
				it2--;
				parameter_map.insert(make_pair(desc2.name, it2));
			}
			manual += "\n\nSince Galaxy does not support a flexible number of output-files, we will create 15 output files when you use LigandFileSplitter in Galaxy.";
		}

		if (it->second.type == OUTFILE || it->second.type == OUTFILELIST)
		{
			outputs.push_back(it->first);
		}
		else if(it->second.name != "quiet")
		{
			if (it->second.type == INFILE || it->second.type == INFILELIST)
			{
				input_files.push_back(it->first);
			}
			else
			{
				input_parameters.push_back(it->first);
			}
		}

		bool is_flag = false;
		if (it->second.allowed_values.size() == 2)
		{
			list<String>::iterator sit = it->second.allowed_values.begin();
			if (*sit == "0")
			{
				sit++;
				if (*sit == "1") is_flag = true;
			}
		}

		if (it->second.name != "quiet")
		{
			if (it->second.type != INFILELIST)
			{
				command += "#if str( $";
				command += it->first + String(" ) != '' ");
				command += String(" and str( $") + it->first + String(" ) != 'None' :\n");
				if (!is_flag)
				{
					String par_name = it->first;
					if (tool_name == "LigandFileSplitter" && par_name.hasPrefix("o"))
					{
						par_name = "o";
					}
					command += "   -" + par_name + " \"$" + it->first + "\"\n";
				}
				else
				{
					command += "   -" + it->first + "\n";
				}
				command += "#end if\n";
			}
			else
			{
				command += "#for $s in $series_i \n";
				command += "   #if str( \"";
				String file = String("${s.") + it->first + String(".file_name}");
				command += file + String("\" ) != '' ");
				command += String(" and str( \"") + file + String("\" ) != 'None' :\n");
				command += "      -" + it->first + " \"" + file + "\"\n";
				command += "   #end if\n";
				command += "#end for \n";
			}
		}
		else
		{
			// If printing of progress-information can be disabled by '-quiet',
			// then always use this flag in galaxy, since the progress-information would
			// otherwise not be visible to the user anyway.
			command += "-quiet \n";
		}
	}

	// If the job does not fail, Galaxy will only display a few lines of the tool's output, so make sure to show the relevant (i.e. the last) lines.
	command += " | tail -n 5\n";

	// Sort inputs in such a way that input-files come first, and other input-parameters later.
	// It just looks nicer in the webinterface ...
	inputs.splice(inputs.begin(), input_parameters);
	inputs.splice(inputs.begin(), input_files);

	QXmlStreamWriter xml(&file);
	xml.setAutoFormatting(true);

	/// Write header
	xml.writeComment("This is a configuration file for the integration of a BALL tools into Galaxy (http://usegalaxy.org). This file was automatically generated using BALL's GalaxyConfigGenerator, so do not bother to make too many manual modifications.");
	xml.writeComment((String("Proposed Tool Section: [") + category + "]").c_str());
	xml.writeStartElement("tool");
	map<String,String>::iterator search_it = old_toolkeys.find(tool_name);
	if (search_it == old_toolkeys.end())
	{
		String s(tool_name.c_str());
		s.toLower();
		xml.writeAttribute("id",s.c_str());
	}
	else xml.writeAttribute("id",search_it->second.c_str());
	xml.writeAttribute("name",tool_name.c_str());
	xml.writeAttribute("version",tool_version.c_str());
	xml.writeStartElement("requirements");
	xml.writeStartElement("requirement");
	xml.writeAttribute("type", "package");
	xml.writeCharacters("ballaxy");
	xml.writeEndElement();
	xml.writeEndElement();
	xml.writeStartElement("description");
	xml.writeCharacters(tool_description.c_str());
	xml.writeEndElement();
	xml.writeStartElement("command");
//	xml.writeAttribute("interpreter","bash");
	xml.writeCDATA(command.c_str()); // Do not use writeCharacters() here, because it would excape the "-" signs within the repeat-sections, which chetah needs!
	xml.writeEndElement();

	/// Write input section
	xml.writeStartElement("inputs");
	for (list<String>::iterator it = inputs.begin(); it != inputs.end(); it++)
	{
		ParameterDescription& desc = parameter_map.find(*it)->second->second;
		if (desc.hidden)
			continue;

		// Write repeat section for input-file lists
		if (desc.type == INFILELIST)
		{
			xml.writeStartElement("repeat");
			String s = String("series_") + desc.name;
			xml.writeAttribute("name", s.c_str());
			xml.writeAttribute("title", desc.description.c_str());
			xml.writeAttribute("min", "1");
		//	desc.name = "";
		}

		xml.writeStartElement("param");
		xml.writeAttribute("name",desc.name.c_str());

		bool is_flag = false;
		if (desc.type == BALL::INT && desc.allowed_values.size() == 2)
		{
			list<String>::iterator sit = desc.allowed_values.begin();
			if (*sit == "0")
			{
				sit++;
				if (*sit == "1") is_flag = true;
			}
		}

		String label = desc.description;
		if (!desc.mandatory)
		{
			xml.writeAttribute("optional", "true");
			if (desc.type != INFILE && desc.type != INFILELIST && !is_flag &&
			(desc.allowed_values.empty() || desc.type != STRING))
			{
				label = "Optional: " + label;
			}
		}
		else
		{
			xml.writeAttribute("optional", "false");
		}
		xml.writeAttribute("label",label.c_str());

		// No need to distinguish between input-files and input-filelists here, since 'repeat' tags have already been added
		if (desc.type == INFILE || desc.type == INFILELIST)
		{
			xml.writeAttribute("type","data");

			// Use only the first format, since galaxy (currently) does not support
			// more than one format for each input-file.
			// However, if the input is a molecule-file, set format to 'GenericMolFile'.
			String formats = "";
			bool has_mol2 = false;
			bool has_sdf = false;
			bool has_drf = false;
			for (list<String>::iterator sit = desc.supported_formats.begin();
				sit != desc.supported_formats.end(); sit++)
			{
				if (sit == desc.supported_formats.begin()) formats = *sit;
				if (*sit == "mol2") has_mol2 = true;
				else if (*sit == "sdf") has_sdf = true;
				else if (*sit == "drf") has_drf = true;
			}
			if (has_mol2 && has_sdf && has_drf) formats = "mol2/sdf/drf";
			if (formats == "") formats = "data";
			xml.writeAttribute("format",formats.c_str());
		}
		else if (is_flag)
		{
			xml.writeAttribute("type","boolean");
			xml.writeAttribute("truevalue","true");
			xml.writeAttribute("falsevalue","");

			// Set default to 'checked' if flag was given
			map<String,list<String> >::iterator search_it = parameter_values.find(desc.name);
			if (search_it != parameter_values.end() && !search_it->second.empty())
			{
				if (search_it->second.front() == "true" || search_it->second.front() == "1")
				{
					xml.writeAttribute("checked", "true");
				}
			}
		}
		else // no flag
		{
			// Disable int and double at the moment, because Galaxy always requires input for those types, even if they are marked as optional.
			if (tool_name == "LigandFileSplitter" && desc.name == "no")
			{
				xml.writeAttribute("type", "hidden");
			}
			else if (desc.type == BALL::INT)
			{
				xml.writeAttribute("type","text");
			}
			else if (desc.type == DOUBLE)
			{
				xml.writeAttribute("type","text");
			}

			if (desc.allowed_values.empty())
			{
				xml.writeAttribute("area","true");
				if (desc.type == STRING || desc.type == STRINGLIST)
				{
					xml.writeAttribute("type","text");
					xml.writeAttribute("size","1x15");
				}
				else xml.writeAttribute("size","1x5");
			}
			else
			{
				if (desc.type == STRING || desc.allowed_values.size() != 2)
				{
					xml.writeAttribute("type","select");
					for (list<String>::iterator sit = desc.allowed_values.begin();
						sit != desc.allowed_values.end(); sit++)
					{
						xml.writeStartElement("option");
						xml.writeAttribute("value",sit->c_str());
						xml.writeCharacters(sit->c_str());
						xml.writeEndElement();
					}
				}
				else // int or double with min-/max-restrictions
				{
					list<String>::iterator sit = desc.allowed_values.begin();
					xml.writeAttribute("min", sit->c_str());
					sit++;
					xml.writeAttribute("max", sit->c_str());
					xml.writeAttribute("area","true");
					xml.writeAttribute("size","1x5");
				}
			}

			// Write default-value (if any)
			map<String,list<String> >::iterator search_it = parameter_values.find(desc.name);
			if (search_it != parameter_values.end() && !search_it->second.empty())
			{
				xml.writeAttribute("value", search_it->second.front().c_str());
			}
			else if (desc.type == BALL::INT || desc.type == DOUBLE)
			{
				xml.writeAttribute("value", "");
			}
		}
		xml.writeEndElement();

		if (desc.type == INFILELIST) // Close repeat section for input-file lists
		{
			xml.writeEndElement();
		}
	}
	xml.writeEndElement();


	/// Write output section
	xml.writeStartElement("outputs");
	for (list<String>::iterator it = outputs.begin(); it != outputs.end(); it++)
	{
		ParameterDescription& desc = parameter_map.find(*it)->second->second;
		xml.writeStartElement("data");
		xml.writeAttribute("name",desc.name.c_str());

		// Use only the first format, since galaxy (currently) does not support more than one format for each output-file
		String formats = "";
		bool has_mol2 = false;
		bool has_sdf = false;
		bool has_drf = false;
		for (list<String>::iterator sit = desc.supported_formats.begin();
			sit != desc.supported_formats.end(); sit++)
		{
			if (sit == desc.supported_formats.begin()) formats = *sit;
			if (*sit == "mol2") has_mol2 = true;
			else if (*sit == "sdf") has_sdf = true;
			else if (*sit == "drf") has_drf = true;
		}
		if (formats == "") formats = "data";
		if (has_mol2 && has_sdf && has_drf) formats = "mol2/sdf/drf";
		xml.writeAttribute("format",formats.c_str());
		if (desc.output_format_source != "")
		{
			xml.writeAttribute("format_source", desc.output_format_source.c_str());
		}

		if (tool_name == "Converter" && desc.name == "o")
		{
			ParameterDescription& desc = parameter_map.find("i")->second->second;
			xml.writeStartElement("change_format");
			for (list<String>::iterator sit = desc.supported_formats.begin();
				sit != desc.supported_formats.end(); sit++)
			{
				xml.writeStartElement("when");
				xml.writeAttribute("input", "f");
				xml.writeAttribute("value", sit->c_str());
				xml.writeAttribute("format", sit->c_str());
				xml.writeEndElement();
			}
			xml.writeEndElement();
		}

		xml.writeEndElement();
	}

	xml.writeEndElement();

	/// Write help section
	xml.writeStartElement("help");
	xml.writeCharacters(manual.c_str());
	xml.writeEndElement();

	/// Close everything properly
	xml.writeEndElement();
	file.close();

	Log.level(20) << "Your galaxy tool-configuration file has been written to \"" << parpars.get("o") <<"\". Goodbye." << endl << endl;
}

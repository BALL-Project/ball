/// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_FORMAT_PARAMFILE_H
#define BALL_FORMAT_PARAMFILE_H

#ifndef BALL_SYSTEM_FILE_H
# include <BALL/SYSTEM/file.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
# include <BALL/DATATYPE/string.h>
#endif

#include <QtXml/QXmlStreamReader>
#include <QtXml/QXmlStreamWriter>
#include <QtCore/QFile>

#include <map>
#include <list>


namespace BALL
{
	enum BALL_EXPORT ParameterType
	{
		INFILE,
		OUTFILE,
		STRING,
		INT,
		DOUBLE,
		INFILELIST,
		OUTFILELIST,
		STRINGLIST,
		INTLIST,
		DOUBLELIST,
		GALAXY_OPT_OUTDIR,
		GALAXY_OPT_OUTID
	};

	struct BALL_EXPORT ParameterDescription
	{
		// constructor
		ParameterDescription()
		{
			name = "";
			description = "";
			category = "";
			mandatory = false;
			advanced = false;
			type = INFILE;
			output_format_source = "";
			allowed_values.clear();
			supported_formats.clear();
			hidden = false;
		};

		String name;
		String description;
		String category;
		bool mandatory;
		bool advanced;
		ParameterType type;

		/** In case of ouput-files, this variable allows to specify the name of an input-parameter, whose format should be used as ouput-format. \n
		Note, that this variable will be used only for the creation of config-files for workflow-systems (Galaxy, Knime, etc.) and not for the command-line interface, where the user will directly specify output-filenames. */
		String output_format_source;

		/** If this list is empty, then there are no restrictions on the value of the parameter */
		list<String> allowed_values;

		/** In case of input-/output-files, this list should contain the supported file-extensions. \n
		If the list is empty, no format restrictions are set. */
		list<String> supported_formats;

		// if parameters shall be hidden in galaxy
		bool hidden;
	};

	/** Class for storing and retrieving parameters (e.g. tool-parameters) to an xml-based file */
	class BALL_EXPORT ParamFile : public File
	{
		public:

			ParamFile(const String& name, File::OpenMode open_mode);
			~ParamFile();

			/** Write a section to the output file (e.g. parameters for one tool)
			@param section_description a *short* description of this section (e.g. the tool), containing only a few words
			@param section_helptext a longer help-text/detailed description can be specified here
			@param category the category into which the tool falls (e.g. Docking, QSAR, etc.)
			@param descriptions description for all parameters to be saved
			@param value values of all parameters to be saved */
			void writeSection(String section_name, String section_description, String version, const String& section_helptext,
					              const String& category, const std::list<std::pair<String,ParameterDescription> >& descriptions,
												const std::map<String,list<String> >& values);

			/** Read a section from input file (e.g. parameters for one tool)
			@param descriptions descriptions of parameters will be stored here 
			@param values values of parameters will be stored here
			@param overwrite_existing if set to true, entries already existing in 'descriptions' and 'values' will be overwritten. */
			void readSection(String& section_name, String& section_description, String& version, String& section_helptext,
					             String& category, std::list<std::pair<String, ParameterDescription> >& descriptions,
											 std::map<String,list<String> >& values,
											 bool overwrite_existing=false);

			void close();


		protected:

			String filename_;
			File::OpenMode open_mode_;
			bool mode_read_;
			QXmlStreamReader* xmlIn_;
			QXmlStreamWriter* xmlOut_;
			QFile* file_;
	};
}

#endif

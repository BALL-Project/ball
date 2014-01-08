#ifndef BALL_FORMAT_COMMANDLINEPARSER_H
#define BALL_FORMAT_COMMANDLINEPARSER_H

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
# include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_FORMAT_PARAMFILE_H
# include <BALL/FORMAT/paramFile.h>
#endif

#include <map>
#include <set>

namespace BALL
{
	/** Class for parsing parameters specified on the command line.\n

			Register the parameters and flags of your tool with registerParameter(), 
			resp. registerFlag() and have the command-line parsed with parse(). 
			After this, you can retrieve the values of all parameters with get().\n

			In addition to this, you can also register a tool-manual text, define 
			restrictions on parameters and register file-formats that are supported 
			for in- or output-files.\n
			
			If the parameter '-write_par somefilename.xml' is given to the program, 
			an xml-based parameter-file will be automatically written, containing the 
			specification the tool, all its parameters and all parameter values used 
			on the commend-line.\n
			
			If the parameter '-par somefilename.xml' is used, such a file can be read, 
			so that the parameter-values stored in it will be used. 
			However, if, although this is not necessary, parameters (other than -par) 
			are specified on the command-line, their values will automatically overload 
			those stored in the xml-file. 

			The parameter '-env' allows to differentiate between command line ('cmdline') 
			behaviour and special adaptions to workflow systems like galaxy or knime.
	*/
	class BALL_EXPORT CommandlineParser
	{
		public:
			CommandlineParser(String tool_name, String tool_description, String tool_version, String build_date, String category="");

			/** Set the text to be displayed as mini-manual when starting the tool, containing a few sentences describing what the tool should be good for ... */
			void setToolManual(const String& manual);

			static const String NOT_FOUND;
			static const list<String> EMTPY_LIST;

			// - CLI switch
			// - description
			// - parameter type
			// - required
			// - default value
			// - hidden in galaxy
			void registerParameter(String name, String description, ParameterType type, bool mandatory = false, String default_value = "", bool hidden = false);

			void registerFlag(String name, String description, bool default_gui_value = false, bool hidden = false);

			void registerAdvancedParameters(Options& options);

			// exclude parameter from showing in help mode e.g. galaxy specific multi-output parameter
			void setParameterAsAdvanced(String name);

			/** Register the minimal and maximal allowed value for a numerical parameter. */
			void setParameterRestrictions(String par_name, double min_value, double max_value);

			/** Register the allowed values for a string-parameter. */
			void setParameterRestrictions(String par_name, list<String>& allowed_values);

			/** In case of output-files, this functions allows to specify the name 
			    of an input-parameter, whose format should be used for the given 
			    output-parameter. \n
					
					Note, that this function will only have an effect on the creation of 
					config-files for workflow-systems (Galaxy, Knime, etc.) and not for 
					the command-line interface, where the user will directly specify 
					output-filenames. 
			*/
			void setOutputFormatSource(String output_parname, String input_parname);

			/** Register the supported formats for input-/output-files.
			  
					@param supported_formats supported file-formats separated by commas 
			*/
			void setSupportedFormats(String par_name, String supported_formats);

			/** Parse all parameters found in the command line 
			 */
			void parse(int argc, char* argv[]);

			/** Copies names and values of all advanced parameters to the given Options object. 
			*/
			void copyAdvancedParametersToOptions(Options& options);

			/** Returns the value for a given parameter name. \n
					
					Example: For "-o outfile.sdf", CommandlineParser::get("o") will return 
					"outfile.sdf". 
			*/
			const String& get(String name);

			/** Find out whether a the parameter or flag has been specified on the command-line. 
			 */
			bool has(String name);

			/** Returns a parameter-list for a given parameter name. \n
				
					Example: For "-i infile1.sdf infile2.sdf", CommandlineParser::getList("i") 
					will return a vector containing 'infile1.sdf' and infile2.sdf'. */
			const list<String>& getList(String name);

			/** Print information about about all registered parameters.\n
			  
					If 'parameter_names' is specified, only information those parameters is shown. 
			*/
			void printHelp(const std::set<String>& parameter_names = std::set<String>(), bool show_manual = true);

			void printToolInfo();

			const String& getStartTime();

			/** Get the exact command with which the tool was started on the command-line. 
			*/
			const String& getStartCommand();

		private:

			void replaceEscapedCharacters_(String& parameter_value);

			void checkAndRegisterParameter(String name, String description, ParameterType type, bool mandatory = false, String default_value = "", bool perform_check = true, bool hidden=false);

			void checkAndRegisterFlag(String name, String description, bool default_gui_value = false, bool perform_check = true, bool hidden=false);


			void validateRegisteredFilesHaveFormats();

			/** Throws an exception if the given parameter name is not allowed to be used. */
			void checkParameterName(const String& name, const bool perform_check = true);

			/** blacklisted param names */
			std::set<String> reserved_params_;

			/** map escaped characters to the original characters */
			std::list<std::pair<String, String> > escaped_chars_;
			std::map<String, list<String> > parameter_map_;
			std::map<String, ParameterDescription> registered_parameters_;

			std::map<String, ParameterDescription> registered_flags_;
			typedef std::map<String, ParameterDescription>::iterator MapIterator;
			std::list<MapIterator> original_parameter_order_;
			std::list<MapIterator> original_flag_order_;
			Size max_parname_length_;
			Size max_flagname_length_;
			String tool_name_;
			String tool_description_;
			String tool_version_;
			String build_date_;
			String hostname_;
			String start_time_;
			String tool_manual_;
			String start_command_;
			String tool_category_;
	};
}

#endif // BALL_FORMAT_COMMANDLINEPARSER_H

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: CIFFile.h,v 1.2 2007/08/22 09:59:45 anne Exp $
//

#ifndef BALL_FORMAT_CIFFILE_H
#define BALL_FORMAT_CIFFILE_H

#ifndef BALL_SYSTEM_FILE_H
#	include <BALL/SYSTEM/file.h>
#endif

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
# include <BALL/DATATYPE/stringHashMap.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
#	include <BALL/KERNEL/molecule.h>
#endif

#define CIFPARSER_LINE_LENGTH 2550

namespace BALL
{
	/** CIF file class.
	 * 	This class enables BALL to read and write input and output CIF files for
	 * 	the NMR shift application.
	 *
	 * 	\ingroup StructureFormats
	 */
	class BALL_EXPORT CIFFile
		: public File
	{
		public:
			BALL_CREATE(CIFFile)
	
			/** Nested class for the dataitems of the CIFFile **/
			class Item
			{
				public:

					/** Acessors**/
					void clear();
					void addPair(String key, String value);
					void startLoop();
					void addTag(String tag);
					void addValue(String value);
					
					/* a data item may be a tag-value pair or a table */
					bool is_loop;

					/** Stores all loop data indices **/						
					//StringHashMap< Index > data;
					/** Stores all raw loop data **/
					std::vector<String> keys;
					/** Stores the loop data per line **/
					std::vector<std::vector<String> > values;

					/** Stores the tag-value pair in key - value format **/
					std::pair<String, String> entry;
					
					void operator >> (std::ostream& os) const;

			};
			
			/** Nested class for the saveframes of the CIFFile **/
			class SaveFrame
			{
				public:
					
					/** The name of this saveframe **/
					String framename;
					
					/** Stores all dataitems of the saveframe  **/ 
					std::vector<Item> items; 

					void clear();
					void start(String name); 
					void addDataItem(Item item);

					void operator >> (std::ostream& os) const;

			};

			/** Nested class for the data entity of the CIFFile **/
			class Datacontent
			{
				public:
					
					/** Constructors **/
					Datacontent();
					Datacontent(SaveFrame new_saveframe);
					Datacontent(Item new_item);

					/** all data of a datablock is either a saveframe or a single item **/
					bool is_saveframe;
		
					/** Stores the saveframe **/
					SaveFrame sframe;

					/** Stores the item **/
					Item dataitem;

					void operator >> (std::ostream& os) const;

			};
			
			/** Nested class for the datablock entity of the CIFFile **/
			class Datablock
			{
				public:

					/** The name of this saveframe **/
					String name;

					/** Stores the datablock content **/
					std::vector<Datacontent> data;
	
					/** Clears the Datablock**/
					void clear();
					
					/** Sets the name of a new Datablock **/
					void start(String blockname);

					void insertDatacontent(Datacontent content);
					void insertDatacontent(const SaveFrame& new_saveframe);
					void insertDatacontent(const Item item);
					
					/** Sets the name of a Datablock **/
					void setName(String blockname);

					void operator >> (std::ostream& os) const;
			};

			/** State of the parser **/
			struct State
			{
				CIFFile* current_parser;
			};
			
			/** @name Constructors and Destructors
			 */
			//@{
			
			/** Default constructor
			 */
			CIFFile();

			/** Copy constructor
			 */
			CIFFile(const CIFFile& file)
				throw(Exception::FileNotFound);

			/** Detailed constructor
			 */
			CIFFile(const String& filename, File::OpenMode open_mode = std::ios::in)
				throw(Exception::FileNotFound);

			/** Destructor
			 */
			virtual ~CIFFile();
			
			//@}
			/** @name Assignment.
			 */
			//@{
			
			/** Assignment operator.
			 */

			const CIFFile& operator = (const CIFFile& rhs)
				throw(Exception::FileNotFound);

			//@}
			
			/** @name Reading and Writing of Crytallsographic Information
			 */
			//@{
			
			/** Write a CIFFile.
			*/
			virtual bool write()
				throw(File::CannotWrite);

			/** Read a CIFFile.
			 */
			virtual bool read()
				throw(Exception::ParseError);
			
			//@}
			/** @name Accessors
			 */
			//@{
			void inSafeframe(const char* name);
			
			void inItem(const char* name);
			
			void inDatablock(const char* name);

			void insertDatablock(const Datablock& datablock);

			const Datablock getDatablock(const String& name);

			void setMolecule(Molecule* molecule);
		
			void clearParameters();
			//@}

			static State state;

		protected:
			virtual void initRead_();

			Molecule*	molecule_;
			
			String current_datablock_;
			String current_saveframe_;
			String current_item_;
			
			StringHashMap<Datablock> datablocks_;
	};	
}
#endif // BALL_FORMAT_CIFFILE_H

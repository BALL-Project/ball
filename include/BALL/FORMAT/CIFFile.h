// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
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

#include <map>

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
	
			/** Nested class for the dataitems of the CIFFile **/
			class Item
			{
				public:

					Item();
					
					Item(const Item& item);

					virtual ~Item();

					/** Acessors **/
					void clear();
					void addPair(String key, String value);
					void startLoop();
					void addTag(String tag);
					void addValue(String value);
				
					/**  Returns the index of the Item identifier key. If there is no such key, -1 is returned. **/
					Index getKeyIndex(String key);
					
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
					
					std::ostream& operator >> (std::ostream& os) const;

			};
			
			/** Nested class for the saveframes of the CIFFile **/
			class SaveFrame
			{
				public:
					
					void clear();
					void start(String name); 
					void addDataItem(Item item);
					void setCategory(String cat) {category = cat;};
					
					std::ostream& operator >> (std::ostream& os) const;
					const String& getCategory() const {return category;};
					
					/** Returns a dummy Dataitem, if no item with such name exists **/
					const Item&  getDataItem(const String& item_name) const;
					// we allow manipulation
					Item&  getDataItem(const String& item_name);

					const String& getDataItemValue(const String& item_name) const;
					// we allow manipulation
					String&  getDataItemValue(const String& item_name);



					bool hasItem (const String& item_name) const;
					String getItemValue(const String& name) const;

					
					/** The name of this saveframe **/
					String framename;	
					String category; // always first_item.entry.second

					/** Stores all dataitems of the saveframe  **/ 
					std::vector<Item> items; 
					StringHashMap<Index> pair_items;
					
					Item dummy_data_item_; 

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
					SaveFrame saveframe;

					/** Stores the item **/
					Item dataitem;

					std::ostream& operator >> (std::ostream& os) const;

			};
			
			/** Nested class for the datablock entity of the CIFFile **/
			class Datablock
			{
				public:
					/** Clears the Datablock **/
					void clear();
					
					/** Sets the name of a new Datablock **/
					void start(String blockname);

					void insertDatacontent(const Datacontent& content);
					void insertDatacontent(const SaveFrame& new_saveframe);
					void insertDatacontent(const Item& item);

					const Item&  getDataItem(const String& item_name) const;
					Item&  getDataItem(const String& item_name);
					
					/** Returns the saveframe of category name.
					 */
					const std::vector<Index> getSaveframeIndicesByCategory(const String& name) const;
					std::vector<Index> getSaveframeIndicesByCategory(const String& name);
					/* Returns copies of saveframes of category name */
					std::vector<SaveFrame> getSaveframesByCategory(const String& name) const;

					
					/** Returns the saveframe labeled 'name'.
					 */
					const SaveFrame& getSaveframeByName(const String& name) const;
					SaveFrame& getSaveframeByName(const String& name);
			
					const SaveFrame& getSaveframeByIndex(const Index index) const;
					SaveFrame& getSaveframeByIndex(const Index index);


					/**	Returns true if the datablock contains a Saveframe of category name.
					*/
					// 
					bool hasSaveframeName(const String& name) const;
					bool hasSaveframeCategory(const String& name) const;
					bool hasItem(const String& name) const;

					/** Sets the name of a Datablock **/
					void setName(String blockname);

					std::ostream& operator >> (std::ostream& os) const;
					//////////////////////////////////////////////////
					/** The name of this saveframe **/
					String name;

					/** Stores the datablock content **/
					std::vector<Datacontent> data;
					
					/** Hashes the saveframes - we assume, that they are unique**/
					StringHashMap<Index> saveframe_names;
					//StringHashMap<std::vector<Index> > saveframe_categories;
					std::multimap<String, Index> saveframe_categories;

					/** Hashes the items*/
					StringHashMap<Index> item_names; 

					Item dummy_data_item_; 
					SaveFrame dummy_saveframe_; 
					vector<SaveFrame> dummy_saveframes_; 
					vector<Index> dummy_indices_; 

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

			/** Detailed constructor
			 *  @throw Exception::FileNotFound if the file could not be opened
			 */
			CIFFile(const String& filename, File::OpenMode open_mode = std::ios::in);

			/** Destructor
			 */
			virtual ~CIFFile();
			
			//@}

			/** @name Reading and Writing of Crytallographic Information
			 */
			//@{
			
			/** Write a CIFFile.
			 *  @throw File::CannotWrite if writing to the file failed
			 */
			virtual bool write();

			/** Read a CIFFile.
			 *  @throw Exception::ParseError if a syntax error was encountered
			 */
			virtual bool read();
			
			//@}
			/** @name Accessors
			 */
			//@{
			void inSafeframe(const char* name);
			
			void inItem(const char* name);
			
			void inDatablock(const char* name);

			void insertDatablock(const Datablock& datablock);
			
			const Datablock& getDatablock(const String& name) const;

		
			// we allow manipulation :-)
			Datablock& getDatablock(const String& name); 

			bool hasDatablock(const String& name) const;

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
			
			StringHashMap<Index> datablocks_hash_;
			vector<Datablock> datablocks_;
	};	
}
#endif // BALL_FORMAT_CIFFILE_H

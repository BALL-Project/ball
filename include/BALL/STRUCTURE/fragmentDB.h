// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: fragmentDB.h,v 1.23 2003/03/26 13:08:33 sturm Exp $

#ifndef BALL_STRUCTURE_FRAGMENTDB_H
#define BALL_STRUCTURE_FRAGMENTDB_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
#	include <BALL/DATATYPE/stringHashMap.h>
#endif

#ifndef BALL_DATATYPE_STRINGHASHSET_H
#	include <BALL/DATATYPE/stringHashSet.h>
#endif

#ifndef BALL_DATATYPE_TRIPLE_H
#	include <BALL/DATATYPE/triple.h>
#endif

#ifndef BALL_MATHS_MATRIX44_H
#	include <BALL/MATHS/matrix44.h>
#endif

#ifndef BALL_FORMAT_RESOURCEFILE_H
#	include <BALL/FORMAT/resourceFile.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
#	include <BALL/KERNEL/residue.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
#	include <BALL/KERNEL/molecule.h>
#endif

#ifndef BALL_STRUCTURE_RECONSTRUCTFRAGMENTPROCESSOR_H
#	include <BALL/STRUCTURE/reconstructFragmentProcessor.h>
#endif

#include <vector>
#include <list>

namespace BALL 
{
	/**	FragmentDB - fragment database class.
			The fragment database is used to store commonly
			used subunits as amino acids, sugars and the like.
			They are entered in a special format described below.
			The main resource file resides under 
			<a href="../../../data/fragments/Fragments.db">data/fragments/Fragments.db</a>.  \par
	\ingroup Structure		
	*/
	class FragmentDB 
	{
		public:

		class AddHydrogensProcessor;
		friend class FragmentDB::AddHydrogensProcessor;

		/**	@name	Enums 
		*/
		//@{

		/**	Fragment types 
		*/
		enum FragmentTypes 
		{
			TYPE__UNKNOWN	= -1,
			TYPE__FRAGMENT,
			TYPE__RESIDUE,
			TYPE__MOLECULE
		};
		//@}

		/**	@name	Type Definitions 
		*/
		//@{

		/**	Type definition for the fragment type 
		*/
		typedef short Type;

		BALL_CREATE_DEEP(FragmentDB)
	
		//@}
		/**	@name	Exceptions
		*/
		//@{
		
		/**	No fragment node found.
				This exception is thrown by  \link init init \endlink  if the resource database
				does not contain a <tt>Fragments</tt> entry.
		*/
		class NoFragmentNode
			:	public Exception::GeneralException
		{
			public:
			NoFragmentNode(const char* file, int line, const string& filename) throw();
			~NoFragmentNode() throw() {}

			protected:
			string filename_;
		};

		//@}
		/**	@name Constructors and destructors 
		*/
		//@{
	
		/**	Default constructor.
				Creates a FragmentDB object and tries to read the fragment data
				from the file <tt>fragments/Fragments.db</tt> in the  \link BALL_DATA_PATH BALL_DATA_PATH \endlink .
				@exception Exception::FileNotFound if <tt>fragments/Fragments.db</tt> is not found in the BALL_DATA_PATH
		*/
		FragmentDB();

		/**	Creates a FragmentDB object and reads the contents of <tt>filename</tt>.
		*/
		FragmentDB(const String& filename);

		/**	Copy constructor.
		*/
		FragmentDB(const FragmentDB& db, bool deep = true);	

		/**	Destructor.
		*/
		virtual ~FragmentDB();

		/**	
		*/
		void destroy();

		//@}
		/**	@name	Database Setup
		*/
		//@{

		/**	Initialization of the database. 
		*/
		void init();
		
		//@}
		/**@name	Inspectors and mutators
		*/
		//@{

		/**	Assigns a filename.
		*/	
		void setFilename(const String& filename);
		
		/**	Get the filename.
		*/	
		const String& getFilename() const;
		
		/**	Checks whether a specified fragment is known to the fragment database.
		*/
		bool has(const String& fragment_name) const;

		/**	Return a fragment.
		*/
		FragmentDB::Type getFragmentType(const String& fragment_name) const;

		/**	Return a list containing all variant names.
		*/
		list<String> getVariantNames(const String& name) const;
		
		/**	Return a fragment.
		*/
		const Fragment* getFragment(const String& fragment_name) const;

		/**	Return a reference fragment.
				This method returns a standard template of a given fragment or a NULL pointer
				if the fragment is not known. The first criterion is the fragment name.
				If there exist multiple variants of the fragment, the correct variant is chosen 
				according to the properties set in <tt>fragment</tt>.
		*/
		const Fragment* getReferenceFragment(const Fragment& fragment) const;

		/**	Return a residue.
		*/
		const Residue* getResidue(const String& fragment_name) const;

		/**	Return a copy of a fragment.
				If a fragment with name <tt>fragment_name</tt> exists in the
				fragment database, a copy is created and returned. 
				Otherwise, a null pointer is returned. 
				Take care to destruct the copy again to avoid memory leaks.
				@return a pointer to the copied fragment or 0
				@param	fragent_name the name of the fragment in the database
		*/
		Fragment* getFragmentCopy(const String& fragment_name) const;

		/**	Return a copy of a fragment as a molecule.
				If a fragment with name <tt>fragment_name</tt> exists in the
				fragment database, a copy is created, inserted into a new molecule, and returned. 
				Otherwise, a null pointer is returned. 
				Take care to destruct the copy again to avoid memory leaks.
				@return a pointer to the copied fragment or 0
				@param	fragent_name the name of the fragment in the database
		*/
		Molecule* getMoleculeCopy(const String& fragment_name) const;

		/**	Return a copy of a residue.
				If a fragment with name <tt>fragment_name</tt> exists in the
				fragment database, a copy is created and returned as a residue. 
				Otherwise, a null pointer is returned. Take care to destruct the copy again
				to avoid memory leaks.
				@return a pointer to the copied fragment or 0
				@param	fragent_name the name of the fragment in the database
		*/
		Residue* getResidueCopy(const String& fragment_name) const;

		/**	Return the default naming standard
		*/
		const String&	getDefaultNamingStandard() const;

		/**	Return a hash map containing all naming maps.
		*/
		StringHashMap<StringHashMap<String>*>&	getNamingStandards();

		//@}
		/**@name	Debugging and diagnostics 
		*/
		//@{

		/**	
		*/
		bool isValid() const;

		//@}
		/**	@name	Processors defined in the fragment DB
		*/
		//@{

		/**	Name normalization processor.
				This class is used to adopt all names in a molecular system 
				to a given naming standard (usually the PDB standard).
		*/
		class NormalizeNamesProcessor 
			: public UnaryProcessor<Fragment>
		{
		
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			/**	Default constructor
			*/
			NormalizeNamesProcessor();
		
		
			/**	Constructor
			*/
			NormalizeNamesProcessor(FragmentDB& db);

			/**	Destructor
			*/
			virtual ~NormalizeNamesProcessor();
		
			//@}
			/**@name	Inspectors and Mutators
			*/
			//@{
			
			/**	Bind the processor to a fragment database.
			*/
			void setFragmentDB(FragmentDB& db);

			/**
			*/
			void setNamingStandard(const String& naming_standard);
		
			/**	Retrieve the current naming standard
			*/
			const String& getNamingStandard();

			/**	Try to match a name in one of the maps
			*/
			bool matchName(String&  res_name, String&  atom_name, const StringHashMap<String>* map) const;
	 
			//@}
			/**@name	Processor specific methods
			*/
			//@{
			
			/**	Start method
			*/
			virtual bool start();

			/**	Finish method	
			*/
			virtual bool finish();
		
			/**	Application method
			*/
			virtual Processor::Result operator () (Fragment& fragment);

			//@}

			private:
			

			String								naming_standard_;

			FragmentDB*						fragment_db_;
		
			std::list<Fragment*>	fragments_;

		};


		/**	Bond creation processor
		*/
		class BuildBondsProcessor 
			: public UnaryProcessor<Fragment> 
		{

			public:

			/** @name	Constructors and Destrutors
			*/
			//@{
		
			/**	Default constructor
			*/
			BuildBondsProcessor();
			
			/**	Constructor
			*/
			BuildBondsProcessor(const FragmentDB& db);

			/**	Destructor
			*/
			virtual ~BuildBondsProcessor();

			//@}
			/**	@name	Processor specific methods 
			*/
			//@{

			/**	
			*/
			virtual bool finish();

			/**	
			*/
			virtual bool start();

			/**	
			*/
			virtual Processor::Result operator () (Fragment& fragment);

			//@}
			/**	@name	Accessors
			*/
			//@{
			
			/**	Return the number of bonds built during the last application.
			*/
			Size getNumberOfBondsBuilt();

			/**	Set the fragment database
			*/
			void setFragmentDB(const FragmentDB& fragment_db);

			//@}
			/**	@name	Bond building methods 
			*/
			//@{

			/**	Build all bonds in a fragment.
					This method builds all bonds that are contained
					in the template.
					@return the number of bonds built
			*/
			Size buildFragmentBonds(Fragment& fragment) const;

			/**	Build all bonds in a fragment according to a manually supplied
					template.
					This method builds all bonds that are contained
					in manually provided template.
					@return the number of bonds built
			*/
			Size buildFragmentBonds(Fragment& fragment, const Fragment& tplate) const;

			/**	Build all possible bonds between two fragments.
					This method builds all bonds that are allowed by
					the <b>Connections</b> entries in a resource database.
					@return the number of bonds built
			*/
			Size buildInterFragmentBonds(Fragment& first, Fragment& second) const;

			//@}

			private:
			
			/*_	A pointer to the fragment database 
			*/
			FragmentDB*			fragment_db_;
			
			/*_	A list of all fragments.
					This list is constructed incrementally by the operator ()
					and is used by finish() to create the inter-fragment bonds
			*/
			list<Fragment*>	fragment_list_;

			/*_	The number of bonds built.
					This value is reset in the start method, so each application of 
					the processor, so {\tt getNumberOfBuiltBonds} always returns
					the number of bonds built in the last application.
			*/
			Size	bonds_built_;
		};

		//@}
		/**	@name	Public Variables
		*/
		//@{

		/**	The standard name normalization processor
		*/
		NormalizeNamesProcessor		normalize_names;

		/**	The standard hydrogen adder
		*/
		ReconstructFragmentProcessor	add_hydrogens;

		/**	The standard bond builder
		*/
		BuildBondsProcessor				build_bonds;

		/**	The resource tree read from the database file
		*/
		ResourceEntry*	tree;

		//@}

		private:
			
		/*_	@name	Parse functions
				These functions parse different sections of the fragment DB resource
				tree and translate everything into the correct data structures.
		*/
		//_@{

		/*_	Parses the Atoms entry and creates the atoms of the fragment
		*/
		void parseAtoms_(ResourceEntry& entry, Fragment& fragment);

		/*_	Parses the Bonds entry and creates the bonds of the fragment
		*/
		void parseBonds_(ResourceEntry& entry, Fragment& fragment);

		/*_	Parses the properties of a fragment/variant and sets the corresponding properties.
				Properties are set or reset (if the property name starts with "!") for the current 
				fragment. All properties of fragment and residue are recognized, if they
				are written exactly as in the header file (case insensitive) and set.
				Unknown properties are set as name/value pairs as bool properties and set to 
				{\bf true}.
		*/
		void parseProperties_(ResourceEntry& entry, PropertyManager& property_man);

		/*_	Parses the Delete section.
				All atoms given in this section are removed from the fragment.
		*/
		void parseDelete_(ResourceEntry& entry, Fragment& fragment);

		/*_	Parses the Rename section.
				All atoms given in this section are renamed to the given new name.
		*/
		void parseRename_(ResourceEntry& entry, Fragment& fragment);

		//_@}

		/*_	Expands all include directives in the resource file.
				This method calls expandFirst_ until it returns true.	
		*/
		void expandTree_(ResourceEntry& root_entry);
	 
		/*_	Expands the first occuring include directive.
				If no include directive is found, {\bf false} is returned, otherwise {\bf true}.
		*/
		bool expandFirst_(ResourceEntry& root_entry);

		bool						valid_;

		String 					filename_;

		String					default_standard_;

		std::vector<Residue*>						fragments_;

		StringHashMap<String>						name_to_path_;

		StringHashMap<Residue*>					name_to_frag_pointer_;

		StringHashMap<list<Residue*> >	name_to_variants_;

		StringHashMap<StringHashMap<String>*>	standards_;
	};
  
} // namespace BALL 


#endif // BALL_STRUCTURE_FRAGMENTDB_H_

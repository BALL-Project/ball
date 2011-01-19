// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_FRAGMENTDB_H
#define BALL_STRUCTURE_FRAGMENTDB_H

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
#	include <BALL/DATATYPE/stringHashMap.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
#	include <BALL/KERNEL/residue.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
#	include <BALL/KERNEL/molecule.h>
#endif

#ifndef BALL_KERNEL_BOND_H
#	include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_STRUCTURE_RECONSTRUCTFRAGMENTPROCESSOR_H
#	include <BALL/STRUCTURE/reconstructFragmentProcessor.h>
#endif

#include <vector>
#include <list>

namespace BALL 
{

	class ResourceEntry;

	/**	FragmentDB - fragment database class.
			The fragment database is used to store commonly
			used subunits as amino acids, sugars and the like.
			They are entered in a special format described below.
			The main resource file resides under 
			<a href="../../../data/fragments/Fragments.db">data/fragments/Fragments.db</a>.  \par
	\ingroup StructureMiscellaneous		
	*/
	class BALL_EXPORT FragmentDB 
	{
		public:

		class AddHydrogensProcessor;
		friend class FragmentDB::AddHydrogensProcessor;

		BALL_CREATE_DEEP(FragmentDB)
	
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
		/** A hash map used to convert one atom naming convention to another */
		typedef StringHashMap<String>	NameMap;

		//@}
		/**	@name	Exceptions
		*/
		//@{
		
		/**	No fragment node found.
				This exception is thrown by  \link init init \endlink  if the resource database
				does not contain a <tt>Fragments</tt> entry.
		*/
		class BALL_EXPORT NoFragmentNode
			:	public Exception::GeneralException
		{
			public:
			NoFragmentNode(const char* file, int line, const string& filename);
			~NoFragmentNode() throw() {}

			protected:
			string filename_;
		};

		//@}
		/**	@name Constructors and destructors 
		*/
		//@{
	
		/**	Creates a default but invalid FragmentDB instance.
		*/
		FragmentDB();

		/**	Creates a FragmentDB object and reads the contents of <tt>filename</tt>.
		 		If filename is an empty string, the default value "fragments/Fragments.db" is used.
				@exception Exception::FileNotFound if the file is not found in the BALL_DATA_PATH
		*/
		FragmentDB(const String& filename);

		/**	Copy constructor.
		*/
		FragmentDB(const FragmentDB& db, bool deep = true);	

		/// Assignment  operator 
		FragmentDB& operator = (const FragmentDB& db);

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
				@exception Exception::FileNotFound if the file is not found in the BALL_DATA_PATH
		 *  @exception Exception::NoFragmentNode if the FragmentDB file did not contain any node entries
		*/
		void init();
		
		//@}
		/**@name	Inspectors and mutators
		*/
		//@{

		/**	Assigns a filename.
				@exception Exception::FileNotFound if the file is not found in the BALL_DATA_PATH
		*/	
		void setFilename(const String& filename);
		
		/**	Get the filename.
		*/	
		const String& getFilename() const;
		
		/**	Checks whether a specified fragment is known to the fragment database.
		*/
		bool has(const String& fragment_name) const;

		///
		const std::vector<Residue*>& getFragments() const { return fragments_;}
		
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
		StringHashMap<NameMap>&	getNamingStandards();

		/**
		 * Return the naming standard given by std
		 *
		 * @return A StringHashMap that maps atom names to atom names
		 * @throw StringHashMap<String>::IllegalKey if std is not a valid naming standard
		 */
		const StringHashMap<String>& getNamingStandard(const String& std) const;

		/**
		 * Return a vector of available naming standards
		 */
		std::vector<String> getAvailableNamingStandards() const;

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
		class BALL_EXPORT NormalizeNamesProcessor 
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
			bool matchName(String& res_name, String& atom_name, const NameMap& map) const;
	 
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
				enum CountingMode { ADD, OVERWRITE };
				String getSuffix_(const Fragment* frag) const;
				bool doMatch_(String& res_name, const String& res_name_suffix, String& atom_name, const NameMap& map) const;
				void countHits_(HashMap<NameMap*, Index>& maps, const std::list<Fragment*>& frags);
				void countHits_(HashMap<NameMap*, Index>& maps, const Fragment* frag, CountingMode mode = OVERWRITE);
				const NameMap* getBestMap_(const HashMap<NameMap*, Index>& maps) const;
				void normalizeFragments_(const NameMap* map, const std::list<Fragment*>& frags);
				void normalizeFragment_ (const NameMap* map, Fragment* frag);

			String								naming_standard_;

			FragmentDB*						fragment_db_;
		
			std::list<Fragment*>	fragments_;

		};


		/**	Bond creation processor
		*/
		class BALL_EXPORT BuildBondsProcessor 
			: public UnaryProcessor<Fragment> 
		{

			public:

			/**	@name Type definitions	
			*/
			//@{
			///
			struct Connection
			{
				Atom*				atom;
				String			type_name;
				String			connect_to;
				Bond::Order order;
				float				dist;
				float				delta;
			};

			///
			typedef std::list<Connection> ConnectionList;
			//@}


			/** @name	Constructors and Destructors
			*/
			//@{
		
			///	
			BuildBondsProcessor();
			
			///
			BuildBondsProcessor(const FragmentDB& db);

			/// 
			virtual ~BuildBondsProcessor();
			//@}

			/**	@name	Processor-related methods 
			*/
			//@{

			///
			virtual bool finish();

			///
			virtual bool start();

			///
			virtual Processor::Result operator () (Fragment& fragment);
			//@}

			/**	@name	Accessors
			*/
			//@{
			
			/// Return the number of bonds built during the last application.
			Size getNumberOfBondsBuilt();

			/// Set the fragment database
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
					@exception Exception::TooManyBonds if an atom would be assigned too many bonds
			*/
			Size buildFragmentBonds(Fragment& fragment, const Fragment& tplate) const;

			/**	Build all possible bonds between two fragments.
					This method builds all bonds that are allowed by
					the <b>Connections</b> entries in a resource database.
					@return the number of bonds built
					@exception Exception::TooManyBonds if an atom would be assigned too many bonds
			*/
			Size buildInterFragmentBonds(Fragment& first, Fragment& second) const;

			//@}

			protected:

			/**	Store connections for a fragment.
					This method extracts all possible connections for a given fragment
					and stores them in a list of possible connections.
					finish will then check that list for possible inter-residue bonds.
			*/
			void storeConnections_(Fragment& fragment);

			/**	Build a connection between two atoms, if possible
					@exception Exception::TooManyBonds if an atom would be assigned too many bonds
			*/
			bool buildConnection_(Connection& con1, Connection& con2);
		
			/**	A pointer to the fragment database 
			*/
			FragmentDB*			fragment_db_;
			
			/**	A list of all fragments.
					This list is constructed incrementally by the operator ()
					and is used by finish() to create the inter-fragment bonds
			*/
			std::list<Fragment*>	fragment_list_;

			/*_	The number of bonds built.
					This value is reset in the start method, so each application of 
					the processor, so <TT>  getNumberOfBuiltBonds </TT> always returns
					the number of bonds built in the last application.
			*/
			Size	bonds_built_;

			/*_	The list of connects between(!) fragments still to be examined
			*/
			ConnectionList	connections_;
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
				<b>  true </b>.
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

		/** Add a new fragment pointer to the database (while parsing) */
		Position addNewFragment_(Residue* fragment);

		/*_	Expands all include directives in the resource file.
				This method calls expandFirst_ until it returns true.	
		*/
		void expandTree_(ResourceEntry& root_entry);
	 
		/*_	Expands the first occuring include directive.
				If no include directive is found, <b>  false </b> is returned, otherwise <b>  true </b>.
				@exception Exception::FileNotFound if the file is not found in the BALL_DATA_PATH
		*/
		bool expandFirst_(ResourceEntry& root_entry);

		// The status of the FragmentDB
		bool						valid_;

		// The filename of the master fragment file.
		String 					filename_;

		// The naming standard we default to.
		String					default_standard_;

		// An array containing all allocated residues.
		std::vector<Residue*>						fragments_;

		// Maps a fragment name back to a path in the database
		NameMap													name_to_path_;

		// Maps a fragment name back to the array index in fragments_
		StringHashMap<Position>					name_to_frag_index_;

		// Maps all variants of a specific fragment back to array indices.
		StringHashMap<list<Position> >	name_to_variants_;

		// Contains the naming standards as a nested map.
		StringHashMap<NameMap>					standards_;
	};
  
} // namespace BALL 


#endif // BALL_STRUCTURE_FRAGMENTDB_H

// $Id: fragmentDB.h,v 1.1 1999/09/17 11:16:37 oliver Exp $

#ifndef BALL_STRUCTURE_FRAGMENTDB_H
#define BALL_STRUCTURE_FRAGMENTDB_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <vector>
#include <list>

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
#	include <BALL/DATATYPE/stringHashMap.h>
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


namespace BALL {

	/**	FragmentDB - fragment database class.
			The fragment database is used to store commonly
			used subunits as amino acids, sugars and the like.
			They are entered in a special format described below.
			The main resource file resides under \URL[data/fragments/Fragments.db]{../../../data/fragments/Fragments.db}.

			{\bf Definition:} \URL{BALL/STRUCTURE/fragmentDB.h}
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
		//@}

		BALL_CREATE(FragmentDB)
	
		/**	@name	Exceptions
		*/
		//@{
		
		/**	No fragment node found.
				This exception is thrown by \Ref{init} if the resource database
				does not contain a {\tt Fragments} entry.
		*/
		class NoFragmentNode
			:	public Exception::GeneralException
		{
			public:
			NoFragmentNode(const char* file, int line, const string& filename);

			protected:
			string filename_;
		};
		//@}

		/**	@name Constructors and destructors */
		//@{
	
		/**	Default constructor.
				Creates a FragmentDB object and tries to read the fragment data
				from the file {\tt fragments/Fragments.db} in the \Ref{BALL\_DATA\_PATH}.
				If the file could not be opened, an excpetion of type FileNotFound is thrown
				@exception Exception::FileNotFound if {\tt fragments/Fragments.db} is not found in the BALL\_DATA\_PATH
		*/
		FragmentDB();

		/**	Creates a FragmentDB object and reads the contents of {\tt filename}.
		*/
		FragmentDB(const String& filename);


		/**	Copy constructor.
				Creates a copy of an existing FragmentDB object.
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

		/**	Initialization of the database. */
		void init();
		
		//@}


		/**@name	Inspectors and mutators
		*/
		//@{

		/**	Assigns a filename.
		*/	
		void setFilename(const String& filename);
		
		/**	Assigns a filename.
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
				according to the properties set in {\tt fragment}.
		*/
		const Fragment* getReferenceFragment(const Fragment& fragment) const;

		/**	Return a residue.
		*/
		const Residue* getResidue(const String& fragment_name) const;

		/**	Return a copy of a fragment.
		*/
		Fragment* getFragmentCopy(const String& fragment_name) const;

		/**	Return a copy of a molecule.
		*/
		Molecule* getMoleculeCopy(const String& fragment_name) const;

		/**	Return a copy of a residue
		*/
		Residue* getResidueCopy(const String& fragment_name) const;

		/**	Return the default naming standard
		*/
		const String&	getDefaultNamingStandard() const;

		/**	Return an array containing all naming maps.
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

		/**	Name normalization processor
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

		/** The AddHydrogensProcessor adds missing hydrogen atoms to all residues its started for.
				It uses a geometric calculation based on aminoacid prototypes which are 
				saved within the FragmentDataBase. 
		*/
		class AddHydrogensProcessor 
			: public UnaryProcessor<Fragment> 
		{
		
			
			public:
			/** @name Constructors and Destructors
			*/
			//@{

			/**	Default Constructor.
					 Creates a new processor
					 the according pointer to the FragmentDatabase is set 0.
					 shortnames of the aminoacids are saved within as\_ini\_.
			*/
			AddHydrogensProcessor();
			
			/**	Copy Constructor.
					 Creates a new processor.
					 The according pointer to the FragmentDataBase is set to the adress of the passed DataBase.
					 shortnames of the aminoacids are saved within as\_ini\_.
			*/
			AddHydrogensProcessor(const FragmentDB& db);

			/** Default Destruction*/
			virtual ~AddHydrogensProcessor();

			//@}
			
			/** @name processor functions*/
			//@{
			/**start function.
				 is called first after an apply() call.
				 is calling the procedures ini\_einlesen and prot\_einlesen
				 count\_h\_ is set to 0.
			*/
			virtual bool start();
			
			/**finish function.
				 is called after apply has finished.
				 in this case it does nothing.
			*/
			virtual bool finish();
			
			/**apply() function
				 adds the missing hydrogens to the passed fragment
			*/
			virtual Processor::Result operator()(Fragment&);
			//@}
		
			/** @name other functions*/
			//@{
			
			/**returns number of recently inserted hydrogens*/
			unsigned long getNumberOfInsertedH();

			//@}
			
		
			private:
			
			/*_ @name Private variables */
			//_@{

			/*_	pointer to used fragmentDataBase*/	
			const FragmentDB*	fragment_db_;
			
			/*_ Counts recently inserted hydrogens*/
			unsigned long			count_h_;
			
			/*_ saves shortnames of the aminoacids*/
			vector<String>		template_names_;
			
			/*_ it saves for every hydrogen atom of each residue the needed
					atoms to start the geometric comparison calculation.
			*/
			StringHashMap<String*> ini_table_;
						
			/*_ it saves for every residue the names of the hydrogens to be inserted
			*/
			StringHashMap<String*> h_table_;
						
			/*_ it saves the coordinates of the prototype atoms under their unique names*/
			StringHashMap<Atom> prot_table_;
						
			Residue*	residue_;

			/*_ list of the hydrogens to be inserted according to the passed residue*/ 
			String *h_atoms_;
			
			/*_ list of the atoms needed for geometric comparison according to the passed residue*/
			String *atoms_;
			//_@}
			
			/*_ @name private functions
			*/	
			//_@{

			/*_ saves prototype atoms of the fragmentDataBase in prot\_table*/
			void buildProtonTable_();
						
			/*_ create tables ini_table_ and h_table_*/
			void readTemplates_();
						
			/*_ turns passed vector around x1- axis*/
			void turn_x1_(Vector3&,const float);
						
			/*_ turns passed vector around x2- axis*/
			void turn_x2_(Vector3&,const float);
						
			/*_ turns passed vector around x3- axis*/
			void turn_x3_(Vector3&,const float);
			
			/*_ calculates angle of passed vector to x1_x2 plane*/
			float winkelx1_x2_(Vector3&);
			
			/*_ calculates angle of passed vector to x1_x3 plane*/
			float winkelx1_x3_(Vector3&);
			
			/*_ calculates position of hydrogen to be inserted based on geometric comparison.
					creates a new PDBAtom which is added to the atom list of the current residue
					creates a new bond between inserted hydrogen and passed Bindungsatom
			*/
			void calculate_(String, Atom*, Vector3& ,Vector3& ,Vector3&, Vector3&, Vector3& ,Vector3& ,Vector3& ,Vector3& ,Vector3&);
			
			/*_ called only if residue is N-terminal
					creates tetraeder around N_atom
			*/
			void buildTetraeder_(Atom*, Vector3&, Vector3&);
			
			/*_ called only if residue is not N-terminal
					adds single hydrogen at bond between the 2 residues
			*/
			void addSingleH_(Atom*,Vector3&, Vector3&, Vector3&);
			
			//_@}	
			
		};


		/**		Bond creation processor
		*/
		class BuildBondsProcessor 
			: public UnaryProcessor<Fragment> 
		{

			public:

			/**@name	Constructors and Destrutors
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
		
			/**	@name	Processor specific methods */
			//@{

			/**	
			*/
			virtual bool finish();

			/**	
			*/
			virtual Processor::Result operator () (Fragment& fragment);
			//@}


			/**	@name	Bond building methods */
			//@{

			/**
			*/
			void buildFragmentBonds(Fragment& fragment) const;

			/**
			*/
			void buildInterFragmentBonds(Fragment& first, Fragment& second) const;
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
		};

		//@}
			
		/**	@name	Public Variables
		*/
		//@{

		/**	The standard name normalization processor
		*/
		NormalizeNamesProcessor*	normalizeNames;

		/**	The standard hydrogen adder
		*/
		AddHydrogensProcessor*		addHydrogens;

		/**	The standard bond builder
		*/
		BuildBondsProcessor*			buildBonds;

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
		void expandTree_(ResourceEntry& root_entry, int depth = 10);
	 
		/*_	Expands the first occuring include directive.
				If no include directive is found, {\bf false} is returned, otherwise {\bf true}.
		*/
		bool expandFirst_(ResourceEntry& root_entry, int depth);

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

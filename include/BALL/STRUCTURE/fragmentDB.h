// $Id: fragmentDB.h,v 1.13 2001/07/25 11:28:49 oliver Exp $

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

#include <vector>
#include <list>

namespace BALL 
{

	/**	FragmentDB - fragment database class.
			The fragment database is used to store commonly
			used subunits as amino acids, sugars and the like.
			They are entered in a special format described below.
			The main resource file resides under 
			\URL[data/fragments/Fragments.db]{../../../data/fragments/Fragments.db}. \\
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

		BALL_CREATE_DEEP(FragmentDB)
	
		//@}
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
				from the file {\tt fragments/Fragments.db} in the \Ref{BALL_DATA_PATH}.
				@exception Exception::FileNotFound if {\tt fragments/Fragments.db} is not found in the BALL_DATA_PATH
		*/
		FragmentDB();

		/**	Creates a FragmentDB object and reads the contents of {\tt filename}.
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
				according to the properties set in {\tt fragment}.
		*/
		const Fragment* getReferenceFragment(const Fragment& fragment) const;

		/**	Return a residue.
		*/
		const Residue* getResidue(const String& fragment_name) const;

		/**	Return a copy of a fragment.
				If a fragment with name {\tt fragment_name} exists in the
				fragment database, a copy is created and returned. 
				Otherwise, a null pointer is returned. 
				Take care to destruct the copy again to avoid memory leaks.
				@return a pointer to the copied fragment or 0
				@param	fragent_name the name of the fragment in the database
		*/
		Fragment* getFragmentCopy(const String& fragment_name) const;

		/**	Return a copy of a fragment as a molecule.
				If a fragment with name {\tt fragment_name} exists in the
				fragment database, a copy is created, inserted into a new molecule, and returned. 
				Otherwise, a null pointer is returned. 
				Take care to destruct the copy again to avoid memory leaks.
				@return a pointer to the copied fragment or 0
				@param	fragent_name the name of the fragment in the database
		*/
		Molecule* getMoleculeCopy(const String& fragment_name) const;

		/**	Return a copy of a residue.
				If a fragment with name {\tt fragment_name} exists in the
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


		/** AddHydrogensProcessor.
				The AddHydrogensProcessor adds missing hydrogen atoms to all residues its 
				started for. It uses a geometric calculation based on aminoacid prototypes 
				which are saved within the FragmentDataBase. For calculation several hashtables 
				are used: {\tt ini_table_}, {\tt h_table_}, and {\tt prot_table}. 
				{\tt ini_table} contains for every hydrogen the list of names of reference 
				atoms used to start calculation. 
				{\tt h_table} contains for every residue the list of names of hydrogens to be inserted. 
				{\tt prot_table} contains the set of atoms of all residues in order to access atoms by name.
				First, the actual residue is identified and its database template is stored in {\tt tmp}.
				Next, tmp is beeing searched for hydrogens to be inserted. Their names are saved in 
				{\tt h_atoms_} and this	entry is stored in {\tt h_table_} for further use.
				Now the actual residue's {\tt h_table} entry is iterated in order to insert the 
				missing hydrogens. At first a check is done , wether the actual hydrogen is inserted or not.
				If it is inserted the next hydrogen is checked. If it is not, try to acces {\tt ini_table} 
				entry of actual hydrogen in order to get names of reference atoms. If it is not accessable, 
				entry has to be built and saved for further use. Get the first reference atom by simply follow 
				the bound of template hydrogen. This atom is saved in {\tt atom_feld[0]}. Now start at atom in 
				{\tt atom_feld[0]} and check its bonds. If the next bond points to a non-hydrogen atom this 
				atom is stored within the next field of {\tt atom_feld}. If actual atom has no more bonds to	
				check, procedure continues with next field of {\tt atom_feld}. Procedure stops if 4 reference 
				atoms are found. These reference atoms are saved in {\tt ini_table_} under name of the 
				hydrogen now to be inserted. Now the coordinates of reference atoms have to be determined in 
				both coordinate systems. {\tt prot_table_} contains names of the template residue. 
				If {\tt prot_table} entry does not exist so far, it has to be built.	In order to do this a 
				function named {\tt addToTemplateTable} is called which saves every atom of template under its 
				name in {\tt prot_table}, so next time its atoms can be easily accessed through their names. 
				Finally the coordinates of reference atoms are saved in vector field {\tt xx}.
				Next the actual residue is searched for the reference atoms, simply by iterating through its atoms. 
				Their coordinates	are stored in vector field {\tt x}. The vector {\tt xtarget} contains position 
				of template hydrogen which has to be transferred into coordinate system of the actual residue.
				Bindungsatom contains the atom with which the inserted atom has to built a bound.
				With these informations the calculate function is called.
				It creates a new {\tt PDBAtom} which is added to the atom list of the current residue
				It creates a new bond between inserted hydrogen and passed {\tt Bindungsatom}.
				The passed template hydrogen is transferred from one coordinate system into another. 
				In order to do the right operations some calculation is done using the passed reference atoms. 
				The following operations are done with reference atoms of actual residue and the template residue. 
				Of course every operation calculated for one reference atom is done with any other reference atom 
				(e.g.: third point is turned the same angles as second point).
				At First, the first reference point is moved to the center in both coordinate systems. 
				After that, the second point is turned onto x1x2-plane and then its turned onto x1-axis. 
				In both coordinate systems the appropriate operations	are done to the other reference points. 
				The third point is then turned onto x1x2-plane. Mirror variables are set in order to get the third 
				point into first quadrant of x1x2-plane. Again appropriate operations are done to the other points 
				in both coordinate systems. See that third point operations are uneffected to first and second point. 
				The forth point is needed just to check whether it is necessary to set another mirror at x1x2-plane.
				Finally, the operations calculated for template residue coordinate system are executed for 
				{\tt x_target}, which is the reference hydrogen to be inserted. 
				After that, the operations calculated for actual residue coordinate system are executed backwards for 
				{\tt x_target}, to bring the hydrogen into the right position.
				At last, the new atom is assigned and bounded. The next hydrogen of residue can be processed. 
				If all hydrogens of actual residue are fitted, next residue can be processed.
		*/
		class AddHydrogensProcessor 
			: public UnaryProcessor<Fragment> 
		{	
			
			public:
			/** @name Constructors and Destructors
			*/
			//@{

			/**	Default Constructor.
					The according pointer to the FragmentDatabase is set 0.
			*/
			AddHydrogensProcessor();
			
			/**	Copy Constructor.
					The according pointer to the FragmentDataBase is set to the adress of the passed DataBase.
			*/
			AddHydrogensProcessor(const FragmentDB& db);

			/** Destructor 
			*/
			virtual ~AddHydrogensProcessor();

			//@}			
			/** @name Processor-related methods 
			*/
			//@{

			/**	Processor start method
			*/
			virtual bool start();
			
			/**	Processor finish method
			*/
			virtual bool finish();
			
			/** Apply() function.
					Adds the missing hydrogens to the passed fragment
					\begin{itemize}
						\item first the actual residue is identified and its database template is stored in tmp
						\item searching tmp for hydrogens to be inserted,
									save the names in {\tt h_atoms_} and store this entry in {\tt h_table_} for further use
						\item iterating the actual residues {\tt h_table_} entry :
						\begin{itemize}
							\item test wether actual hydrogen is inserted
							\item try to access {\tt ini_table} entry of actual hydrogen in order to get names of 
										reference atoms.	If not accessable, entry is built and stored for further use
							\item getting coordinates of reference vectors and call calculate function in order
										to calculate position of actual hydrogen 
						\end{itemize}
					\end{itemize}
			*/
			virtual Processor::Result operator() (Fragment&);

			//@}
			/** @name Accessors
			*/
			//@{
			
			/** Returns number of recently inserted hydrogens
			*/
			Size getNumberOfInsertedHydrogens();

			/**	Set the fragment database
			*/
			void setFragmentDB(const FragmentDB& fragment_db);

			//@}
		
			protected:
			
			/*_ @name Protected variables 
			*/
			//_@{

			/*_	Pointer to the fragment DB 
			*/	
			const FragmentDB*	fragment_db_;
			
			/*_ Number of hydrogens inserted during the last application 
			*/
			Size	number_of_inserted_h_;
			
			/*_	Maps the handle of the reference fragments onto StringHashMaps containing 
					the hydrogen names in this fragment 
			*/
			HashMap<Handle, StringHashSet> reference_fragment_h_names_;

		  static Matrix4x4 matchVectors_
				(const Vector3& w1, const Vector3& w2, const Vector3& w3,
				 const Vector3& v1, const Vector3& v2, const Vector3& v3);

			// BAUSTELLE: bull shit
			/*_ Calculates position of hydrogen to be inserted based on geometric comparison.
					Creates a new PDBAtom which is added to the atom list of the current residue.
					Creates a new bond between inserted hydrogen and passed Bindungsatom.
					The passed template hydrogen is transferred from one coordinate system into another
					in order to do the right operations some calculation is done using the passed
					reference atoms:
					The following operations are done with reference atoms of actual residue and the prototype
					of course every operation calculated for one reference atom is done with any other reference
					atom (example:third point is turned the same angles as second point)
					\begin{itemize}
					\item first point is moved to center
					\item second point is turned onto x1x2 - plane
					\item second point is turned onto x1 - axis
					\item third point is turned onto x1x2 - plane
					\item setting of mirrors in order to get the third point into first quadrant of x1x2 plane
					\item forth point is needed just to check wether it is necessary to set another mirror
								at x1x2 plane
					\item operations calculated for prototype coordinate system are executed for x_target, the
								reference hydrogen to be inserted
					\item operations calculated for actual residue coordinate system are executed backwards for x_target in orde
								to bring hydrogen into the right position.
					\end{itemize}		  
			*/
			PDBAtom* createNewHydrogen_
				(const Atom& ref_hydrogen, 
				 const Atom& center_atom, const Atom& atom_1, const Atom& atom_2,
				 const Atom& ref_center_atom, const Atom& ref_atom_1, const Atom& ref_atom_2)
				throw();

			Triple<bool, const Atom*, const Atom*>
				getTwoReferenceAtoms_(const Atom& ref_center_atom)
				throw();

			//_@}	
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

			/**	Build all possible bonds between two fragments.
					This method builds all bonds that are allowed by
					the {\bf Connections} entries in a resource database.
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
		AddHydrogensProcessor			add_hydrogens;

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

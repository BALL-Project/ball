// $Id: nucleicAcid.h,v 1.16 2001/01/14 21:57:14 amoll Exp $ 

#ifndef BALL_KERNEL_NUCLEICACID_H
#define BALL_KERNEL_NUCLEICACID_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
#	include <BALL/KERNEL/molecule.h>
#endif

#ifndef BALL_KERNEL_NUCLEOTIDE_H
#	include <BALL/KERNEL/nucleotide.h>
#endif

#define BALL_NUCLEICACID_DEFAULT_ID   ""

namespace BALL 
{

	class Nucleotide;

	/**	Kernel class representing nucleic acids.
	*/
	class NucleicAcid
		: public Molecule
	{
		public:

		BALL_CREATE_DEEP(NucleicAcid)

		/**	@name	Enums
		*/
		//@{
			
		/**
		*/
		enum Property
		{
			NUMBER_OF_PROPERTIES = Molecule::NUMBER_OF_PROPERTIES
		};
		//@}

		/** @name	Constructors and Destructors */
		//@{

		/**	Default constructor.
		*/
		NucleicAcid()
			throw();
	
		/**	Copy constructor.
		*/
		NucleicAcid(const NucleicAcid& nucleic_acid, bool deep = true)
			throw();
	
		/**	Detailled constructor.
		*/
		NucleicAcid(const String& name, const String& id = BALL_NUCLEICACID_DEFAULT_ID)
			throw();

		/**	Destructor.
		*/
		virtual ~NucleicAcid()
			throw();
	
		/**	Clear the contents of the nucleic acid.
		*/
		virtual void clear()
			throw();
	
		/**	Clear the contents of the nucleic acid and remove it from 
				parent kernel objects.
		*/
		virtual void destroy()
			throw();

		//@}


		/**	@name	Persistence */
		//@{
		
		/**	Persistent writing.
				Writes a NucleicAcid object to a persistent stream.
				@param pm the persistence manager
		*/
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const
			throw();

		/**	Persistent reading.
				Reads a NucleicAcid object from a persistent stream.
				@param pm the persistence manager
		*/
		void persistentRead(PersistenceManager& pm)
			throw();

		//@}


		/**	@name Assignment Methods */
		//@{

		/** Assignment with cloning facility.
				Assign the NucleicAcid {\em nucleic_acid} to {\em *this}.
				The assignment is either deep or shallow (default).
				@param  nucleic_acid the NucleicAcid to be copied (cloned)
				@param  deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em nucleic_acid}
		*/
		void set(const NucleicAcid& nucleic_acid, bool deep = true)
			throw();

		/**	Assignment operator.
		*/
		const NucleicAcid& operator = (const NucleicAcid& nucleic_acid)
			throw();

		/**	Assign a NucleicAcid object from another.
		*/
		void get(NucleicAcid& nucleic_acid, bool deep = true) const
			throw();
	
		/**	Swap the contents of two NucleicAcid objects.
		*/
		void swap(NucleicAcid& nucleic_acid)
			throw();
	
		//@}

		/**	Equality operator.
				Two nucleic acids are equal if they have the same attributes and properties and are homomorph.
				@see Molecule::operator ==
		*/
		bool operator == (const NucleicAcid& nucleic_acid) const
			throw();

		/**	Inequality operator
				@see operator ==
		*/
		bool operator != (const NucleicAcid& nucleic_acid) const
			throw();

		/**	@name Accessors */
		//@{

		/** Get a pointer to a child Nucleotide at a given position.
				The reference is 0 if {\em *this} instance does not have a Nucleotide at the given position.
				@param   position the position of the child Nucleotide
				@return  Nucleotide* -
								 mutable reference to the child Nucleotide at {\em positon} of {\em *this},
		*/
		Nucleotide* getNucleotide(Position position)
			throw();
	
		/** Get a pointer to a child Nucleotide at a given position.
				The reference is 0 if {\em *this} instance does not have a Nucleotide at the given position.
				@param   position the position of the child Nucleotide
				@return  Nucleotide* -
								 constant reference to the child Nucleotide at {\em positon} of {\em *this},
		*/
		const Nucleotide* getNucleotide(Position position) const
			throw();

		/**	Retrieve a pointer to the Nucleotide at the 3'-end of {\em *this}.
				The reference is 0 if {\em *this} instance does not have a Nucleotide.
				@return  Nucleotide* -
								 mutable reference to the first child Nucleotide
		*/
		Nucleotide* get3Prime()
			throw();

		/**	Retrieve a const pointer to the nucleotide at the 3'-end of {\em *this}.
				The reference is 0 if {\em *this} instance does not have a Nucleotide.
				@return  Nucleotide* -
								 constant reference to the first child Nucleotide
		*/
		const Nucleotide* get3Prime() const
			throw();

		/**	Retrieve a pointer to the Nucleotide at the 5'-end of {\em *this}.
				The reference is 0 if {\em *this} instance does not have a Nucleotide.
				@return  Nucleotide* -
								 mutable reference to the last child Nucleotide
		*/
		Nucleotide* get5Prime()
			throw();

		/**	Retrieve a const pointer to the Nucleotide at the 5'-end of {\em *this}.
				The reference is 0 if {\em *this} instance does not have a Nucleotide.
				@return  Nucleotide* -
								 constant reference to the last child Nucleotide
		*/
		const Nucleotide* get5Prime() const
			throw();

		/**	Set the ID of the NucleicAcid.
				@param id the new ID
		*/
		void setID(const String& id)
			throw();

		/**	Retrieve the ID of the NucleicAcid.
				@return String the ID
		*/
		const String& getID() const
			throw();

		/**	Return the number of Nucleotides contained in {\em *this} NucleicAcid.
				@return Size - number of Nucleotides
		*/
		Size countNucleotides() const
			throw();
		//@}

#ifdef	BALL_CFG_USING_METHOD_DIRECTIVE
		using Molecule::prepend;
		using Molecule::append;
		using Molecule::insert;
		using Molecule::insertBefore;
		using Molecule::insertAfter;
		using Molecule::remove;
		using Molecule::splice;
		using Molecule::spliceBefore;
		using Molecule::spliceAfter;
#else
		Molecule::prepend;
		Molecule::append;
		Molecule::insert;
		Molecule::insertBefore;
		Molecule::insertAfter;
		Molecule::remove;
		Molecule::splice;
		Molecule::spliceBefore;
		Molecule::spliceAfter;
#endif

		/**	@name	Debugging and Diagnostics
		*/
		//@{

		/** Internal state and consistency self-validation.
				Initiate self-validation of the internal state and data structure consistencies of {\em *this}.
				If the internal state of {\em *this} is correct (self-validated) and consistent {\tt true}
				is returned, {\tt false} otherwise. 
	
				@return			bool -
										{\tt true} if the internal state of {\em *this} atom is correct (self-validated)
										and consistent,	{\tt false} otherwise
		*/
		virtual bool isValid() const
			throw();

			/** Internal state dump.
					Dump the current internal state of {\em *this} to the output ostream {\em s}
					with dumping depth {\em depth}.
					@param   s - output stream where to output the internal state of {\em *this}
					@param   depth - the dumping depth
			*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
			throw();
		//@}

		/**	@name	Storers
		*/
		//@{

		/* Persistent stream input and state restorage.
				Read persistent data from the input stream {\em s} and restore the state of {\em *this}.
				\\
				{\bf Note:} Not yet implemented.
				@param  s input stream from where to restore the internal state of {\em *this}
		*/
		virtual void read(std::istream& s)
			throw();

		/* Persistent stream input and state restorage.
				Read persistent data from the input stream {\em s} and restore the state of {\em *this}.
				\\
				{\bf Note:} Not yet implemented.
				@param  s input stream from where to restore the internal state of {\em *this}
		*/
		virtual void write(std::ostream& s) const
			throw();
		//@}

		// --- EXTERNAL ITERATORS

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Nucleotide)
		

		private:

		// --- ATTRIBUTES

		String id_;
	};

} // namespace BALL

#endif // BALL_KERNEL_NUCLEICACID_H

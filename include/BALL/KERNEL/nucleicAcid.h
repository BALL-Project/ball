// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_KERNEL_NUCLEICACID_H
#define BALL_KERNEL_NUCLEICACID_H

#ifndef BALL_KERNEL_MOLECULE_H
#	include <BALL/KERNEL/molecule.h>
#endif

#ifndef BALL_KERNEL_NUCLEOTIDE_H
#	include <BALL/KERNEL/nucleotide.h>
#endif

#define BALL_NUCLEICACID_DEFAULT_ID   ""

namespace BALL 
{
	/**	Nucleic acid class.
			 \par
			
			 \par
    	
			\ingroup KernelContainers 
	*/
	class BALL_EXPORT NucleicAcid
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
		/** @name	Constructors and Destructors 
		*/
		//@{

		/**	Default constructor.
		*/
		NucleicAcid();
	
		/**	Copy constructor.
		*/
		NucleicAcid(const NucleicAcid& nucleic_acid, bool deep = true);
	
		/**	Detailed constructor.
		*/
		NucleicAcid(const String& name,
				const String& id = BALL_NUCLEICACID_DEFAULT_ID);

		/**	Destructor.
		*/
		virtual ~NucleicAcid();
	
		/**	Clear the contents of the nucleic acid.
		*/
		virtual void clear();
	
		/**	Clear the contents of the nucleic acid and remove it from 
				parent kernel objects.
		*/
		virtual void destroy();

		//@}
		/**	@name	Persistence 
		*/
		//@{
		
		/**	Writes a NucleicAcid object to a persistent stream.
		 *	@param pm the persistence manager
		 */
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		/**	Reads a NucleicAcid object from a persistent stream.
		 *	@param pm the persistence manager
		 */
		void persistentRead(PersistenceManager& pm);

		//@}
		/**	@name Assignment Methods 
		*/
		//@{

		/** Assignment with cloning facility.
				The assignment is either deep or shallow (default).
				@param  nucleic_acid the NucleicAcid to be copied (cloned)
				@param  deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy
		*/
		void set(const NucleicAcid& nucleic_acid, bool deep = true);

		/**	Assignment operator.
		*/
		NucleicAcid& operator = (const NucleicAcid& nucleic_acid);

		/**	Assign a NucleicAcid object from another.
		*/
		void get(NucleicAcid& nucleic_acid, bool deep = true) const;
	
		/**	Swap the contents of two NucleicAcid objects.
		*/
		void swap(NucleicAcid& nucleic_acid);
	
		//@}

		/**	Equality operator.
				Two nucleic acids are equal if they have the same handle.
				@see Object::operator ==.
		*/
		bool operator == (const NucleicAcid& nucleic_acid) const;

		/**	Inequality operator
				@see operator ==
		*/
		bool operator != (const NucleicAcid& nucleic_acid) const;

		/**	@name Accessors */
		//@{

		/** Get a pointer to a child Nucleotide at a given position.
				The pointer is 0 if this instance does not have a Nucleotide at the
				given position.
				@param		position the position of the child Nucleotide
				@return		Nucleotide* - mutable pointer to the child Nucleotide at
									<b>  positon </b>
		*/
		Nucleotide* getNucleotide(Position position);
	
		/** Get a pointer to a child Nucleotide at a given position.
				The pointer is 0 if this instance does not have a Nucleotide at the
				given position.
				@param		position the position of the child Nucleotide
				@return		Nucleotide* - constant pointer to the child Nucleotide at
									<b>  positon </b>
		*/
		const Nucleotide* getNucleotide(Position position) const;

		/**	Retrieve a pointer to the Nucleotide at the 3'-end of this instance.
				The pointer is 0 if this instance does not have a Nucleotide.
				@return  Nucleotide* - mutable pointer to the first child Nucleotide
		*/
		Nucleotide* get3Prime();

		/**	Retrieve a const pointer to the nucleotide at the 3'-end of this instance.
				The pointer is 0 if this instance does not have a Nucleotide.
				@return  Nucleotide* - constant pointer to the first child Nucleotide
		*/
		const Nucleotide* get3Prime() const;

		/**	Retrieve a pointer to the Nucleotide at the 5'-end of this instance.
				The pointer is 0 if this instance does not have a Nucleotide.
				@return  Nucleotide* - mutable pointer to the last child Nucleotide
		*/
		Nucleotide* get5Prime();

		/**	Retrieve a const pointer to the Nucleotide at the 5'-end of this instance.
				The pointer is 0 if this instance does not have a Nucleotide.
				@return  Nucleotide* - constant pointer to the last child Nucleotide
		*/
		const Nucleotide* get5Prime() const;

		/**	Set the ID of the NucleicAcid.
				@param id the new ID
		*/
		void setID(const String& id);

		/**	Retrieve the ID of the NucleicAcid.
				@return String the ID
		*/
		const String& getID() const;

		/**	Return the number of Nucleotides contained in this instance NucleicAcid.
				@return Size - number of Nucleotides
		*/
		Size countNucleotides() const;

		//@}

		using Molecule::prepend;
		using Molecule::append;
		using Molecule::insert;
		using Molecule::insertBefore;
		using Molecule::insertAfter;
		using Molecule::remove;
		using Molecule::splice;
		using Molecule::spliceBefore;
		using Molecule::spliceAfter;

		/**	@name	Debugging and Diagnostics
		*/
		//@{

		/** Internal state and consistency self-validation.
				@return	 bool - <tt>true</tt> if the internal state of this instance atom 
								 is correct (self-validated) and consistent, <tt>false</tt>
								 otherwise
		*/
		virtual bool isValid() const;

			/** Internal state dump.
					Dump the current internal state of this instance to the output
					ostream <b>  s </b> with dumping depth <b>  depth </b>.
					@param	s - output stream where to output the internal state of
									this instance
					@param	depth - the dumping depth
			*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

		//@}

		// --- EXTERNAL ITERATORS

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Nucleotide)
		BALL_DECLARE_STD_ITERATOR_WRAPPER(NucleicAcid, Nucleotide, nucleotides)

		private:

		// --- ATTRIBUTES

		String id_;
	};
} // namespace BALL

#endif // BALL_KERNEL_NUCLEICACID_H

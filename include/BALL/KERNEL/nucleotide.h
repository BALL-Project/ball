// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_KERNEL_NUCLEOTIDE_H
#define BALL_KERNEL_NUCLEOTIDE_H

#ifndef BALL_KERNEL_FRAGMENT_H
#	include <BALL/KERNEL/fragment.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
#	include <BALL/KERNEL/residue.h>
#endif

#ifndef BALL_KERNEL_NUCLEOTIDEITERATOR_H
#	include <BALL/KERNEL/nucleotideIterator.h>
#endif

 
#define BALL_NUCLEOTIDE_DEFAULT_ID               ""
#define BALL_NUCLEOTIDE_DEFAULT_INSERTION_CODE   ' '

namespace BALL 
{
	class NucleicAcid;

	/**	Nucleotide class.
			This class is used to represent nucleotides within
			an  \link NucleicAcid NucleicAcid \endlink  object.
			 \par
			
    	\ingroup KernelContainers 
	*/
	class BALL_EXPORT Nucleotide
		: public Fragment
	{
		public:

		BALL_CREATE_DEEP(Nucleotide)
	
		/**	@name	Enums
		*/
		//@{

		///
		enum Property
		{
			///
			PROPERTY__5_PRIME = Residue::NUMBER_OF_PROPERTIES + 1,
			///
			PROPERTY__3_PRIME,
			/// 
			PROPERTY__NUCLEOTIDE,

			///
			NUMBER_OF_PROPERTIES
		};

		//@}
		/**	@name	Constructors and Destructors 
		*/
		//@{	
	
		/// Default constructor
		Nucleotide();
	
		/// Copy constructor
		Nucleotide(const Nucleotide& nucleotide, bool deep = true);
	
		/// Detailled constructor
		Nucleotide
			(const String& name,
			 const String& id = BALL_NUCLEOTIDE_DEFAULT_ID,
			 char insertion_code = BALL_NUCLEOTIDE_DEFAULT_INSERTION_CODE);

		/// Destructor
		virtual ~Nucleotide();

		/// Clears the nucleotides contents.
		virtual void clear();
	
		/// Clears the nucleotides contents and removes it from all composite structures.
		virtual void destroy();
	
		//@}
		/** @name Persistence 
		*/
		//@{

		/**	Writes a Nucleotide object to a persistent stream.
		 *	@param pm the persistence manager
		 */
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		/**	Reads a Nucleotide object from a persistent stream.
		 *	@param pm the persistence manager
		 */
		void persistentRead(PersistenceManager& pm);

		//@}
		/**	@name	Assignment 
		*/
		//@{

		/** Assignment with cloning facility.
				The assignment is either deep or shallow (default).
				@param  nucleotide the nucleotide to be copied (cloned)
				@param  deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy
		*/
		void set(const Nucleotide& nucleotide, bool deep = true);

		/** Assignment operator.
				The assignment is either deep or shallow (default).
				@param   nucleotide the nucleotide to be copied (cloned)
				@return  nucleotide& - this instance nucleotide
				@see     nucleotide::set
		*/
		Nucleotide& operator = (const Nucleotide& nucleotide);

		/** Copy this instance to <b>  nucleotide </b>.
				The assignment is either deep or shallow (default).
				@param  nucleotide the nucleotide to be assigned to
				@see    nucleotide::set
		*/
		void get(Nucleotide& nucleotide, bool deep = true) const;

		/** Swapping of instaces of nucleotide.
				@param  nucleotide the instance of nucleotide to swap with
		*/
		void swap(Nucleotide& nucleotide);
	
		//@}

		/**	Equality operator.
				Two nucleotides are equal if they have the same handle.
				@see Object::operator ==.
		*/
		bool operator == (const Nucleotide& nucleotide) const;

		/**	Inequality operator
				@see operator ==
		*/
		bool operator != (const Nucleotide& nucleotide) const;

		/**	@name	Accessors */
		//@{

		/** Get a pointer to the parent NucleicAcid.
				The pointer is 0 if this instance nucleotide does not have a parent NucleicAcid.
				@return  NucleicAcid* - mutable pointer to the parent NucleicAcid
		*/
		NucleicAcid* getNucleicAcid();
		
		/** Get a pointer to the parent NucleicAcid.
				The pointer is 0 if this instance nucleotide does not have a parent NucleicAcid.
				@return  NucleicAcid* - constant pointer to the parent NucleicAcid
		*/
		const NucleicAcid* getNucleicAcid() const;

		/**	Set the ID of the nucleotide.
				@param id the new ID
		*/
		void setID(const String& id);

		/**	Retrieve the ID of the nucleotide.
				@return String the ID (constant)
		*/
		const String& getID() const;

		/**
		 * Set the insertion code.
		 *
		 * An insertion code is a visible ASCII character as well as the space
		 * character ' '.
		 *
		 * @param insertion_code the new insertion code
		 * @throws InvalidArgument if an unsupported insertion code has been specified.
		 */
		void setInsertionCode(char insertion_code);

		/**
		 * Unset the current insertion code.
		 *
		 * This is equivalent to calling setInsertionCode(' ').
		 */
		void unsetInsertionCode();

		/**	Retrieve the insertion code of the nucleotide.
				@return String the insertion code (constant)
		*/
		char getInsertionCode() const;

		/** Prepend an atom at position 0.
				@param atom the atom to prepend
		*/
		void prepend(Atom& atom);

		/** Append an atom after the last position.
				@param atom the atom to append
		*/
		void append(Atom& atom);

		/** Insert an atom after the last position.
				@param atom the atom to insert
		*/
		void insert(Atom& atom);

		/** Insert an atom before a given Composite object.
				@param atom the atom to insert
				@param before the Composite object to insert before
		*/
		void insertBefore(Atom& atom, Composite& before);

		/** Insert an atom after a given Composite object.
				@param atom the atom to insert
				@param after the Composite object to insert after
		*/
		void insertAfter(Atom& atom, Composite& after);

		/** Remove an atom.
				@param atom the atom to remove
		*/
		bool remove(Atom& atom);

		/**	Cut all children of <tt>nucleotide</tt> and prepend them before the children of this instance.
				@param nucleotide the nucleotide to access
		*/
		void spliceBefore(Nucleotide& nucleotide);

		/**	Cut all children of <tt>nucleotide</tt> and append them after the children of this instance.
				@param nucleotide the nucleotide to access
		*/
		void spliceAfter(Nucleotide& nucleotide);

		/**	Move the children of <tt>nucleotide</tt> into this instance.
				The children are inserted using  \link spliceBefore spliceBefore \endlink .
		*/
		void splice(Nucleotide& nucleotide);

		//@}
		/**	@name	Predicates 
		*/
		//@{

		/**	Test if this instance nucleotide is terminal.
				Returns true, if this instance is the first or 
				last nucleotide in its parent NucleicAcid.
				@return bool
		*/
		bool isTerminal() const;
	
		/**	Test if this instance nucleotide is 3-prime.
				Returns true, if this instance is the first nucleotide in its parent NucleicAcid.
				@return bool
		*/
		bool is3Prime() const;

		/**	Test if this instance nucleotide is 5-prime.
				Returns true, if this instance is the last nucleotide in its parent NucleicAcid.
				@return bool
		*/
		bool is5Prime() const;

		//@}
		/**	@name	Debugging and Diagnostics 
		*/
		//@{

		/** Internal state and consistency self-validation.
				@return	 bool -	<tt>true</tt> if the internal state of this instance nucleotide is correct
												(self-validated) and consistent, <tt>false</tt> otherwise
		*/
		virtual bool isValid() const;

		/** Internal state dump.
				Dump the current internal state of this instance 
				to the output ostream <b>  s </b> with dumping depth <b>  depth </b>.
				@param	s output stream where to output the internal state of this instance nucleotide
				@param  depth the dumping depth
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

		//@}
 
		private:

		AtomContainer* getAtomContainer(Position position);
	
		const AtomContainer* getAtomContainer(Position position) const;
	
		Size countAtomContainers() const;

		void prepend(AtomContainer& atom_container);

		void append(AtomContainer& atom_container);

		void insert(AtomContainer& atom_container);

		void insertBefore(AtomContainer& atom_container, Composite& composite);

		void insertAfter(AtomContainer& atom_container, Composite& composite);

		void spliceBefore(AtomContainer& atom_container);

		void spliceAfter(AtomContainer& base_ragment);

		void splice(AtomContainer& AtomContainer);

		bool remove(AtomContainer& AtomContainer);

		bool isSuperAtomContainerOf(const AtomContainer& atom_container) const;

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(AtomContainer)
		BALL_DECLARE_STD_ITERATOR_WRAPPER(Nucleotide, AtomContainer, atomContainers)

		// --- ATTRIBUTES

		String 	id_;

		char 		insertion_code_;
	};


  template <class NucleotideContainerType>
  const Nucleotide* get5Prime(const NucleotideContainerType& nucleotide_container)
  {
		NucleotideConstIterator res_it;
    for ( res_it = nucleotide_container.beginNucleotide(); !res_it.isEnd(); ++res_it)
		{
			return &(*res_it);
		}

    return 0;
  }

  template <class NucleotideContainerType>
  const Nucleotide* get3Prime(const NucleotideContainerType& nucleotide_container)
  {
    for (NucleotideConstIterator res_it = nucleotide_container.rbeginNucleotide(); !res_it.isREnd(); ++res_it)
		{
			return &(*res_it);
		}

    return 0;
  }
 
} // namespace BALL

#endif // BALL_KERNEL_NUCLEOTIDE_H

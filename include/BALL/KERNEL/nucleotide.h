// $Id: nucleotide.h,v 1.8 2000/05/02 14:03:50 amoll Exp $

#ifndef BALL_KERNEL_NUCLEOTIDE_H
#define BALL_KERNEL_NUCLEOTIDE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_FRAGMENT_H
#	include <BALL/KERNEL/fragment.h>
#endif

#ifndef BALL_KERNEL_NUCLEOTIDEITERATOR_H
#	include <BALL/KERNEL/nucleotideIterator.h>
#endif

 
#define BALL_NUCLEOTIDE_DEFAULT_ID               ""
#define BALL_NUCLEOTIDE_DEFAULT_INSERTION_CODE   ' '

namespace BALL 
{

	class NucleicAcid;
	class Protein;

	/**	Kernel class representing a nucleotide.
	*/
	class Nucleotide
		: public Fragment
	{
		public:

		BALL_CREATE(Nucleotide)
	
		/**	@name	Enums
		*/
		//@{

		///
		enum Property
		{
			///
			PROPERTY__5_PRIME = Fragment::NUMBER_OF_PROPERTIES + 1,
			///
			PROPERTY__3_PRIME,

			///
			NUMBER_OF_PROPERTIES
		};
		//@}

		/**	@name	Constructors and Destructors */
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

		/// Clears the nucleotides contents
		virtual void clear();
	
		/// Clears the nucleotides contents and removes it from all composite structures
		virtual void destroy();
	
		//@}

		/** @name Persistence */
		//@{

		/// Persistent writing
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		/// Persistent reading
		void persistentRead(PersistenceManager& pm);

		//@}


		/**	@name	Assignment */
		//@{

		/** Assignment with cloning facility.
				Assign the nucleotide {\em nucleotide} to {\em *this} nucleotide.
				The assignment is either deep or shallow (default).
				@param  nucleotide the nucleotide to be copied (cloned)
				@param  deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em nucleotide}
		*/
		void set(const Nucleotide& nucleotide, bool deep = true);

		/** Assignment operator.
				Assign the nucleotide {\em nucleotide} to {\em *this}.
				The assignment is either deep or shallow (default).
				@param   nucleotide the nucleotide to be copied (cloned)
				@return  nucleotide& - {\em *this} nucleotide
				@see     nucleotide::set
		*/
		Nucleotide& operator = (const Nucleotide& nucleotide);

		/** Copying with cloning facility.
				Copy {\em *this} nucleotide to the nucleotide {\em nucleotide}.
				The assignment is either deep or shallow (default).
				@param  nucleotide the nucleotide to be assigned to
				@see    nucleotide::set
		*/
		void get(Nucleotide& nucleotide, bool deep = true) const;

		/** Swapping of nucleotide.
				Swap the states of {\em *this} nucleotide with the nucleotide {\em nucleotide}.
				@param  nucleotide the nucleotide {\em *this} is being swapped with
		*/
		void swap(Nucleotide& nucleotide);
	
		//@}

		/**	@name	Accessors */
		//@{

		/** Get a pointer to the parent NucleicAcid.
				The reference is 0 if {\em *this} nucleotide does not have a parent NucleicAcid.
				@return  NucleicAcid* -
								 mutable reference to the parent NucleicAcid of {\em *this} nucleotide,
		*/
		NucleicAcid* getNucleicAcid();
		
		/** Get a pointer to the parent NucleicAcid.
				The reference is 0 if {\em *this} nucleotide does not have a parent NucleicAcid.
				@return  NucleicAcid* -
								 constant reference to the parent NucleicAcid of {\em *this} nucleotide,
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

		/**	Set the insertion code of the nucleotide.
				@param id the new ID
		*/
		void setInsertionCode(char insertion_code);

		/**	Retrieve the insertion code of the nucleotide.
				@return String the ID
		*/
		char getInsertionCode() const;

		/** Prepend an atom at position 0.
				@param atom, the atom to prepend
		*/
		void prepend(Atom& atom);

		/** Append an atom after the last position.
				@param atom, the atom to append
		*/
		void append(Atom& atom);

		/** Insert an atom after the last position.
				@param atom, the atom to insert
		*/
		void insert(Atom& atom);

		/** Insert an atom before a given {\em Comosite} object.
				@param atom, the atom to insert
				@param before, the {\em Comosite} object to insert before
		*/
		void insertBefore(Atom& atom, Composite& before);

		/** Insert an atom after a given {\em Comosite} object.
				@param atom, the atom to insert
				@param after, the {\em Comosite} object to insert after
		*/
		void insertAfter(Atom& atom, Composite& after);

		/** Remove an atom
				@param atom the atom to remove
		*/
		bool remove(Atom& atom);

		/**	Cut all children of {\tt nucleotide} and prepend them before the children of this nucleotide.
				@param nucleotide the nucleotide to access
		*/
		void spliceBefore(Nucleotide& nucleotide);

		/**	Cut all children of {\tt nucleotide} and append them after the children of this nucleotide.
				@param nucleotide the nucleotide to access
		*/
		void spliceAfter(Nucleotide& nucleotide);

		/**	Move the children of {\tt nucleotide} into this nucleotide.
				The children of {\tt nucleotide} are inserted at the position of 
				{\tt nucleotide} if it is a child of {\em *this}.
				Otherwise the children are inserted using \Ref{spliceBefore}.
		*/
		void splice(Nucleotide& nucleotide);

		//@}

	
		/**	@name	Predicates */
		//@{

		/**	Test if {\em *this} nucleotide is terminal.
				@return bool true if {\em *this} nucleotide is terminal
				{\bf Note:} Not yet implemented.	
		*/
		bool isTerminal() const;
	
		/**	Test if {\em *this} nucleotide is 3-prime.
				@return bool true if {\em *this} nucleotide is 3-prime
				{\bf Note:} Not yet implemented.	
		*/
		bool is3Prime() const;

		/**	Test if {\em *this} nucleotide is 5-prime.
				@return bool true if {\em *this} nucleotide is 5-prime
				{\bf Note:} Not yet implemented.	
		*/
		bool is5Prime() const;
		//@}

	
		/**	@name	Debugging and Diagnostics */
		//@{

		/** Internal state and consistency self-validation.
				Initiate self-validation of the internal state and data structure consistencies of {\em *this}.
				If the internal state of {\em *this} nucleotide is correct (self-validated)
				and consistent {\tt true} is returned, {\tt false} otherwise. 
				@return			bool -
										{\tt true} if the internal state of {\em *this} nucleotide is correct
										(self-validated) and consistent, {\tt false} otherwise
		*/
		virtual bool isValid() const;

		/** Internal state dump.
				Dump the current internal state of {\em *this} 
				to the output ostream {\em s} with dumping depth {\em depth}.
				@param	s output stream where to output the internal state of {\em *this} nucleotide
				@param  depth the dumping depth
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
		//@}

		/**	@name	Storers
		*/
		//@{

		/* Persistent stream input and state restorage.
				Read persistent bond data from the input stream {\em s} and restore the state of {\em *this}.
				\\
				{\bf Note:} Not yet implemented.
				@param  s input stream from where to restore the internal state of {\em *this} nucleotide
		*/
		virtual void read(std::istream& s);

		/* Persistent stream output and state storage.
				Write persistent bond data to the output stream {\em s} and store the state of {\em *this}.
				\\
				{\bf Note:} Not yet implemented.	
				@param  s input stream from where to restore the internal state of {\em *this} nucleotide
		*/
		virtual void write(std::ostream& s) const;
		//@}


		private:

		BaseFragment* getBaseFragment(Position position);
	
		const BaseFragment* getBaseFragment(Position position) const;
	
		Size countBaseFragments() const;

		void prepend(BaseFragment& base_fragment);

		void append(BaseFragment& base_fragment);

		void insert(BaseFragment& base_fragment);

		void insertBefore(BaseFragment& base_fragment, Composite& composite);

		void insertAfter(BaseFragment& base_fragment, Composite& composite);

		void spliceBefore(BaseFragment& base_fragment);

		void spliceAfter(BaseFragment& base_ragment);

		void splice(BaseFragment& BaseFragment);

		bool remove(BaseFragment& BaseFragment);

		bool isSuperBaseFragmentOf(const BaseFragment& base_fragment) const;

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(BaseFragment)

		void clear_();

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

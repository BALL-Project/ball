// $Id: nucleotide.h,v 1.2 1999/12/28 18:38:45 oliver Exp $

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

	/**
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
			PROPERTY__5_PRIME Fragment::NUMBER_OF_PROPERTIES + 1,
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

		///	
		void set(const Nucleotide& nucleotide, bool deep = true);

		///	
		Nucleotide &operator = (const Nucleotide& nucleotide);

		///	
		void get(Nucleotide& nucleotide, bool deep = true) const;

		///	
		void swap(Nucleotide& nucleotide);
	
		//@}

		/**	@name	Accessors */
		//@{

		///
		Protein* getProtein();

		///
		const Protein* getProtein() const;

		///
		NucleicAcid* getNucleicAcid();
		
		///
		const NucleicAcid* getNucleicAcid() const;

		///
		void setID(const String& id);

		///
		String& getID();

		///
		const String& getID() const;

		///
		void setInsertionCode(char insertion_code);

		///
		char getInsertionCode() const;

		///
		void prepend(Atom& atom);

		///
		void append(Atom& atom);

		///
		void insert(Atom& atom);

		///
		void insertBefore(Atom& atom, Composite& before);

		///
		void insertAfter(Atom& atom, Composite& after);

		///
		bool remove(Atom& atom);

		///
		void spliceBefore(Nucleotide& nucleotide);

		///
		void spliceAfter(Nucleotide& nucleotide);

		///
		void splice(Nucleotide& nucleotide);

		//@}


	
		/**	@name	Predicates */
		//@{

		///
		bool isTerminal() const;
	
		///
		bool is3Prime() const;
	
		///
		bool is5Prime() const;

		//@}

	
		/**	@name	Debugging and Diagnostics */
		//@{

		///
		virtual bool isValid() const;

		///
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
		//@}

		/**	@name	Storers
		*/
		//@{

		virtual void read(std::istream& s);

		virtual void write(std::ostream& s) const;
		//@}


		private:

		BaseFragment* getBaseFragment(Index index);
	
		const BaseFragment* getBaseFragment(Index index) const;
	
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
			return &(*res_it);

    return 0;
  }

  template <class NucleotideContainerType>
  const Nucleotide* get3Prime(const NucleotideContainerType& nucleotide_container)
  {
    for (NucleotideConstIterator res_it = nucleotide_container.rbeginNucleotide(); !res_it.isREnd(); ++res_it)
			return &(*res_it);

    return 0;
  }
 

} // namespace BALL

#endif // BALL_KERNEL_NUCLEOTIDE_H

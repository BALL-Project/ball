// $Id: nucleicAcid.h,v 1.3 1999/12/28 18:38:45 oliver Exp $ 

#ifndef BALL_KERNEL_NUCLEICACID_H
#define BALL_KERNEL_NUCLEICACID_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#define BALL_NUCLEICACID_DEFAULT_ID   ""

namespace BALL 
{

	/**
	*/
	class NucleicAcid
		: public Molecule
	{
		public:

		BALL_CREATE(NucleicAcid)

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

		///
		NucleicAcid();
	
		///
		NucleicAcid(const NucleicAcid& nucleic_acid, bool deep = true);
	
		///
		NucleicAcid(const String& name, const String& id = BALL_NUCLEICACID_DEFAULT_ID);

		///
		virtual ~NucleicAcid();
	
		///
		virtual void clear();
	
		///
		virtual void destroy();

		//@}


		/**	@name	Persistence */
		//@{
		
		///
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		///
		void persistentRead(PersistenceManager& pm);

		//@}


		/**	@name Assignment Methods */
		//@{

		/**
		*/
		void set(const NucleicAcid& nucleic_acid, bool deep = true);

		/**
		*/
		NucleicAcid& operator = (const NucleicAcid& nucleic_acid);

		/**
		*/
		void get(NucleicAcid& nucleic_acid, bool deep = true) const;
	
		/**
		*/
		void swap(NucleicAcid& nucleic_acid);
	
		//@}


		/**	@name Accessors */
		//@{

		///
		Nucleotide* get3Prime();

		///		
		const Nucleotide* get3Prime() const;

		///		
		Nucleotide* get5Prime();

		///		
		const Nucleotide* get5Prime() const;

		///		
		void setID(const String& id);

		///
		String& getID();

		///
		const String& getID() const;

		///
		Size countNucleotides() const;
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

		/**
		*/
		virtual bool isValid() const;

		/**
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
		//@}

		/**	@name	Storers
		*/
		//@{

		/**
		*/
		virtual void read(std::istream& s);

		/**
		*/
		virtual void write(std::ostream& s) const;
		//@}

		// --- EXTERNAL ITERATORS

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Nucleotide)

		
		private:

		void clear_();

		// --- ATTRIBUTES

		String id_;
	};

} // namespace BALL

#endif // BALL_KERNEL_NUCLEICACID_H

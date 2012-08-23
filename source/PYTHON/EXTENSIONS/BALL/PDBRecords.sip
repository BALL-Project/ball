// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//
	class PDBRecords
	{
%TypeHeaderCode
	#include <BALL/FORMAT/PDBRecords.h>
	using namespace BALL;
%End
		public:
		PDBRecords() throw();
		PDBRecords(const PDBRecords&) throw();
		virtual ~PDBRecords() throw();
		//		PDBRecords& operator = (const PDBRecords& rhs);

		//ConstIterator begin() const;
		//Iterator begin();
		//ConstIterator end() const;
		//Iterator end();

		//ConstReverseIterator rbegin() const;
		//ReverseIterator rbegin();
		//ConstReverseIterator rend() const;
		//ReverseIterator rend();

		Size size() const;
		Size max_size() const;
		Size capacity() const;
		bool empty() const;
		void clear();

		void resize(Size, String c = String());
		String& front();
		// const String& front() const;

		String& back();
		// const String& back() const;

		void push_back(const String&);
		// void insert(Index, const String&);
		// void insert(Index, Size, const String&);
		void pop_back();
		// Index erase(Index);
		// Index erase(Index, Index);

		bool operator == (const PDBRecords&) const;
		bool operator != (const PDBRecords&) const;
		bool operator < (const PDBRecords&) const;
		bool operator > (const PDBRecords&) const;
		bool operator <= (const PDBRecords&) const;
		bool operator >= (const PDBRecords&) const;
		void swap(PDBRecords&);

		// const String& operator [] (Size) const;
		// String& operator [] (Size);
		// const String& at(Size) const throw(std::out_of_range);
		// String& at(Size) throw(std::out_of_range);
	};

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
		PDBRecords(const PDBRecords& /pdbr/) throw();
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

		void resize(Size /sz/, String /c/ = String());
		String& front();
		// const String& front() const;

		String& back();
		// const String& back() const;

		void push_back(const String& /x/);
		// void insert(Index /pos/, const String& /value/);
		// void insert(Index /pos/, Size /n/, const String& /value/);
		void pop_back();
		// Index erase(Index /pos/);
		// Index erase(Index /first/, Index /last/);

		bool operator == (const PDBRecords& /rhs/) const;
		bool operator != (const PDBRecords& /rhs/) const;
		bool operator < (const PDBRecords& /rhs/) const;
		bool operator > (const PDBRecords& /rhs/) const;
		bool operator <= (const PDBRecords& /rhs/) const;
		bool operator >= (const PDBRecords& /rhs/) const;
		void swap(PDBRecords& /rhs/);

		// const String& operator [] (Size /n/) const;
		// String& operator [] (Size /n/);
		// const String& at(Size /n/) const throw(std::out_of_range);
		// String& at(Size /n/) throw(std::out_of_range);
	};

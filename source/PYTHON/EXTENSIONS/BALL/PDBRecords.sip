class PDBRecords
{
%TypeHeaderCode
	#include <BALL/FORMAT/PDBRecords.h>
%End
	public:
	PDBRecords() throw();
	PDBRecords(const PDBRecords&) throw();
	virtual ~PDBRecords() throw();

	Size size() const;
	Size max_size() const;
	Size capacity() const;
	bool empty() const;
	void clear();

	void resize(Size, String c = String());
	String& front();
	String& back();
	void push_back(const String&);
	void pop_back();

	bool operator == (const PDBRecords&) const;
	bool operator != (const PDBRecords&) const;
	bool operator < (const PDBRecords&) const;
	bool operator > (const PDBRecords&) const;
	bool operator <= (const PDBRecords&) const;
	bool operator >= (const PDBRecords&) const;
	void swap(PDBRecords&);
};

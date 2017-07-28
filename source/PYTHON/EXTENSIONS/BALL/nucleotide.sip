class Nucleotide
	: Fragment
{
%TypeHeaderCode
	#include <BALL/KERNEL/nucleotide.h>
%End
	public:
	Nucleotide();
	Nucleotide(const Nucleotide&, bool deep = true);
	Nucleotide
		(const String&,
		const String& id = "" ,
		char insertion_code = ' ' );
	~Nucleotide() throw();

	virtual void clear() throw();
	virtual void destroy() throw();

	void set(const Nucleotide&, bool deep = true);
	void get(Nucleotide&, bool deep = true) const;
	void swap(Nucleotide&);

	NucleicAcid* getNucleicAcid();

	void setID(const String&);
	const String& getID() const;
	void setInsertionCode(char);
	void unsetInsertionCode();
	char getInsertionCode() const;
	void prepend(Atom& /Transfer/);
	void append(Atom& /Transfer/);
	void insert(Atom& /Transfer/);
	void insertBefore(Atom& /Transfer/, Composite&);
	void insertAfter(Atom& /Transfer/, Composite&);
	bool remove(Atom&);
	void spliceBefore(Nucleotide&);
	void spliceAfter(Nucleotide&);
	void splice(Nucleotide&);
	bool isTerminal() const;
	bool is3Prime() const;
	bool is5Prime() const;
	virtual bool isValid() const throw();
};

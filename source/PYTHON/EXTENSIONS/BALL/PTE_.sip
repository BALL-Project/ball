class Element
{
%TypeHeaderCode
	#include <BALL/KERNEL/PTE.h>
%End
	public:
	Element();
	Element(const Element&);
	void setName(const String&);
	const String& getName() const;
	void setSymbol(const String&);
	const String& getSymbol() const;
	void setGroup(int);
	int getGroup() const;
	void setPeriod(int);
	int getPeriod() const;
	void setAtomicNumber(int);
	int getAtomicNumber() const;
	void setAtomicWeight(float );
	float getAtomicWeight() const;
	void setAtomicRadius(float);
	float getAtomicRadius() const;
	void setCovalentRadius(float);
	float getCovalentRadius() const;
	void setVanDerWaalsRadius(float);
	float getVanDerWaalsRadius() const;
	void setElectronegativity(float);
	float getElectronegativity() const;
	bool operator == (const Element&) const;
	bool operator !=(const Element&) const;
	bool operator <(const Element&) const;
	bool operator <=(const Element&) const;
	bool operator >=(const Element&) const;
	bool operator >(const Element&) const;
	bool isUnknown() const;
};

class PTE_
{
%TypeHeaderCode
	#include <BALL/KERNEL/PTE.h>
%End
	public:
	PTE_();
	PTE_(const PTE_&);
	static Element& getElement(int);
	static Element& getElement(const String&);
	const Element& operator [] (const String&);
	const Element& operator [] (Index) const;
%MethodCode
  sipRes = &sipCpp->operator [] (*a0);
%End
};

// THE one PTE
PTE_ PTE;

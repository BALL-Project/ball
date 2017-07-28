class Selectable
{
%TypeHeaderCode
	#include <BALL/CONCEPT/selectable.h>
%End
	public:
	Selectable() throw();
	Selectable(const Selectable&, bool = true) throw();
	~Selectable() throw();
	void clear() throw();
	void set(const Selectable&, bool = true) throw();
	void get(Selectable&, bool = true) const throw();
	void swap(Selectable&) throw();
	virtual void select() throw();
	virtual void deselect() throw();
	bool isSelected() const throw();
};

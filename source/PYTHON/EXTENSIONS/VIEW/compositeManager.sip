class CompositeManager
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/compositeManager.h>
%End
	public:

	CompositeManager() throw();
	CompositeManager(const CompositeManager&) throw();
	~CompositeManager() throw();
	virtual void clear() throw();
	bool has(const Composite*) const throw();
	bool hasRoot(const Composite*) const throw();
	bool insert(Composite&) throw();
	void remove(Composite&, bool = true) throw();
	Size getNumberOfComposites() const throw();
	PyCompositeList getComposites() throw();
};

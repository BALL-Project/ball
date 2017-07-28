class Object
{
%TypeHeaderCode
	#include <BALL/CONCEPT/object.h>
%End
	public:
	Object() throw();
	Object(const Object&) throw();
	~Object() throw();
	virtual void clear() throw();
	Size getHandle() const throw();
	static Size getNextHandle() throw();
	static Size getNewHandle() throw();
	bool operator == (const Object&) const;
	bool operator != (const Object&) const;
	bool operator < (const Object&) const;
	bool operator <= (const Object&) const;
	bool operator >= (const Object&) const;
	bool operator > (const Object&) const;
	int compare(const Object&) const;
	virtual bool isValid() const throw();
};

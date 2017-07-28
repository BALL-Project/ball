class Embeddable
{
%TypeHeaderCode
	#include <BALL/CONCEPT/embeddable.h>
%End
	public:
	~Embeddable() throw();

	void unregisterThis() throw();
	virtual void registerThis() throw();
};

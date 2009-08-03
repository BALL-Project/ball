class MolecularInformation
{
%TypeHeaderCode
	#include <BALL/CONCEPT/molecularInformation.h>
	using namespace BALL;
%End
  public:
			
	enum Type
	{
		TYPE__UNKNOWN              = -1,
		TYPE__SYSTEM               = 0,
		TYPE__PROTEIN              = 1,
		TYPE__MOLECULE             = 2,
		TYPE__CHAIN                = 3,
		TYPE__FRAGMENT             = 4,
		TYPE__RESIDUE              = 5,
		TYPE__SECONDARY_STRUCTURE  = 6,
		TYPE__ATOM                 = 7,
		TYPE__BOND 								 = 8
	};

	MolecularInformation() throw();
  ~MolecularInformation()	throw();
	virtual void clear() throw();
	Type getType() const throw();
	String getName() const throw();
	String getTypeName() const throw();
	virtual void visit(Composite&);
};

#define BALL_NMR_PEAK

#include<BALL/KERNEL/PDBAtom.h>

#include <list>
using std::list;

using namespace std;

namespace BALL {

/**@name 	peak
*/

//@{

/**	represents the peak datastructure.
	each peak may contain several atoms, by now the {\tt atomlist\_} contains just one atom.
	Probably this structure will be changed in near future.
	There are several floats describing the peak, in fact only one of them is used, which
	is {\tt ppm\_} set to the chemical shift of the atom.
*/

class peak
	{
	private:
	
	float min_,max_;
	float ppm_,breadth_,height_;
	list<PDBAtom*> atomlist_;
	float anzahl_;
	
	public:

	/**@name	Constructors and Destructors
	*/
	
	//@{

	/**	Default Constructor
	*/
	peak();
	
	/**	Copy Constructor
	*/
	peak(peak,int);
	
	/**	Destructor
	*/
	~peak();
	
	//@}

	/**@name class special functions
	*/

	//@{

	/** returns the value of {\tt ppm\_} in fact the chemical shift
	*/
	float get_ppm() const;

	/** returns the value of {\tt breadth\_} the breadth of the peak, actually unused and set to 0
	*/
	float get_breadth();
	
	/** returns the value of {\tt height\_} the height of the peak, actually unused and set to 0
	*/
	float get_height();
	
	/** returns the value of {\tt raster\_} actually unused and set to 0
	*/
	float get_raster();
	
	/** returns the value of {\tt anzahl\_} the number of atoms of the peak
	*/
	float get_anzahl();
	
	/** returns the value of {\tt min\_} the lowest chemicalshift of atoms in {\tt atomlist\_}
	*/
	float get_min();
	
	/** returns the value of {\tt max\_} the highest chemicalshift of atoms in {\\t atomlist\_}
	*/
	float get_max();
	
	/** returns {\tt atomlist\_}
	*/
	list<PDBAtom*> get_atomlist();

	/** sets the value of {\tt ppm\_}
	*/
	void set_ppm(float);

	/** sets the value of {\tt breadth\_}
	*/
	void set_breadth(float);
	
	/** sets the value of {\tt height\_}
	*/
	void set_height(float);
	
	/** sets the value of {\tt raster\_}
	*/
	void set_raster(float);
	
	/** sets the value of {\tt anzahl\_}
	*/
	void set_anzahl(float);
	
	/** sets the value of {\tt min\_}
	*/
	void set_min(float);
	
	/** sets the value of {\tt max\_}
	*/
	void set_max(float);
	
	/** assigns a list of atoms to {\tt atomlist\_}
	*/
	void set_atomlist(list<PDBAtom*>);

	/** Assignment Operator
	*/
	void operator=(peak);

	/** adding a PDBAtom to {\tt atomlist\_} and appropriate reassignment of above parameters
	*/
	void add(PDBAtom*);
	
	//@}
	};

bool operator==(peak,peak);
bool operator<(peak,peak);
ostream& operator<<(ostream&,peak&);

//@}

} // namespace Ball


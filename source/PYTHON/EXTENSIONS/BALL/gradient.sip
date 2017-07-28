class Gradient
{
%TypeHeaderCode
	#include <BALL/MOLMEC/COMMON/gradient.h>
%End
	public:

	Gradient();
	Gradient(const AtomVector&);
	Gradient(const Gradient&, bool);

	void set(const Gradient&);
	void set(const AtomVector&);
	void negate();
	void normalize();
	int size() const ;
	void invalidate();
	bool isValid() const;
	double norm;
	double inv_norm;
	double rms;
};

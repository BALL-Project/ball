// $Id: peak.h,v 1.6 2001/03/02 22:19:33 amoll Exp $

#ifndef BALL_NMR_PEAK_H
#define BALL_NMR_PEAK_H

#include <iostream>

namespace BALL 
{
	
	class Atom;

	/**	1D Peak Class.
			Each peak contains a pointer to an associated atom 
			(in the case of NMR: the atom that causes this peak).	\\
			{\bf Definition}\URL{BALL/NMR/peak.h}
	*/
	class Peak1D
	{
		public:

		/** @name	Constructors and Destructors
		*/
		//@{

		/**	Default Constructor
		*/
		Peak1D();
		
		/**	Copy Constructor
		*/
		Peak1D(const Peak1D& peak);
		
		/**	Destructor
		*/
		~Peak1D();
		
		//@}
		/** @name Accessors
		*/
		//@{

		/** Return the peak position.
		*/
		float getValue() const;

		/** Return the peak width.
		*/
		float getWidth() const;
		
		/** Return the peak height (amplitude).
		*/
		float getHeight() const;
		
		/** Set the peak position.
		*/
		void setValue(const float& value);

		/** Set the peak width
		*/
		void setWidth(const float& width);
		
		/** Set the peak height
		*/
		void setHeight(const float& height);

		/**	Return the atom pointer.
		*/
		const Atom* getAtom() const;

		/**	Set the atom pointer.
		*/
		void setAtom(const Atom* atom);

		//@}
		/**	@name Assignment
		*/
		//@{

		/** Assignment Operator
		*/
		void operator = (const Peak1D& peak);

		//@}
		/**	@name Predicates
		*/
		//@{

		/**	Equality operator
		*/
		bool operator == (const Peak1D& peak) const;

		/**	Lesser than operator
		*/
		bool operator < (const Peak1D& peak) const;

		//@}

		protected:

		float value_;
		float width_;
		float height_;
		const Atom*	atom_;
	};

	/**	Output operator
	*/
	std::ostream& operator << (std::ostream& os, const Peak1D& peak);


} // namespace BALL

#endif // BALL_NMR_PEAK_H

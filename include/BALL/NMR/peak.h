// $Id: peak.h,v 1.3 2000/09/07 19:37:04 oliver Exp $

#ifndef BALL_NMR_PEAK_H
#define BALL_NMR_PEAK_H

#include <iostream>

namespace BALL 
{
	
	class Atom;

	/**@name 	One-dimensional (NMR) Peaks.
	*/

	//@{

	/**	1D Peak Class.
			Each peak contains a pointer to an associated atom (in the case of NMR: the atom that causes
      this peak).																													
	*/
	class Peak1D
	{
		public:

		/**@name	Constructors and Destructors
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

		/**@name Accessors
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
		void setValue(float value);

		/** Set the peak width
		*/
		void setWidth(float width);
		
		/** Set the peak height
		*/
		void setHeight(float height);

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
	ostream& operator << (ostream& os, const Peak1D& peak);

//@}

} // namespace BALL

#endif // BALL_NMR_PEAK_H

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_CONCEPT_VISITOR_H
#define BALL_CONCEPT_VISITOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{

	/**	Generic Visitor Class.
			This class implements the visitor design pattern.
			
			\ingroup ConceptsMiscellaneous
	*/
	//@{
	template <typename Host>
	class Visitor
	{
		public:

		/**	@name	Constructors and destructors
		*/	
		//@{
		/**	Default constructor
		*/
		Visitor()
			;

		/**	Copy constructor
		*/
		Visitor(const Visitor& visitor)
			;

		/**	Destructor
		*/
		virtual ~Visitor()
			;

		//@}

		/**	Visit method.				
				The visit method is an abstract method defining an 
				interface between the visitor object and its host.
		*/
		virtual void visit(Host &) = 0;

		virtual void dummy();
	};
	//@}
	
	template <typename T>
	void Visitor<T>::dummy(){}

	template <typename T>
	BALL_INLINE
	Visitor<T>::Visitor()
		
	{
	}

	template <typename Host>
	BALL_INLINE
	Visitor<Host>::Visitor(const Visitor<Host>& /* visitor */)
		
	{
	}

	template <typename T>
	Visitor<T>::~Visitor()
		
	{
	}
}

#endif // BALL_CONCEPT_VISITOR_H

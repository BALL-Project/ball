// $Id: visitor.h,v 1.1 1999/08/26 07:53:17 oliver Exp $

#ifndef BALL_CONCEPT_VISITOR_H
#define BALL_CONCEPT_VISITOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL {

	/**	Generic Visistor Class.
			This class implements the visitor design pattern.
			{\bf Definition:} \URL{BALL/CONCEPT/visitor.h}
	*/
	template <class Host>
	class Visitor
	{
		public:

		/**	@name Constructors and Destructors
		*/
		//@{

		/**	Default Constructor.
				The Default constructor has no functionality.
		*/
		Visitor()
		{
		}

		/**	Copy constructor.
				The copy constructor has no functionality.
		*/
		Visitor(const Visitor& /* visitor */, bool /* deep */ = false)
		{
		}

		/**	Destructor.
				The destructor has no functionality.
		*/
		virtual ~Visitor()
		{
		}
		//@}

		/**	Visit method.
				
		*/
		virtual void visit(Host &);

	};

	template <class Host>
	inline void Visitor<Host>::visit(Host &)
	{
	}
}

#endif // BALL_CONCEPT_VISITOR_H

// $Id: visitor.h,v 1.5 2001/05/18 02:50:40 oliver Exp $

#ifndef BALL_CONCEPT_VISITOR_H
#define BALL_CONCEPT_VISITOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{

	/**	Generic Visitor Class.
			This class implements the visitor design pattern.
			{\bf Definition:} \URL{BALL/CONCEPT/visitor.h}
	*/
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
			throw();

		/**	Copy constructor
		*/
		Visitor(const Visitor& visitor)
			throw();

		/**	Destructor
		*/
		virtual ~Visitor()
			throw();

		//@}

		/**	Visit method.				
				The visit method is an abstract method defining an 
				interface between the visitor object and its host.
		*/
		virtual void visit(Host &) = 0;

		virtual void dummy();
	};

	template <typename T>
	void Visitor<T>::dummy(){}

	template <typename T>
	BALL_INLINE
	Visitor<T>::Visitor()
		throw()
	{
	}

	template <typename T>
	BALL_INLINE
	Visitor<T>::Visitor(const Visitor<T>& /* visitor */)
		throw()
	{
	}

	template <typename T>
	Visitor<T>::~Visitor()
		throw()
	{
	}
}

#endif // BALL_CONCEPT_VISITOR_H

// $Id: predicate.h,v 1.7 2001/02/26 00:21:49 amoll Exp $

#ifndef BALL_KERNEL_PREDICATE_H
#define BALL_KERNEL_PREDICATE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_COMMON_RTTI_H
#	include <BALL/COMMON/rtti.h>
#endif

#ifndef BALL_CONCEPT_PREDICATE_H
#	include <BALL/CONCEPT/predicate.h>
#endif

namespace BALL 
{

	/**	@name	Kernel Predicates
			This chapter describes a set of unary predicates acting on \Ref{Composites}, that
			can be used to identify the class type of a composite at runtime.\\
			This predicates are functionally similar to the \Ref{RTTI:isKindOf} function.
			They decide whether a given composite is an instance of a certain class (or of
			a class derived thereof). For instance, the application of a \Ref{MoleculePredicate}
			to a composite will yield {\bf true}, if the given composite was an instance of \Ref{Molecule}
			(or \Ref{Protein}, as Protein is derived from Molecule). Otherwise false is returned.\\
			Each KernelPredicate possesses three methods:
			\begin{itemize}
				\item{\bf operator (const Composite\&)}
				\item{\bf operator (const Composite\&) const}
			\end{itemize}
			The two operators are functionally identical (two methods are needed, because the class
			is derived from TUnaryPredicate and we want to make sure both methods are overwritten).
			{\bf operator()} returns true, if the given composite is a kind of the class corresponding 
			to the respective predicate, false otherwise.\\
			{\bf Definition:} \URL{BALL/KERNEL/predicate.h}
			@see	UnaryPredicate
			@see	RTTI	
	*/
	//@{
	
	class Composite;

#ifndef BALL_KERNEL_PREDICATE_TYPE
#define BALL_KERNEL_PREDICATE_TYPE
	typedef UnaryPredicate<Composite>	KernelPredicateType;
#endif

	/// Kernel predicate class
	template <class T>
	class KernelPredicate
		: public UnaryPredicate<Composite>
	{
		public:

		/// Constant virtual RTTI evaluation operator
		virtual bool operator () (const Composite& composite) const
			throw();
	};

	template <class T>
	bool KernelPredicate<T>::operator () (const Composite& composite) const
		throw()
	{
		return RTTI::isKindOf<T>(composite);
	}

	//@}

} // namespace BALL


#endif // BALL_KERNEL_PREDICATE_H

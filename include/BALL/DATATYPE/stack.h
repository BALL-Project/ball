// $Id: stack.h,v 1.1.2.1 2002/03/05 08:34:44 anker Exp $
// Datatypes: a simple stack implementation

#ifndef BALL_DATATYPE_STACK_H
#define BALL_DATATYPE_STACK_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL
{

	/** A simple stack.
			{\bf Definition:} \URL{BALL/DATATYPE/stack.h}
	*/
	template <class T> 
	class Stack
	{
		public:
		
		BALL_CREATE(Stack)

		/** @name Constructors and destructor
		*/
		//@{

		/// Default constructor
		Stack()
			throw();
		
		/// Copy constructor
		Stack(const Stack& stack)
			throw();

		/// Destructor
		virtual ~Stack()
			throw();
		//@}
		/** @name Accessors
		*/
		//@{

		///
		void push(T value)
			throw();
		
		///
		T pop()
			throw();
		
		///
		T top() const
			throw();
		
		//@}
		/** @name Predicates
		*/
		//@{

		///
		bool isEmpty() const
			throw();
		
		///
		bool isFull() const
			throw();

		//@}

		private:

		Size capacity_;

		Index top_;

		T* stack_;

	};


	template <class T>
	Stack<T>::Stack()
		throw()
		: capacity_(1),
			top_(0),
			stack_(new T[1])
	{
	}

	template <class T>
	Stack<T>::Stack(const Stack& stack)
		throw()
		: capacity_(stack.capacity_),
			top_(stack.top_),
			stack_(new T[capacity_])
	{
		for (Index i = 0; i < top_; ++i)
		{
			stack_[i] = stack.stack_[i];
		}
	}

	template <class T>
	Stack<T>::~Stack()
		throw()
	{
		delete [] stack_;
	}

	template <class T>
	void
	Stack<T>::push(T value)
		throw()
	{
		// if we don't have enough space left on our stack, double its size
		if (top_ >= (Index) capacity_)
		{
			T *new_stack = new T [capacity_ << 1];

			for (Index i = 0; i < (Index) capacity_; ++i)
			{
				new_stack[i] = stack_[i];
			}

			delete [] stack_;
			stack_ = new_stack;
			capacity_ <<= 1;
		}

		stack_[top_++] = value;

#ifdef DEBUG
		cout << "Stack::push(): " << value << endl;
		
		if (top_ == 1)
		{
			testit = value;
		}

		for (Size i = 0; i < top_; ++i)
		{
			cout << i << ": " << stack_[i] << endl;
		}
		cout << endl;

		if (stack_[0] != testit)
		{
			cerr << "Stack inconsistent." << endl;
		}
#endif
	}


	template <class T>
	T
	Stack<T>::pop()
		throw()
	{
		// ?????
		// should we decrease the capacity again?
		if (isEmpty())
		{
			// ?????
			// What should we return if the stack is empty? should we return
			// anything at all? exception?
			cerr << "Stack is empty." << endl;
			return 0;
		}
		else
		{
#ifdef DEBUG
			cout << "Stack::pop(): " << stack_[top_ - 1] << endl;
			T value = stack_[--top_];
			for (Size i = 0; i < top_; ++i)
			{
				cout << i << ": " << stack_[i] << endl;
			}
			cout << endl;
			return value;
#else
			return stack_[--top_];
#endif
		}
	}

	template <class T>
	T
	Stack<T>::top() const
		throw()
	{
		if (isEmpty())
		{
			// ?????
			// What should we return if the stack is empty? should we return
			// anything at all? exception?
			cerr << "Stack is empty." << endl;
			return 0;
		}
		else
		{
#ifdef DEBUG
			for (Size i = 0; i < top_; ++i)
			{
				cout << i << ": " << stack_[i] << endl;
			}
			cout << endl;
#endif
			return stack_[top_ - 1];
		}
	}


	template <class T>
	bool Stack<T>::isEmpty() const
		throw()
	{
		if (top_ == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

}
#endif // BALL_DATATYPE_STACK_H

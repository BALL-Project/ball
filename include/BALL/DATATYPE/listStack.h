// $Id: listStack.h,v 1.2 2000/01/03 15:16:18 oliver Exp $

#ifndef BALL_DATATYPE_LISTSTACK_H
#define BALL_DATATYPE_LISTSTACK_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{

	// all these classes are used to implement the binary search tree
	
	class StackListItem 
	{
		public:
		
			StackListItem()
				:	next(0)	
			{
			}

			void insertAfter(StackListItem *item)
			{
				item->next = next;
				next = item;
			}

			StackListItem* removeNext()
			{
				StackListItem *item = next;
				next = item->next;
				return item;
			}

			StackListItem* next;
	};



	class StackList
		: public StackListItem 
	{
		public:

		virtual void clear()
		{
			register StackListItem* p = next;
			register StackListItem* q = 0;
			while(!isHead(p)) 
			{
				q = p->next;
				deleteItem(p);
				p = q;
			}
			clear_();
		}

		bool isEmpty() const
		{
			return (bool)((const StackListItem *)next == this);
		}

		bool isHead(const StackListItem* item) const
		{
			return (bool)(item == this);
		}

		protected:

		StackList()
			:	StackListItem(),
				back(0)
		{
			clear_();
		}

		virtual ~StackList()
		{
			clear();
		}
		
		StackListItem* getHeader()
		{
			return this;
		}

		const StackListItem* getHeader() const
		{
			return this;
		}

		StackListItem* getFirst()
		{
			return next;
		}

		const StackListItem* getFirst() const
		{
			return next;
		}
		
		StackListItem* getLast()
		{
			return back;
		}
		
		const StackListItem* getLast() const
		{
			return back;
		}

		void set(const StackList& stack_list)
		{
			if (this != &stack_list)
			{
				clear();
				append(stack_list);
			}
		} 
		
		void get(StackList& stack_list) const
		{
			stack_list.set(*this);
		} 
		
		void append(const StackList& stack_list)
		{
			if (this == &stack_list)
			{
				return;
			}
			
			const StackListItem* item = stack_list.next;
			while(!stack_list.isHead(item)) 
			{ 
				append(newItem(item));
				item = item->next;
			}
		}

		void insertAfter(StackListItem* p,  StackListItem* q)
		{
			p->insertAfter(q);
			if (p == back)
			{
				back = q;
			}
		}

		StackListItem* removeNext(StackListItem* item)
		{
			if (item->next == this)
			{
				return 0;
			}
			StackListItem* p = item->removeNext();
			if (p == back)
			{
				back = item;
			}
			return p;
		}
		
		StackListItem* removeFront()
		{
			return removeNext(this);
		}

		void prepend(StackListItem* item)
		{
			insertAfter(this, item);
		}

		void append(StackListItem* item)
		{
			insertAfter(back, item);
		}
		
		void splice(StackList& stack_list)
		{
			if (stack_list.isEmpty()
					|| this == &stack_list)
			{
				return;
			}
			back->next = stack_list.next;
			stack_list.back->next = this;
			back = stack_list.back;
			stack_list.clear_();
		}

		void splitAfter(StackListItem* item,  StackList& stack_list)
		{
			if (this == &stack_list || item == back)
			{
				return;
			}
			stack_list.back->next = item->next;
			stack_list.back = back;
			back->next = &stack_list;
			item->next = this;
			back = item;
		}

		virtual StackListItem* newItem(const StackListItem* item) = 0;

		virtual void deleteItem(StackListItem* item) = 0;

		StackListItem* back;

		private:

		void clear_()
		{
			next = this;
			back = this;
		}
	};



	template<class DataType>
	class TStackListItem
		: public StackListItem 
	{
		public:
		
			TStackListItem()
				:	StackListItem()
			{
			}

			TStackListItem(const DataType& data)
				:	StackListItem(),
					data_(data)
			{
			}

			TStackListItem* getNext()
			{
				return (TStackListItem*)next;
			}

			const TStackListItem* getNext() const
			{
				return (TStackListItem*)next;
			}

			DataType data_;
	};



	template<class DataType>
	class TSStack 
		: public StackList 
	{
		public:
		
		TSStack()
			:	StackList()
		{
		}

		TSStack(const TSStack& stack)
			:	StackList()
		{
			set(stack);
		}

		virtual ~TSStack()
		{
			rewind();
		}

		TSStack& operator = (const TSStack& stack)
		{
			set(stack);

			return *this;
		}

		void push(const DataType& data)
		{
			TStackListItem<DataType>* p = newItem(data);
			prepend(p);
		}

		bool pop(DataType& data)
		{
			TStackListItem<DataType>* item = (TStackListItem<DataType> *)removeFront();
			if (item == 0)
			{
				return false;
			}
			data = item->data_;
			deleteItem(item);
			return true;
		}

		bool pop()
		{
			if (isEmpty())
			{
				return false;
			}
			deleteItem(removeFront());
			return true;
		}

		DataType* peek(Size size)
		{
			TStackListItem<DataType> *item = (TStackListItem<DataType> *)getFirst();
			while(!isHead(item)) 
			{
				if (size-- == 0)
				{
					return &(item->data_);
				}
				item = item->getNext();
			}
			return 0;
		}

		const DataType* peek(Size size) const
		{
			return ((TSStack *)this)->peek(size);
		}

		DataType* top()
		{
			return isEmpty() ? 0 : &(((TStackListItem<DataType> *)getFirst())->data_);
		}

		const DataType* top() const
		{
			return ((TSStack *)this)->top(); 
		}

		void rewind(Size size = 0)
		{
			if (size == 0)
			{
				size = SIZE_MAX;
			}
			register TStackListItem<DataType>* p = (TStackListItem<DataType>*)next;
			register TStackListItem<DataType> *q = 0;
			while(!isHead(p) && size--) 
			{
				q = p->getNext();
				deleteItem(p);
				p = q;
				next = q;
			}
		}

		void pushBottom(const DataType& data)
		{
			TStackListItem<DataType>* item = newItem(data);
			append(item);
		}

		virtual void clear()
		{
			rewind();
		}

		bool isFull() const
		{  
			return false;
		}

		protected:
		
		virtual TStackListItem<DataType>* newItem(const DataType& data)
		{
		 return new TStackListItem<DataType>(data);
		}

		virtual StackListItem* newItem(const StackListItem* item)
		{
			return new TStackListItem<DataType>(((TStackListItem<DataType>*)item)->data_);
		}

		virtual void deleteItem(StackListItem* item)
		{
			delete (TStackListItem<DataType> *)item;
		}
	};

} // namespace BALL

#endif // BALL_DATATYPE_LISTSTACK_H

#include <BALL/CONCEPT/comparator.h>
#include <BALL/DATATYPE/list.h>

#include <iostream>

using namespace BALL;

/** Helper class: a processor counting items.
		The items addresses are stored in a list.
		This class is used in several tests for container::apply() methods.
*/
template<typename DataType>
class ItemCollector	: public UnaryProcessor<DataType>
{
	public:

	/// Get the list of pointers.
	list<DataType*>& getList()
	{
		return list_;
	}
	
	/// Get the pointer assigned by the iterator.
	const DataType* getPointer()
	{
		if (list_it_ == list_.end())
		{
			return 0;
		}
		return *list_it_;
	}

	/** Forward the iterator.
	*/
	void forward()
	{	
		list_it_++;
	}

	/// get the size of the list
	Size getSize()
	{	
		return list_.size();
	}
	
	/** Reset the iterator to the first element of the list.
	*/
	void reset()
	{ // 
		list_it_ = list_.begin();
	}


	private:

	/** Start the processor.
			This method is called by the apply()-method
			of the container-classes to be tested.
	*/
	bool start()
	{
		// clear the item list
		list_.clear();
		list_it_ = list_.begin();
		return true;
	}

	/** Finish the processor.
			This method is called by the apply()-method
			of the container-classes to be tested.
	*/
	bool finish()
	{
		list_it_ = list_.begin();
		return true;
	}

	/** Store the item.
			This method is called by the apply()-method
			of the container-classes to be tested.
	*/
	Processor::Result operator () (DataType& item)
	{
		list_.push_back(&item);
		return Processor::CONTINUE;
	}

	List<DataType*>											list_;
	typename List<DataType*>::iterator	list_it_;
};

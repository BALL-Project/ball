#include <BALL/CONCEPT/comparator.h>
#include <BALL/DATATYPE/list.h>

using namespace BALL;

// helper class: a processor counting items in a list
template<typename DataType>
class ItemCollector	: public UnaryProcessor<DataType>
{
	public:
	bool start()
	{
		// clear the item list
		list_.clear();
		list_it_ = list_.begin();
		return true;
	}

	bool finish()
	{
		return true;
	}

	Processor::Result operator () (DataType& item)
	{	// store the item
		list_.push_back(&item);
		return Processor::CONTINUE;
	}

	list<DataType*> getList()
	{ // get a pointer to the list
		return list_;
	}
	
	DataType* getPointer()
	{	// get a pointer to the first element in the list
		if (list_it_ == list_.end())
		{
			return 0;
		}
		DataType* temp = *list_it_;
		return temp;
	}

	void forward()
	{	
		list_it_++;
	}


	Size getSize()
	{	// get the size of the list
		return list_.size();
	}
	
	void reset()
	{ // reset the iterator to the first element of the list
		list_it_ = list_.begin();
	}

	private:
	List<DataType*>	list_;
	typename List<DataType*>::iterator list_it_;
};

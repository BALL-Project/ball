#include <BALL/CONCEPT/processor.h>

namespace BALL
{

	class TestProcessor
		:	public UnaryProcessor<int>
	{
		virtual Processor::Result operator () (int& arg)
			throw()
		{
			arg++;
			return Processor::CONTINUE;
		}
	};
}

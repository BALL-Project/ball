// $Id: rules.h,v 1.3 2001/06/21 08:58:41 amoll Exp $

#ifndef BALL_STRUCTURE_RULES_H
#define BALL_STRUCTURE_RULES_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{

	/**	@name	Rule-based assignments.
			{\bf Definition:}\URL{BALL/STRUCTURES/rules.h}
	*/
	class RuleProcessor
		:	public UnaryProcessor<Atom> 
	{

		public:

		AssignRadiusProcessor();

		AssignRadiusProcessor(const String& filename);

		virtual bool start();

		virtual bool finish();

		virtual Processor::Result operator()(Atom& atom);

		/**	Set the filename to read the charges from
		*/
		void setFilename(const String& filename);

		/**	Return the current filename
		*/
		String& getFilename();
		
		/**	Return the number of assigned atoms
		*/
		unsigned long getNumberOfAssignments();

		/**	Return the number of unassignable atoms
		*/
		unsigned long getNumberOfErrors();


		protected:

		bool buildTable_();

		String									filename_;

		StringHashMap<float>		table_;

		unsigned long						number_of_errors_;
		unsigned long						number_of_assignments_;
	};


} // namespace BALL

#endif // BALL_STRUCTURE_DEFAULTPROCESSORS_H

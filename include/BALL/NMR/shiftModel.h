// $Id: shiftModel.h,v 1.1 2000/09/18 10:32:15 oliver Exp $

#ifndef BALL_NMR_SHIFTMODEL_H
#define BALL_NMR_SHIFTMODEL_H

#ifndef BALL_NMR_SHIFTMODULE_H
#	include <BALL/NMR/shiftModule.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
#	include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_FORMAT_PARAMETERS_H
#	include <BALL/FORMAT/parameters.h>
#endif

namespace BALL 
{

	/**	A class representing a complete parameterized NMR shift model.
			The model consists of a set of single contributions (e.g. ring curent,
			magnetic anisotropy, etc.). Each of these contributions is calculated
			by a separate \Ref{ShiftModule} object. ShiftModel contains a list of 
			these processors and is also derived from ShiftModel. Hence, it can
			be applied to arbitrary kernel objects in the same fashion as \Ref{ShiftModule}s.
			\\
			{\bf Definition:}\URL{BALL/NMR/shiftModel.h}
			\\
	*/
	class ShiftModel 
		: public ShiftModule		
	{
		public:	

		BALL_CREATE(ShiftModel)

		/**	@name	Type definitions
		*/
		//@{

		/**	The module list type
		*/
		typedef List<ShiftModule*> ModuleList;
		//@}

		/**	@name	Constructors and Destructors
		*/
		//@{
		
		/** 	Default Constructor
		*/
		ShiftModel()
			throw();

		/** 	Detailed Constructor
		*/
		ShiftModel(const String& filename)
			throw();

		/**	Copy constructor
		*/
		ShiftModel(const ShiftModel& model)
			throw();

		/**	Destructor
		*/
		virtual ~ShiftModel()
			throw();

		/**	Clear method.
				This method destructs all shift modules in the
				module list.Then, it clears the module list, the parameters, 
				and the options.
			
		*/
		void clear()
			throw();

		//@}

		/**	@name Accessors
		*/	
		//@{
		/**
		*/
		Parameters& getParameters()
			throw();

		/**	Return the list of modules
		*/
		ModuleList& getModuleList()
			throw();

		/**	Set the parameter filename.
		*/
		void setFilename(const String& filename)
			throw(Exception::FileNotFound);

		/**	Return the parameter filename.
		*/
		const String& getFilename() const
			throw();
		
		//@}

		/**	@name	Predicates
		*/
		//@{
			
		/**	Validity flag
		*/
		bool isValid() const
			throw();
		//@}

		/**	@name	Assignment
		*/
		//@{
			
		/**	Assignment operator
		*/
		const ShiftModel& operator = (const ShiftModel& model)
			throw();

		/**	Assignment operator (via filename).
		*/
		const ShiftModel& operator = (const String& filename)
			throw();
		//@}

		/**	@name Public Attributes
		*/
		//@{

		/**	Options
		*/
		Options options;

		//@}

		protected:

		bool init_()
			throw();
		
		Parameters	parameters_;

		ModuleList	modules_;

		bool valid_;
	};

} // namespace BALL

#endif // BALL_NMR_SHIFTMODEL_H

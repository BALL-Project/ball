// $Id: shiftModel.h,v 1.4 2000/09/25 19:09:54 oliver Exp $

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

		/**	The creation method type.
				This type describes a method that can construct an object
				of type ShiftModule.
		*/
		typedef void * (*CreateMethod) ();

		/**	The creation method hash map type.
				This type is used internally to store the creation
				method corresponding to a certaion symbolic name
				(usually the class name).
		*/
		typedef StringHashMap<CreateMethod>	CreateMethodMap;
		//@}

		/**	@name Constants
		*/
		//@{
		/**	The name of the section containing the module types and names of the model
		*/
		static const char* MODULE_LIST_SECTION;
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
		
		/**	Register a new module type.
				Add the 
		*/
		void registerModule(const String& name, CreateMethod method) 
			throw(Exception::NullPointer);

		/**	Unregister a module type.
		*/
		void unregisterModule(const String& name) 
			throw();
		//@}

		/**	@name	Predicates
		*/
		//@{
			
		/**	Validity flag
		*/
		bool isValid() const
			throw();

		/**	Check whether a module of this name is registered
		*/
		bool isRegistered(const String& name) const
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

		/**	@name	Processor related methods
		*/
		//@{

		/**	Start method
		*/
		bool start()
			throw();

		/**	Finish method
		*/
		bool finish()
			throw();

		/**	Application method
		*/
		Processor::Result operator () (Composite& composite)
			throw();
		//@}

		protected:

		/**	Initialize the model from the parameter file.
				This method assumes that object has a valid
				parameter file assigned. It sets {\tt valid_} 
				to {\bf true} if it could create a shfit model 
				from the contents of the parameter file.
		*/
		bool init_()
			throw(Exception::FileNotFound);

		/**	Create a ShiftModule from a symbolic name.
				This method create a shift module from the symbolic
				name if this name is contained in the hash map \Ref{registered_modules_}.
		*/
		ShiftModule* createModule_(const String& type, const String& name) const
			throw();

		/**	Register the standard modules.
		*/
		void registerStandardModules_()
			throw();
		
		/**	The parameters object
		*/
		Parameters				parameters_;

		/**	The list of shift modules of this model
		*/
		ModuleList				modules_;

		/**	A hash map containing all registered module types and their creation methods.
		*/
		CreateMethodMap	registered_modules_;

		/**	The validity flag.
				Set to {\bf true} if the object was initialized correctly.
		*/
		bool valid_;
	};

} // namespace BALL

#endif // BALL_NMR_SHIFTMODEL_H

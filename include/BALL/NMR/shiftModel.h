// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: shiftModel.h,v 1.17 2005/12/23 17:01:56 amoll Exp $
//

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
			by a separate  \link ShiftModule ShiftModule \endlink  object. ShiftModel contains a list of 
			these processors and is also derived from ShiftModul. Hence, it can
			be applied to arbitrary kernel objects in the same fashion as  \link ShiftModule ShiftModule \endlink s.	 \par
		\ingroup ShiftModel	
	*/
	class BALL_EXPORT ShiftModel 
		: public ShiftModule		
	{
		public:	

		BALL_CREATE(ShiftModel)

		/**	@name	Type definitions
		*/
		//@{

		/**	The module list type
		*/
		typedef std::list<ShiftModule*> ModuleList;

		/**	The creation method type.
				This type describes a method that can construct an object
				of type ShiftModule.
		*/
		typedef void * (*CreateMethod) ();

		/**	The creation method hash map type.
				This type is used internally to store the creation
				method corresponding to a certain symbolic name (usually the class name).
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
		
		/** Default Constructor
		*/
		ShiftModel();

		/** Detailed Constructor
		*/
		ShiftModel(const String& filename);

		/**	Copy constructor
		*/
		ShiftModel(const ShiftModel& model);

		/**	Destructor
		*/
		virtual ~ShiftModel();

		/**	Clear method.
				This method destructs all shift modules in the module list.
				Then, it clears the module list, the parameters, and the options.		
		*/
		void clear();

		//@}
		/**	@name Accessors
		*/	
		//@{

		/**
		*/
		Parameters& getParameters();

		/**	Return the list of modules
		*/
		ModuleList& getModuleList();

		/**	Set the parameter filename.
		*/
		void setFilename(const String& filename)
			throw(Exception::FileNotFound);

		/**	Return the parameter filename.
		*/
		const String& getFilename() const;
		
		/**	Register a new module type.
		*/
		void registerModule(const String& name, CreateMethod method) 
			throw(Exception::NullPointer);

		/**	Unregister a module type.
		*/
		void unregisterModule(const String& name) ;

		//@}
		/**	@name	Predicates
		*/
		//@{
			
		/**	Validity flag
		*/
		bool isValid() const;

		/**	Check whether a module of this name is registered
		*/
		bool isRegistered(const String& name) const;

		//@}
		/**	@name	Assignment
		*/
		//@{
			
		/**	Assignment operator
		*/
		const ShiftModel& operator = (const ShiftModel& model);

		/**	Assignment operator (via filename).
		*/
		const ShiftModel& operator = (const String& filename);

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
		bool start();

		/**	Finish method
		*/
		bool finish();

		/**	Application method
		*/
		Processor::Result operator () (Composite& composite);
		//@}

		protected:

		/*_	Initialize the model from the parameter file.
				This method assumes that object has a valid	parameter file assigned.
				It sets {\tt valid_} to <b>  true </b> if it could create a shift model 
				from the contents of the parameter file.
		*/
		bool init_()
			throw(Exception::FileNotFound);

		/*_	Create a ShiftModule from a symbolic name.
				This method create a shift module from the symbolic
				name if this name is contained in the hash map \Ref{registered_modules_}.
		*/
		ShiftModule* createModule_(const String& type, const String& name) const;

		/*_	Register the standard modules.
		*/
		void registerStandardModules_();
		
		/*_	The parameters object
		*/
		Parameters parameters_;

		/*_	The list of shift modules of this model
		*/
		ModuleList modules_;

		/*_	A hash map containing all registered module types and their creation methods.
		*/
		CreateMethodMap	registered_modules_;

		/*_	The validity flag.
				Set to <b>  true </b> if the object was initialized correctly.
		*/
		bool valid_;
	};
  
} // namespace BALL

#endif // BALL_NMR_SHIFTMODEL_H

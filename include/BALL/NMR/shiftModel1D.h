#ifndef BALL_NMR_SHIFTMODEL1D_H
#define BALL_NMR_SHIFTMODEL1D_H

#ifndef BALL_NMR_SHIFTMODULE_H
#	include <BALL/NMR/shiftModule.h>
#endif

#ifndef BALL_NMR_SPECTRUM_H
	#include <BALL/NMR/spectrum.h>
#endif


namespace BALL 
{
	/**	A class representing a complete parameterized 1D NMR shift model.
	The model consists of peak list, a system, a spectrum type and parameters
	*/
	class BALL_EXPORT ShiftModel1D 
		: public ShiftModule	
	{	
		
		public:
		BALL_CREATE(ShiftModel1D)
		
		/**	@name	Type definitions
		*/
		//@{

		enum SPECTRUM_TYPE{
			H = 1, 
			N,
			C,
			H_ON_BACKBONE,
			N_BACKBONE,
			C_BACKBONE
		};

		//@}
		/**	@name	Constructors and Destructors
		*/
		//@{
		
		/** Default Constructor
		*/
		ShiftModel1D(); 

		/** Detailed Constructor.
		 *  If compute_shifts is set to false, we assume that the shifts have been previously assigned, and will use the
		 *  properties as we find them in the system.
		*/
		ShiftModel1D(const String& filename,SPECTRUM_TYPE st, bool compute_shifts=true);
				
		/** Detailed Constructor.
		 *  If compute_shifts is set to false, we assume that the shifts have been previously assigned, and will use the
		 *  properties as we find them in the system.
		*/
		ShiftModel1D(const String& filename,SPECTRUM_TYPE st, double origin, double dimension, double spacing, bool compute_shifts=true);
	
		/**	Copy constructor
		*/
		ShiftModel1D(const ShiftModel1D& model);

		/**	Destructor
		*/
		virtual ~ShiftModel1D();
		
		/**	Clear method.	
		*/
		void clear();

		//@}
		/**	@name Accessors
		*/	
		//@{
		
		/**	Set the parameter filename.
		*/
		void setFilename(const String& filename)
			throw(Exception::FileNotFound);

		/**	Return the parameter filename.
		*/
		const String& getFilename() const
		{
			return parameters_.getFilename();
		};
		
		/**
		*/
		const Parameters& getParameters() const
		{
			return parameters_;
		};

		/**	Get the peaks
		*/
		const std::vector<Peak1D>& getPeaks() const
		{
			return peaks_;
		};
		
		/**	Set the peaks
		*/
		void setPeaks(std::vector<Peak1D> peaks)	
		{
			peaks_ = peaks;
		};
			
		
		/**	Get the origin
		*/
		const double& getOrigin() const
		{
			return origin_;
		};
 		
		/**	Set the origin
		*/
		void setOrigin(const double& origin) 
		{
			origin_ = origin;
		};
 
		/**	Get the dimension
		*/
		const double& getDimension() const
		{ 
			return dimension_;
		};

		/**	Set the dimension 
		*/
		void setDimension(	const double& dimension) 
		{ 
			dimension_ = dimension; 
		}

		/**	Get the spacing
		*/
		const double getSpacing() const
		{
			return spacing_;
		};

		/**	Set the system
		*/
		void setSystem( System* system)
		{
			system_ = system;
		};
	
		/**	Return the system
		*/
		const System* getSystem() const
		{
			return system_;
		};
		

		void operator >> (Spectrum1D& spectrum);

		//@}
		/**	@name	Predicates
		*/
		//@{
			
		/**	Validity flag
		*/
		bool isValid() const;

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

		/*_ The spectrum peaks
		 * */
		std::vector<Peak1D> peaks_;	
		
		/*_ The origin of the data
		*/
		double             origin_;	
		
		/*_ Dimension of the data
		*/
		double             dimension_;	
		
		/*_ spacing of the data
		 */
		double             spacing_;
			
		/*_  spectrum type
		 * */
		SPECTRUM_TYPE 			type_;
		
		/*_	The parameters object ??  Why do we need them???  evtl durch String filename ersetzten
		*/
		Parameters				parameters_;

		/*_ pointer to the system
		 */
		System* 					system_;
		
		/*_	The validity flag.
				Set to <b>  true </b> if the object was initialized correctly.
		*/
		bool valid_;

		/// Flag for shift computation
		bool compute_shifts_;

		bool hasType_(Atom* a, SPECTRUM_TYPE type);

	}; // end of class

} // namespace BALL

#endif // BALL_NMR_SHIFTMODEL1D_H

#ifndef BALL_NMR_SHIFTMODEL2D_H
#define BALL_NMR_SHIFTMODEL2D_H

#ifndef BALL_NMR_SHIFTMODULE_H
#	include <BALL/NMR/shiftModule.h>
#endif

#ifndef BALL_NMR_SPECTRUM_H
	#include <BALL/NMR/spectrum.h>
#endif

#include <vector>

namespace BALL 
{
	/**	A class representing a complete parameterized 2D NMR shift model.
	The model consists of peak list, a system, a spectrum type and parameters
	*/
	class BALL_EXPORT ShiftModel2D 
		: public ShiftModule	
	{	
		public:
		
		BALL_CREATE(ShiftModel2D)
		
		/**	@name	Type definitions
		*/
		//@{

		enum SPECTRUM_TYPE{
			HSQC_NH = 1, 
			HSQC_CH, 
			COSY_HH, // ist noch nicht implementiert!!! muesste ueber 2 Bindungslaengen spin = 1/2 pruefen.... 
			COSY_CH,  //
			TOCSY_HH, 
			TOCSY_CH, 
			TOCSY_NH
		};
		
		//@}
		/**	@name	Constructors and Destructors
		*/
		//@{
		
		/** Default Constructor
		*/
		ShiftModel2D();

		/** Detailed Constructor.
		 *  If compute_shifts is set to false, we assume that the shifts have been previously assigned, and will use the
		 *  properties as we find them in the system.
		 */
		ShiftModel2D(const String& filename,SPECTRUM_TYPE st, bool compute_shifts=true);
		
		/** Detailed Constructor.
		 *  If compute_shifts is set to false, we assume that the shifts have been previously assigned, and will use the
		 *  properties as we find them in the system.
		 */
		ShiftModel2D(const String& filename,SPECTRUM_TYPE st, Vector2 origin, Vector2 dimension, Vector2 spacing, bool compute_shifts=true);	
		
		/**	Copy constructor
		*/
		ShiftModel2D(const ShiftModel2D& model);

		/**	Destructor
		*/
		virtual ~ShiftModel2D();
		
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
		const std::vector<Peak2D>& getPeaks() const
		{
			return peaks_;
		};
		
		/**	Set the peaks
		*/
		void setPeaks(std::vector<Peak2D> peaks)	
		{
			peaks_ = peaks;
		};
			
		/**	Get the origin
		*/
		const Vector2& getOrigin() const
		{
			return origin_;
		};
 		
		/**	Set the origin
		*/
		void setOrigin(const Vector2& origin) 
		{
			origin_ = origin;
		};
 
		/**	Get the dimension
		*/
		const Vector2& getDimension() const
		{ 
			return dimension_;
		};

		/**	Set the dimension 
		*/
		void setDimension(const Vector2& dimension) 
		{ 
			dimension_ = dimension; 
		}

		/**	Get the spacing
		*/
		const Vector2& getSpacing() const
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
		

		void operator >> (Spectrum2D& spectrum);
		
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
		
		void createPeak_(Atom* proton, Atom* atom, float peakwidth_proton, float peakwidth_atom);
		/*_ The spectrum peaks
		 * */
		std::vector<Peak2D> peaks_;	
		
		/*_ The origin of the data
		*/
		Vector2             origin_;	
		
		/*_ Dimension of the data
		*/
		Vector2             dimension_;	
		
		/*_ spacing of the data
		 */
		Vector2             spacing_;
	
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

	};
	
	std::ostream& operator << (std::ostream& os, const BALL::ShiftModel2D& shiftmodel);

} // namespace BALL

#endif // BALL_NMR_SHIFTMODEL2D_H

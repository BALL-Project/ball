// $Id: experiment.h,v 1.2 2001/12/30 13:28:41 sturm Exp $

#ifndef BALL_NMR_EXPERIMENT_H
#define BALL_NMR_EXPERIMENT_H

#ifndef BALL_NMR_PEAKLIST_H
#	include <BALL/NMR/peakList.h>
#endif

#ifndef BALL_NMR_SHIFTMODULE_H
#	include <BALL/NMR/shiftModule.h>
#endif

#ifndef BALL_KERNEL_EXPRESSION_H
#	include <BALL/KERNEL/expression.h>
#endif

namespace BALL 
{

	/**	Class describing a certain type of NMR experiment.
			This class (and its subclasses) are used to describe
			how a list of peaks is derived from the shifts calculated
			for a system.
			\\
			{\bf Definition}\URL{BALL/NMR/experiment.h}
			\\
	*/
	template <typename PeakListType>
	class Experiment
		:	public ShiftModule
	{
		public:
	
		BALL_CREATE(Experiment)

		/**	@name	Typedefs
		*/
		//@{
		/// The peak type
		typedef typename PeakListType::PeakType PeakType;
		//@}

		/** @name	Constructors and Destructors
		*/
		//@{

		/**	Default Constructor
		*/	
		Experiment();
		/**	Copy constructor
		*/
		Experiment(const Experiment& experiment);
		/**	Destructor
		*/
		virtual ~Experiment()
			throw();
		//@}
		
		/** @name Accessors
		*/
		//@{
		/** Return the peak list
		*/
		const PeakListType& getPeakList() const;

		/**	Return the default peak.
				This peak can be assigned a default width and intensity 
				that is assigned to all extracted peaks in the peak list.
				This is the default behaviour and can be overridden in
				derived classes.
		*/
		const PeakType& getDefaultPeak() const;
			
		/**	Assign the default peak.
				@see getDefaultPeak
		*/
		void setDefaultPeak(const PeakType& peak);
		//@}

		protected:
		PeakListType	peak_list_;
		PeakType			default_peak_;
	};
	
	template <typename PeakListType>
	Experiment<PeakListType>::Experiment()
		:	ShiftModule(),
			peak_list_(),
			default_peak_()
	{
	}
	
	template <typename PeakListType>
	Experiment<PeakListType>::Experiment(const Experiment<PeakListType>& experiment)
		:	ShiftModule(experiment),
			peak_list_(experiment.peak_list_),
			default_peak_(experiment.default_peak_)
	{
	}

	template <typename PeakListType>
	Experiment<PeakListType>::~Experiment()
		throw()
	{
	}

	template <typename PeakListType>
	const PeakListType& Experiment<PeakListType>::getPeakList() const
	{
		return peak_list_;
	}
	
	template <typename PeakListType>
	const Experiment<PeakListType>::PeakType& Experiment<PeakListType>::getDefaultPeak() const
	{
		return default_peak_;
	}

	template <typename PeakListType>
	void Experiment<PeakListType>::setDefaultPeak(const Experiment<PeakListType>::PeakType& peak) 
	{
		default_peak_ = peak;
	}

	/**	@name	Convenience typedefs
	*/
	//@{
	/**	1D NMR experiment
	*/
	typedef Experiment<PeakList1D>	Experiment1D;

	/**	2D NMR experiment
	*/
	// ?????
	//typedef Experiment<PeakList2D>	Experiment2D;

	/**	3D NMR experiment
	*/
	typedef Experiment<PeakList3D>	Experiment3D;
	//@}

	/**	Simple 1D NMR experiment class.
			This experiment extracts all atoms with an assigned
			shift value, which match a given kernel expression.
			The default expression is {\bf "true()"}, so by default
			all atoms with an assigned shift value are added to 
			the peak list. The default peak intensity and width
			are user-defined (\Ref{setDefaultPeak}).
	*/
	class SimpleExperiment1D
		:	public Experiment1D
	{
		public:

		/**	@name Constructors and destructors
		*/
		//@{
		/// Default constructor
		SimpleExperiment1D();

		/// Copy constructor
		SimpleExperiment1D(const SimpleExperiment1D& experiment);
		
		/// Destructor
		virtual ~SimpleExperiment1D()
			throw();
		//@}
		
		/**	@name Processor related methods
		*/
		//@{
		/**	Initialize the experiment.
				This method simply clears the internal peak list.
				@return always {\bf true}
		*/
		virtual bool start()
			throw();
		/**	Processor application operator.
				This operator creates a new peak in the peak list for
				every atom it encounters that possesses shift data
				and matches the expression set with \Ref{setExpression}.
				The default is an empty expression, which matches all atoms.
				Each peak in the peak list is assigned the intensity and width	
				defined using \Ref{setDefaultPeak} (defaults to zero).
		*/
		virtual Processor::Result operator () (Composite& composite)
			throw();
		//@}

		/**	@name Accessors
		*/
		//@{
		/**
		*/
		void setExpression(const String& expression);
		/**
		*/
		const Expression& getExpression() const;
		//@}

		protected:
		Expression expression_;
	};
} //namespace BALL
	 
#endif // BALL_NMR_EXPERIMENT_H

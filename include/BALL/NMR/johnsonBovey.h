// $Id: johnsonBovey.h,v 1.3 2000/03/26 12:37:53 oliver Exp $

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_CONCEPT_OBJECT_H
#	include <BALL/CONCEPT/object.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
#	include <BALL/KERNEL/residue.h>
#endif

#ifndef BALL_KERNEL_PDBATOM_H
#	include <BALL/KERNEL/PDBAtom.h>
#endif

#include <list>
using std::list;

namespace BALL 
{
		
	/**@name	Johnson Bovey Model
	*/
	//@{		

	/**	Shift assignment processor implementing Johnson Bovey theory.
	*/
	class JohnsonBoveyShift
		:	public UnaryProcessor<Object>
	{
		public:

			/**@name	Constructors and Destructors
			*/
			//@{

			/**	Default constructor.
			*/
			JohnsonBoveyShift();

			/**	Destructor
			*/
			virtual ~JohnsonBoveyShift();

			//@}


			/**@name	Processor specific functions.
			*/
			//@{

			/**	Start method.
			*/
			virtual bool start();

			/**	Finish method.
			*/
			virtual bool finish();

			/**	Application method
			*/
			virtual Processor::Result operator()(Object&);
			//@}


		private:

			// private Variablen :

			double			n_;
			double			radius_;
			Vector3		mittelpunkt_;
			Vector3		normal_;

			list<PDBAtom*>	proton_list_;	
			list<Residue*> 	aromat_list_;
			String**				asrings_;
			Residue* 				residue_;
			PDBAtom* 				patom_;
			double 					shift_;


			// private Funktionen :

			double rf(double,double,double);	// Carlson elliptical Integral of 1st kind
			double rd(double,double,double);	// Carlson elliptical Integral of 2nd kind
			double ellf(double,double);	// Legendre elliptical Integral of 1st kind
			double elle(double,double);	// Legendre elliptical Integral of 2nd kind
	};

	//@}

} // namespace BALL 

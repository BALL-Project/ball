// $Id: geometricTransformations.h,v 1.2 1999/12/04 18:34:25 oliver Exp $

#ifndef BALL_STRUCTURE_GEOMETRICTRANSFORMATIONS_H
#define BALL_STRUCTURE_GEOMETRICTRANSFORMATIONS_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_MATRIX44_H
#	include <BALL/MATHS/matrix44.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

namespace BALL {

	/**	@name	Geometric transformation applicators
			@nonames
	*/
	//@{

	/**	Translation applicator.
			This applicator executes a translation on each atom of a given Composite object.
			The translation ma be given in the constructor, or by calling setTranslation.
			Applying the applicator to a given Composite object will add the given
			translation vector to the coordinates of each Atom object (and derived classes).\\

			{\bf Definition:} \URL{BALL/STRUCTURE/geometricTransformations.h}
			\\
	*/
	class TranslationProcessor
		:	public UnaryProcessor<Atom> 
	{
		public:

		/**	Default constructor.
				The translation applicator object is initialized and the transformation
				is set to (0, 0, 0).
		*/
		TranslationProcessor();
			
		
		/**	Constructor.
				Creates a new instance of a TranslationProcessor and sets
				the translation vector.
				@param	translation the new translation vector
		*/
		TranslationProcessor(const Vector3&	translation);


		/**	Sets a new translation
				@param	translation the new translation vector
		*/
		void setTranslation(const Vector3& translation);

		/**	Returns the translation vector
				@return	the current translation vector
		*/
		const Vector3& getTranslation() const;
		
		
		//----------------  applicator methods ----------------
		virtual Processor::Result operator()(Atom& atom);


		private:

		Vector3      translation_;
	};


	/**	Transformation applicator.
			This applicator executes a transformation on each atom of a given Composite object.
			The transformation may be given in the constructor, or by calling setTransformation.
			Applying the applicator to a given Composite object will multiply the coordinates
			of each Atom object (or objects derived from Atom) with the given transformation
			matrix.\\
			{\bf Definition:} \URL{BALL/STRUCTURE/geometricTransformations.h}
			\\
	*/
	class TransformationProcessor
		:public UnaryProcessor<Atom> 
	{
		public:

		/**	Default constructor.
				The transformation applicator is initialized and the transformation
				is set to identity:
				\[
					\left(	\begin{array}{cccc}
										1 & 0 & 0 & 0 \\
										0 & 1 & 0 & 0 \\
										0 & 0 & 1 & 0 \\
										0 & 0 & 0 & 1 \\
									\end{array}
					\right)
				\]
		*/
		TransformationProcessor();
			
		
		/**	Constructor.
				Creates a new instance of a TransformationProcessor and sets
				the transformation matrix.
				@param	transformation the new transformation matrix
		*/
		TransformationProcessor(const Matrix4x4&	transformation);


		/**	Sets a new transformation
				@param	transformation the new transformation matrix
		*/
		void setTransformation(const Matrix4x4& transformation);

		/**	Returns the transformation vector
				@return	the current transformation matrix
		*/
		const Matrix4x4& getTransformation() const;
		
		
		//----------------  applicator methods ----------------
		virtual Processor::Result operator()(Atom& atom);


		protected:

		Matrix4x4      transformation_;
	};


	//@}

} // namespace BALL

#endif // BALL_STRUCTURE_GEOMETRICTRANSFORMATIONS_H

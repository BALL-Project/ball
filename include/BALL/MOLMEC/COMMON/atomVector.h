// $Id: atomVector.h,v 1.8 2001/06/05 15:46:30 anker Exp $

#ifndef BALL_MOLMEC_COMMON_ATOMVECTOR_H 
#define BALL_MOLMEC_COMMON_ATOMVECTOR_H 

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

namespace BALL 
{ 
	
	class Gradient;
	class Atom;
	class Composite;
	
	/**	Atom vector class.
			This class is used to store atom pointers. \\
			{\bf Definition:}\URL{BALL/MOLMEC/COMMON/atomVector.h}
	*/
	class AtomVector
		:	private std::vector<Atom*>
	{
    public:

    BALL_CREATE_DEEP(AtomVector)

		/**	@name Type definitions
		*/
		//@{
		
		/**	Iterator type
		*/
		typedef std::vector<Atom*>::iterator Iterator;

		/**	Const iterator type.
		*/
		typedef std::vector<Atom*>::const_iterator ConstIterator;

		//@}
    /**	@name	Constructors and Destructors	
    */
    //@{

    /**	Default constructor.
    */
    AtomVector();

    /**	Construct from a Composite.
				This method constructs an AtomVector from a given composite
				using selection or not.
				@param	composite the composite containing the atoms
				@param	selected_only store the selected atoms only (if {\bf true})
    */
    AtomVector(const Composite& composite, bool selected_only = false);

    /**	Copy constructor
    */
    AtomVector(const AtomVector& atoms, bool deep = true);

    /**	Destructor.
    */
    virtual ~AtomVector();

		/**	Clear the vector.
				Removes all atoms from the vector
		*/
		using std::vector<Atom*>::clear;

    //@}
    /**	@name	Assignments 
    */
    //@{

    /**	Assignment operator
    */
    AtomVector& operator = (const AtomVector& rhs);

		/**	Assign from another atom vector
		*/
		void set(const AtomVector& atoms);

    /**	Assignment operator for Composites.
				Calls \Ref{set} and extracts all atoms, if 
				none of the atoms in {\tt composite} are selected or the selected atoms
				only (if any atom is selected in {\tt composite}.
				The implementation calls {\tt \Ref{set}({\tt rhs}, {\tt rhs}.\Ref{containsSelection}())}.
				@see	Composite::containsSelection
				@see	Selectable
    */
    AtomVector& operator = (const Composite& rhs);

		/**	Assign from a composite.
				This method iterates over the composite tree and
				extracts all atoms. 
				@param	selected_only extract only selected atoms if set to {\bf true}
		*/
		void set(const Composite& composite, bool selected_only = false);

    //@}
    /**	@name	Accessors 
    */
    //@{

		/**	Return the vector size;
		*/
		using std::vector<Atom*>::size;

		// BAUSTELLE: PROBLEM IN DOCU: using
    /* Random access operator
    */
    using std::vector<Atom*>::operator [];

		/**	Store the current atom positions.
				AtomVector also contains an array with positions	
				for each atom.
				\Ref{moveTo} considers these coordinates as start coordinates.
		*/
		void savePositions();

		/**	Resets the atom positions to the saved positions.
				If coordinates weres stored using \Ref{savePositions}, the atoms
				coordinates are reset to the saved positions.
				If no savedPositions exist the coordinates remain unchanged.
		*/
		void resetPositions();

		/**	Move all atoms along a direction vector.
				The method translates all atoms a long a given direction.
				The direction vector is multiplied with a step length {\tt step}.
				If a saved position exists (\Ref{savePositions}), it is used as a start
				position (i.e. the final positions are $\vec{\mathrm{start}} + \mathrm{step} \vec{\mathrm{direction}}$).
				Otherwise, the current atom positions are used.
				If the gradient's size differs from the number of atoms, nothing is done.
		*/
		void moveTo(const Gradient& direction, float step = 1.0);

		/**	Insert an atom pointer.
		*/
		using std::vector<Atom*>::push_back;

		/**	Resize the vector.
		*/
		using std::vector<Atom*>::resize;

    //@}
		/**	@name	Iteration
		*/
		//@{

		/**	begin method
		*/
		using std::vector<Atom*>::begin;

		/**	end method
		*/
		using std::vector<Atom*>::end;

		//@}
		
		protected:

		/**	The saved positions.
		*/
		std::vector<Vector3>	saved_position_;
  };


	class AtomConstVector
		:	private std::vector<const Atom*>
	{
    public:

    BALL_CREATE_DEEP(AtomConstVector)

		/**	@name Type definitions
		*/
		//@{
		
		/**	Iterator type
		*/
		typedef std::vector<const Atom*>::iterator Iterator;

		/**	Const iterator type.
		*/
		typedef std::vector<const Atom*>::const_iterator ConstIterator;

		//@}
    /**	@name	Constructors and Destructors	
    */
    //@{

    /**	Default constructor.
    */
    AtomConstVector();

    /**	Construct from a Composite.
				This method constructs an AtomConstVector from a given composite
				using selection or not.
				@param	composite the composite containing the atoms
				@param	selected_only store the selected atoms only (if {\bf true})
    */
    AtomConstVector(const Composite& composite, bool selected_only = false);

    /**	Copy constructor
    */
    AtomConstVector(const AtomConstVector& atoms, bool deep = true);

    /**	Destructor.
    */
    virtual ~AtomConstVector();

		/**	Clear the vector.
				Removes all atoms from the vector
		*/
		using std::vector<const Atom*>::clear;

    //@}
    /**	@name	Assignments 
    */
    //@{

    /**	Assignment operator
    */
    AtomConstVector& operator = (const AtomConstVector& rhs);

		/**	Assign from another atom vector
		*/
		void set(const AtomConstVector& atoms);

    /**	Assignment operator for Composites.
				Calls \Ref{set} and extracts all atoms, if 
				none of the atoms in {\tt composite} are selected or the selected atoms
				only (if any atom is selected in {\tt composite}.
				The implementation calls {\tt \Ref{set}({\tt rhs}, {\tt rhs}.\Ref{containsSelection}())}.
				@see	Composite::containsSelection
				@see	Selectable
    */
    AtomConstVector& operator = (const Composite& rhs);

		/**	Assign from a composite.
				This method iterates over the composite tree and
				extracts all atoms. 
				@param	selected_only extract only selected atoms if set to {\bf true}
		*/
		void set(const Composite& composite, bool selected_only = false);

    //@}
    /**	@name	Accessors 
    */
    //@{

		/**	Return the vector size;
		*/
		using std::vector<const Atom*>::size;

		// BAUSTELLE: PROBLEM IN DOCU: using
    /* Random access operator
    */
    using std::vector<const Atom*>::operator [];

		/**	Store the current atom positions.
				AtomConstVector also contains an array with positions	
				for each atom.
				\Ref{moveTo} considers these coordinates as start coordinates.
		*/
		void savePositions();

		/**	Resets the atom positions to the saved positions.
				If coordinates weres stored using \Ref{savePositions}, the atoms
				coordinates are reset to the saved positions.
				If no savedPositions exist the coordinates remain unchanged.
		*/
		void resetPositions();

		/**	Move all atoms along a direction vector.
				The method translates all atoms a long a given direction.
				The direction vector is multiplied with a step length {\tt step}.
				If a saved position exists (\Ref{savePositions}), it is used as a start
				position (i.e. the final positions are $\vec{\mathrm{start}} + \mathrm{step} \vec{\mathrm{direction}}$).
				Otherwise, the current atom positions are used.
				If the gradient's size differs from the number of atoms, nothing is done.
		*/
		void moveTo(const Gradient& direction, float step = 1.0);

		/**	Insert an atom pointer.
		*/
		using std::vector<const Atom*>::push_back;

		/**	Resize the vector.
		*/
		using std::vector<const Atom*>::resize;

    //@}
		/**	@name	Iteration
		*/
		//@{

		/**	begin method
		*/
		using std::vector<const Atom*>::begin;

		/**	end method
		*/
		using std::vector<const Atom*>::end;

		//@}
		
		protected:

		/**	The saved positions.
		*/
		std::vector<Vector3>	saved_position_;
  };

} // end of namespace BALL



#endif // BALL_MOLMEC_MINIMIZATION_GRADIENT_H 

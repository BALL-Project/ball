// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorProcessor.h,v 1.32.18.1 2007/03/25 21:26:06 oliver Exp $
//

#ifndef BALL_VIEW_MODELS_COLORPROCESSOR_H
#define BALL_VIEW_MODELS_COLORPROCESSOR_H

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_DATATYPE_HASHGRID_H
# include <BALL/DATATYPE/hashGrid.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif

namespace BALL
{
	class Atom;

	namespace VIEW
	{
		class Mesh;
				
/** Calculating colors for given GeometricObject instances. 
		This class defines the interface and basic algorithms to colorize geometric objects, e.g. 
		Sphere, Tube, TwoColoredTube, etc.
		Derived classes implement special ways to colorize, e.g. by charge or element.
		They have to overload the method getColor(Composite, ColorRGBA). 
		<br>
		If no color can be calculated for a GeometricObject, e.g. if it has no Composite, the default
		color is set.
		<br>
		A bit tricky is the coloring of meshes. The goal was to give every triangle of a mesh a color
		according to its nearest Atom. To do so, a threedimensional HashGrid3 is used. It is
		computed, when the ColorProcessor first encounters a Mesh in the operator ().
		For this a pointer to the HashSet with the used Composite instances exists, which is set by the 
		Representation.
		\ingroup  ViewModels
*/
class BALL_VIEW_EXPORT ColorProcessor
	: public UnaryProcessor<GeometricObject*>
{
	public:

	BALL_CREATE(ColorProcessor)
	
	/// A HashSet with the used Composites, see also Representation
	typedef HashSet<const Composite*>  CompositeSet;

	/// a threedimensioal grid with the pointers to the atoms
	typedef HashGrid3<const Atom*>  AtomGrid;

	/// a single box in the threedimensional grid
	typedef HashGridBox3<const Atom*> AtomBox;

	/// Default Constructor.
	ColorProcessor();

	/// Copy constructor.
	ColorProcessor(const ColorProcessor& color_calculator);

	/// Destructor.
	virtual ~ColorProcessor();

	/** Explicit default initialization.
			Reset the state of the
			 <b> default_color_</b> to red (<tt> "FF0000FF"</tt>).
			The grid is cleared and the pointer to the CompositeSet is set to NULL.
	*/
	virtual void clear();

	/// Assignment
	void set(const ColorProcessor& color_calculator);

	///
	virtual bool start();

	/** Assignment operator.
			Calls set.
	*/
	const ColorProcessor&  operator = (const ColorProcessor& color_calculator);

	/** Some coloring processors need only to be applied to a Representation if the hierarchy of
			the Representations Composite is changed, or the Composites Names or Type (like SecondaryStructure::Type)
			is changed. As this is seldom the case, we can speedup the call to Representation::update() in most cases.
			This method defines if a ColoringMethod needs to be applied in all cases.
			The default value is false. 
			Initialise the member update_always_needed_ to true in derived classes, if the derived ColorProcessor shall
			always be applied.
	*/
	bool updateAlwaysNeeded() { return update_always_needed_;};

	/** Change the default color.
	*/
	void setDefaultColor(const ColorRGBA& color);

	/** Non-mutable inspection of the default color.
	*/
	const ColorRGBA& getDefaultColor() const
		{return default_color_;}

	/** Calculate a color for a GeometricObject.
	*/
	virtual Processor::Result operator() (GeometricObject*& object);

	/** Calculate a color for a Composite.
			The given ColorRGBA instance is set to the calculated color.
			This method is called by the operator() method.
			Here it just sets the default color.
			You have to overload this operator in derived classes.
	*/
	virtual void getColor(const Composite& composite, ColorRGBA& color_to_be_set);

	///
	Size getTransparency() const
		{ return transparency_;}

	/** Set the transparency.
			To be overloaded in derived classes
	*/
	virtual void setTransparency(Size value);

	/** Set the pointer to the CompositeSet.
			This method is called by Representation::setColorProcessor and Representation::update.
	*/
	void setComposites(const std::list<const Composite*>* composites);

	/// Return a pointer to the Composites.
	const std::list<const Composite*>* getComposites()
		{ return composites_;}

	///
	void clearAtomGrid();

	///
	void setAdditionalGridDistance(float distance)
		{ additional_grid_distance_ = distance;}

	///
	float getAdditionalGridDistance() const
		{ return additional_grid_distance_;}

	///
	void setMinGridSpacing(float spacing) {min_spacing_ = spacing;}

	///
	AtomGrid& getAtomGrid() 
		{ return atom_grid_;}

	/** Internal value dump.
			Dump the current state of this ColorProcessor to 
			the output ostream <b> s</b> with dumping depth <b> depth</b>.
			\param   s output stream where to output the state of this ColorProcessor
			\param   depth the dumping depth
	*/
	virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

	///
	void setModelType(ModelType type) {model_type_ = type;}

	//_ Create the threedimensional grid from the CompositeSet, or a given Composite 
	virtual void createAtomGrid(const Composite* from_mesh = 0);

	///
	const Atom* getClosestItem(const Vector3& v) const;

	protected:

	//_ Colorize the mesh with the computed grid.
	virtual void colorMeshFromGrid_(Mesh& mesh);

	//_
	virtual bool canUseMeshShortcut_(const Composite&) { return false;}

	bool  			update_always_needed_;
	//_ a color that will be used if no other color can be calculated.
	ColorRGBA		default_color_;
	//_ for speedup, we dont have to set transparency each time we color a geometric object
	ColorRGBA		selection_color_;
	Size 				transparency_;

	const       std::list<const Composite*>* composites_;

	AtomGrid 		atom_grid_;
	ModelType   model_type_;
	const Composite*  last_composite_of_grid_;
	float 			additional_grid_distance_;
	float 			min_spacing_;
	//@}
};


/** Base class for ColorProcessors, that interpolate between two values
		\ingroup  ViewModels
*/
class BALL_VIEW_EXPORT InterpolateColorProcessor
	: public ColorProcessor
{
	public: 

	///
	enum Mode
	{
		///
		USE_OUTSIDE_COLOR = 0,

		///
		DEFAULT_COLOR_FOR_OUTSIDE_COLORS,

		///
		NO_OUTSIDE_COLORS
	};
		

	BALL_CREATE(InterpolateColorProcessor)

	///
	InterpolateColorProcessor();
	
	///
	InterpolateColorProcessor(const InterpolateColorProcessor& pro);

	///
	virtual bool start();

	///
	void setMode(Mode mode) { mode_ = mode;}

	///
	Mode getMode() const { return mode_;}

	///
	vector<ColorRGBA>& getColors() { return colors_;}
	
	///
	const vector<ColorRGBA>& getColors() const { return colors_;}

	///
	void setColors(const vector<ColorRGBA>& colors) { colors_ = colors;}

	///
	void setMinColor(const ColorRGBA& color);

	///
	void setMaxColor(const ColorRGBA& color);

	///
	const ColorRGBA& getMinColor() const;
	
	///
	const ColorRGBA& getMaxColor() const;

	///
	void setMaxValue(float value) {max_value_ = value;}

	///
	float getMaxValue() const { return max_value_;}

	///
	void setMinValue(float value) { min_value_ = value;}

	///
	float getMinValue() const { return min_value_;}

	/** Interpolate a color between the given colors.
			To be overloaded in derived classes.
	*/
	virtual void interpolateColor(float value, ColorRGBA& color_to_be_set);

	protected:

	// out of range colors
	ColorRGBA min_color_,
						max_color_;

	// standard colors
	vector<ColorRGBA> colors_;

	Mode 			mode_;

	float 		max_value_;
	float 		min_value_;
	
	// value distance between two colors
	float 		x_;
};

} } // namespaces

#endif // BALL_VIEW_MODELS_COLORPROCESSOR_H

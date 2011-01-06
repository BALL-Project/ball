// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_RENDERING_RENDERERS_VRMLRENDERER_H
#define BALL_VIEW_RENDERING_RENDERERS_VRMLRENDERER_H

#ifndef BALL_VIEW_RENDERING_RENDERERS_RENDERER_H
# include <BALL/VIEW/RENDERING/RENDERERS/renderer.h>
#endif

#ifndef BALL_SYSTEM_FILE_H
# include <BALL/SYSTEM/file.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
# include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_MATRIX44_H
# include <BALL/MATHS/matrix44.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		class ColorRGBA;

/** VRMLRenderer class.
		This class walks over all the geometric primitives in a Scene
		and exports them into a data file in the VRML format, which can
		be used to render the same scene externally.
		<br>
		<br>
		This is unmaintained code!
		\ingroup ViewRendering
*/
class BALL_VIEW_EXPORT VRMLRenderer : public Renderer
{
	public:

	/** @name Constructors and Destructors.
	 */
	//@{

	/// Default constructor.
	VRMLRenderer();

	/** Detailed constructor.
			\param name The name of the file we will create
	 */
	VRMLRenderer(const String& name)
		throw(Exception::FileNotFound);
	
	/// Destructor.
	virtual ~VRMLRenderer();

	/// Clear method.
	virtual void clear();

	//@}
	/** @name Accessors
	 */
	//@{

	/** Sets the name of the file we will create.
			\param name The file name
	 */
	void setFileName(const String& name)
		throw(Exception::FileNotFound);

	/** Converts a ColorRGBA into a String in VRMLRay format.
	 */
	String VRMLColorRGBA(const ColorRGBA& input);

	/** Converts a Vector3 into a String in VRMLRay format.
	 */
	String VRMLVector3(Vector3 input);

	///
	void VRMLColor(const ColorRGBA& color);

	//@}
	/** @name Processor specific methods
	 */
	//@{
	
	/** Start method. 
			This method creates the file and writes the header.
	 */
	virtual bool init(const Stage& stage);

	/** Finish method.
			This method writes the ending of the file and closes it.
	 */
	virtual bool finish();

	//@}

	void renderSphere_(const Sphere& sphere);
	
	void renderMesh_(const Mesh& mesh);

	void renderLine_(const Line& miniTube);

	void renderTwoColoredLine_(const TwoColoredLine& miniTube);

	void renderTube_(const Tube& tube);

	void renderTwoColoredTube_(const TwoColoredTube& tube);

	void out_(const String& data);

	void outheader_(const String& data)
		{out_(data); current_indent_ += 1;}

	void outfinish_(const String& data)
		{out_(data); current_indent_ -= 1;}

	Size width, height;

	protected:

	void header_(const Vector3& translation, const ColorRGBA& color, 
							 const String& rotation = "");
		
	void footer_();

	File outfile_;

	Vector3   origin_;
	Matrix4x4 rotation_;
	Index current_indent_;

	//size estimate for afterwards scaling for printing
	float smallX, smallY, smallZ, bigX, bigY, bigZ; 

	//boolean showing if scaling is relevant afterall
	bool scalingUsed;
};
  
} } // namespaces

#endif // BALL_VIEW_RENDERING_VRMLRENDERER_H

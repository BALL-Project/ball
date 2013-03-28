//STLRenderer: Version 1.0 (2008.09.12) Annette Treichel

#ifndef BALL_VIEW_RENDERING_RENDERERS_STLRENDERER_H
#define BALL_VIEW_RENDERING_RENDERERS_STLRENDERER_H

#ifndef BALL_VIEW_RENDERING_SCENEEXPORTER_H
# include <BALL/VIEW/RENDERING/sceneExporter.h>
#endif

#ifndef BALL_VIEW_RENDERING_GEOMETRICOBJECTDISPATCHER_H
# include <BALL/VIEW/RENDERING/geometricObjectDispatcher.h>
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

/** STLRenderer class.
		Representation of all primitives as Meshes in stl 
		==> extrernal rendering of the same scene
		<br>
		<br>
		\ingroup ViewRendering
*/

class BALL_VIEW_EXPORT STLRenderer 
	: public SceneExporter, public GeometricObjectDispatcher
{
	public:

	/** @name Constructors and Destructors.
	 */
	//@{

	/** Detailed constructor.
			\param name The name of the file we will create
	 */
	STLRenderer(const String& name);
	
	/// Destructor.
	virtual ~STLRenderer();

	/// Clear method.
	virtual void clear();

	//@}
	/** @name Accessors
	 */
	//@{

	/** Converts a Vector3 into a String in VRML format as stl works in the same perspektive.
	 */
	String VRMLVector3(Vector3 input);

	//@}
	/** @name Processor specific methods
	 */
	//@{
	
	/** Start method. 
			This method creates the file and writes the header.
	 */
	virtual bool init(const Stage* stage, float width, float height);

	bool exportOneRepresentation(const Representation* representation);

	//@}

	protected:
	/**
	 * Finish method.
	 * This method writes the ending of the file and closes it.
	 */
	bool finishImpl_();

	void renderSphere_(const Sphere& sphere);

	void renderMesh_(const Mesh& mesh);

	void renderTube_(const Tube& tube);

	void renderTwoColoredTube_(const TwoColoredTube& tube);

	void renderLine_(const Line& tube);

	void renderTwoColoredLine_(const TwoColoredLine& tube);

	void out_(const String& data);

	void outheader_(const String& data)
		{out_(data); current_indent_ += 1;}

	void outfinish_(const String& data)
		{out_(data); current_indent_ -= 1;}

	void header_(const Vector3& translation, const ColorRGBA& color, 
							 const String& rotation = "");

	void footer_();

	Vector3   origin_;
	Matrix4x4 rotation_;
	Index current_indent_;
	String endingName_;
};
  
} } // namespaces

#endif // BALL_VIEW_RENDERING_STLRENDERER_H


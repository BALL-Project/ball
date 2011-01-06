//STLRenderer: Version 1.0 (2008.09.12) Annette Treichel

#ifndef BALL_VIEW_RENDERING_RENDERERS_STLRENDERER_H
#define BALL_VIEW_RENDERING_RENDERERS_STLRENDERER_H

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

/** STLRenderer class.
		Representation of all primitives as Meshes in stl 
		==> extrernal rendering of the same scene
		<br>
		<br>
		\ingroup ViewRendering
*/

class BALL_VIEW_EXPORT STLRenderer 
	: public Renderer
{
	public:

	/** @name Constructors and Destructors.
	 */
	//@{

	/// Default constructor.
	STLRenderer();

	/** Detailed constructor.
			\param name The name of the file we will create
	 */
	STLRenderer(const String& name)
		throw(Exception::FileNotFound);
	
	/// Destructor.
	virtual ~STLRenderer();

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
	virtual bool init(const Stage& stage);

	/** Finish method.
			This method writes the ending of the file and closes it.
	 */
	virtual bool finish();

	//@}

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

	Size width, height;

	protected:

	void header_(const Vector3& translation, const ColorRGBA& color, 
							 const String& rotation = "");
		
	void footer_();

	File outfile_;

	Vector3   origin_;
	Matrix4x4 rotation_;
	Index current_indent_;
	String endingName_;
};
  
} } // namespaces

#endif // BALL_VIEW_RENDERING_STLRENDERER_H


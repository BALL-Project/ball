//STLRenderer: Version 1.0 (2008.09.12) Annette Treichel

#ifndef BALL_VIEW_RENDERING_STLRENDERER_H
#define BALL_VIEW_RENDERING_STLRENDERER_H

#ifndef BALL_VIEW_RENDERING_RENDERER_H
# include <BALL/VIEW/RENDERING/renderer.h>
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

class BALL_VIEW_EXPORT STLRenderer : public Renderer
{
	public:

	BALL_CREATE(STLRenderer)

	/** @name Constructors and Destructors.
	 */
	//@{

	/// Default constructor.
	STLRenderer()
		throw();

	/** Detailed constructor.
			\param name The name of the file we will create
	 */
	STLRenderer(const String& name)
		throw(Exception::FileNotFound);
	
	/// Destructor.
	virtual ~STLRenderer()
		throw();

	/// Clear method.
	virtual void clear()
		throw();

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
	String VRMLVector3(Vector3 input)
		throw();

	//@}
	/** @name Processor specific methods
	 */
	//@{
	
	/** Start method. 
			This method creates the file and writes the header.
	 */
	virtual bool init(const Stage& stage)
		throw();

	/** Finish method.
			This method writes the ending of the file and closes it.
	 */
	virtual bool finish()
		throw();

	//@}

	void renderSphere_(const Sphere& sphere)
		throw();
	
	void renderMesh_(const Mesh& mesh)
		throw();

	void renderTube_(const Tube& tube)
		throw();

	void renderTwoColoredTube_(const TwoColoredTube& tube)
		throw();

	void renderLine_(const Line& tube)
		throw();

	void renderTwoColoredLine_(const TwoColoredLine& tube)
		throw();

	void out_(const String& data)
		throw();

	void outheader_(const String& data)
		throw() {out_(data); current_indent_ += 1;}

	void outfinish_(const String& data)
		throw() {out_(data); current_indent_ -= 1;}

	Size width, height;

	protected:

	void header_(const Vector3& translation, const ColorRGBA& color, 
							 const String& rotation = "")
		throw();
		
	void footer_()
		throw();

	File outfile_;

	Vector3   origin_;
	Matrix4x4 rotation_;
	Index current_indent_;
	String endingName_;
};
  
} } // namespaces

#endif // BALL_VIEW_RENDERING_STLRENDERER_H


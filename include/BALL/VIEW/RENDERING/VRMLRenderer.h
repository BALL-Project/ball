// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: VRMLRenderer.h,v 1.5 2004/02/26 08:41:45 anhi Exp $
//

#ifndef BALL_VIEW_RENDERING_VRMLRENDERER_H
#define BALL_VIEW_RENDERING_VRMLRENDERER_H

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
		class ColorRGBA;

/** VRMLRenderer class.
		This class walks over all the geometric primitives in a Scene
		and exports them into a data file in the VRML format, which can
		be used to render the same scene externally.
		\ingroup ViewRendering
*/
class BALL_EXPORT VRMLRenderer : public Renderer
{
	public:

	/** @name Constructors and Destructors.
	 */
	//@{

	/// Default constructor.
	VRMLRenderer()
		throw();

	/** Detailed constructor.
			\param name The name of the file we will create
	 */
	VRMLRenderer(const String& name)
		throw(Exception::FileNotFound);
	
	/// Destructor.
	virtual ~VRMLRenderer()
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

	/** Converts a ColorRGBA into a String in VRMLRay format.
	 */
	String VRMLColorRGBA(const ColorRGBA& input)
		throw();

	/** Converts a Vector3 into a String in VRMLRay format.
	 */
	String VRMLVector3(Vector3 input)
		throw();

	///
	void VRMLobjectColor(const GeometricObject& object)
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

	void renderSphere_(const Sphere& sphere)
		throw();
	
	void renderMesh_(const Mesh& mesh)
		throw();

	void out_(const String& data)
		throw();

	void outheader_(const String& data)
		throw() {out_(data); current_intend_ += 1;}

	void outfinish_(const String& data)
		throw() {out_(data); current_intend_ -= 1;}
	//@}

		Size width, height;
	protected:
		
		File outfile_;

		Vector3   origin_;
		Matrix4x4 rotation_;
		Index current_intend_;

};
  
} } // namespaces

#endif // BALL_VIEW_RENDERING_VRMLRENDERER_H

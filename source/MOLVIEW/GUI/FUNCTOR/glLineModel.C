// $Id: glLineModel.C,v 1.4 2001/05/13 15:02:41 hekl Exp $

#include <BALL/MOLVIEW/GUI/FUNCTOR/glLineModel.h>


namespace BALL
{
	namespace MOLVIEW
	{

		using VIEW::GLPoint;
		using VIEW::GLLine;

		AddGLLineModel::AddGLLineModel()
			throw()
			: AddLineModel()
		{
		}

		AddGLLineModel::AddGLLineModel(const AddGLLineModel& rAddGLLineModel, bool deep)
			throw()
			: AddLineModel(rAddGLLineModel, deep)
		{
		}

		AddGLLineModel::~AddGLLineModel()
      throw()
 		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<AddGLLineModel>() << endl;
			#endif 

			AddLineModel::destroy();
		}

		Point* AddGLLineModel::createPoint_()
		{
			return (Point *)(new GLPoint());
		}

		Line* AddGLLineModel::createLine_()
		{
			return (Line *)(new GLLine());
		}

		TwoColoredLine* AddGLLineModel::createTwoColoredLine_()
		{
			return (TwoColoredLine *)(new GLTwoColoredLine());
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/FUNCTOR/glLineModel.iC>
#		endif
	
	} // namespace MOLVIEW

} // namespace BALL

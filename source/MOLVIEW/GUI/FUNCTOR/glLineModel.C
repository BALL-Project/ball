// $Id: glLineModel.C,v 1.2 2000/12/12 16:19:34 oliver Exp $

#include <BALL/MOLVIEW/GUI/FUNCTOR/glLineModel.h>


namespace BALL
{

	namespace MOLVIEW
	{

		using VIEW::GLPoint;
		using VIEW::GLLine;

		AddGLLineModel::AddGLLineModel
			()
				:
				AddLineModel()
		{
		}

		AddGLLineModel::AddGLLineModel
			(const AddGLLineModel &__rAddGLLineModel,
			 bool deep)
				:
				AddLineModel(__rAddGLLineModel, deep)
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

		Point *
		AddGLLineModel::createPoint_
			()
		{
			return (Point *)(new GLPoint());
		}

		Line *
		AddGLLineModel::createLine_
			()
		{
			return (Line *)(new GLLine());
		}

		TwoColoredLine *
		AddGLLineModel::createTwoColoredLine_
			()
		{
			return (TwoColoredLine *)(new GLTwoColoredLine());
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/FUNCTOR/glLineModel.iC>
#		endif
	
	} // namespace MOLVIEW

} // namespace BALL

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glLineModel.C,v 1.6 2002/12/12 10:57:46 oliver Exp $

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
				cout << "Destructing object " << (void*)this << " of class " << RTTI::getName<AddGLLineModel>() << endl;
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

	} // namespace MOLVIEW

} // namespace BALL

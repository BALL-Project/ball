// $Id: events.C,v 1.1 2000/09/23 13:28:38 hekl Exp $

#include <BALL/VIEW/GUI/WIDGETS/events.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		BALL_VIEW_IMPLEMENT_NOTIFY_FUNCTION_FOR_EVENT_CLASS(RotateSystem)
		BALL_VIEW_IMPLEMENT_NOTIFY_FUNCTION_FOR_EVENT_CLASS(ZoomSystem)
		BALL_VIEW_IMPLEMENT_NOTIFY_FUNCTION_FOR_EVENT_CLASS(TranslateSystem)
		BALL_VIEW_IMPLEMENT_NOTIFY_FUNCTION_FOR_EVENT_CLASS(SelectionPressedMoved)
		BALL_VIEW_IMPLEMENT_NOTIFY_FUNCTION_FOR_EVENT_CLASS(SelectionPressed)
		BALL_VIEW_IMPLEMENT_NOTIFY_FUNCTION_FOR_EVENT_CLASS(SelectionReleased)
		BALL_VIEW_IMPLEMENT_NOTIFY_FUNCTION_FOR_EVENT_CLASS(DeselectionPressedMoved)
		BALL_VIEW_IMPLEMENT_NOTIFY_FUNCTION_FOR_EVENT_CLASS(DeselectionPressed)
		BALL_VIEW_IMPLEMENT_NOTIFY_FUNCTION_FOR_EVENT_CLASS(DeselectionReleased)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(ShiftKeyPressed, ControlKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(ShiftKeyPressed, MouseMoved)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(ControlKeyPressed, MouseMoved)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(ShiftKeyPressed, ControlKeyPressedMouseMoved)

		BALL_VIEW_IMPLEMENT_OPERATOR_WITH_RETURN_TYPE
			(ControlKeyPressed,
			 ShiftKeyPressedMouseMoved,
			 ShiftKeyPressedControlKeyPressedMouseMoved)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(ShiftKeyPressedControlKeyPressed,
			 MouseMoved)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseLeftButtonPressed,
			 ShiftKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseLeftButtonPressed,
			 ControlKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseLeftButtonPressed,
			 ShiftKeyPressedControlKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseLeftButtonPressedShiftKeyPressed,
			 ControlKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR_WITH_RETURN_TYPE
			(MouseLeftButtonPressedControlKeyPressed,
			 ShiftKeyPressed,
			 MouseLeftButtonPressedShiftKeyPressedControlKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseLeftButtonPressed,
			 MouseMoved)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseLeftButtonPressedShiftKeyPressed,
			 MouseMoved)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseLeftButtonPressedControlKeyPressed,
			 MouseMoved)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseLeftButtonPressedShiftKeyPressed,
			 ControlKeyPressedMouseMoved)

		BALL_VIEW_IMPLEMENT_OPERATOR_WITH_RETURN_TYPE
			(MouseLeftButtonPressedControlKeyPressed,
			 ShiftKeyPressedMouseMoved,
			 MouseLeftButtonPressedShiftKeyPressedControlKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseLeftButtonPressedShiftKeyPressedControlKeyPressed,
			 MouseMoved)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseLeftButtonReleased,
			 ShiftKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseLeftButtonReleased,
			 ControlKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseLeftButtonReleasedShiftKeyPressed,
			 ControlKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR_WITH_RETURN_TYPE
			(MouseLeftButtonReleasedControlKeyPressed,
			 ShiftKeyPressed,
			 MouseLeftButtonReleasedShiftKeyPressedControlKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseMiddleButtonPressed,
			 ShiftKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseMiddleButtonPressed,
			 ControlKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseMiddleButtonPressed,
			 ShiftKeyPressedControlKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseMiddleButtonPressedShiftKeyPressed,
			 ControlKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR_WITH_RETURN_TYPE
			(MouseMiddleButtonPressedControlKeyPressed,
			 ShiftKeyPressed,
			 MouseMiddleButtonPressedShiftKeyPressedControlKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseMiddleButtonPressed,
			 MouseMoved)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseMiddleButtonPressedShiftKeyPressed,
			 MouseMoved)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseMiddleButtonPressedControlKeyPressed,
			 MouseMoved)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseMiddleButtonPressedShiftKeyPressed,
			 ControlKeyPressedMouseMoved)

		BALL_VIEW_IMPLEMENT_OPERATOR_WITH_RETURN_TYPE
			(MouseMiddleButtonPressedControlKeyPressed,
			 ShiftKeyPressedMouseMoved,
			 MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedMouseMoved)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseMiddleButtonPressedShiftKeyPressedControlKeyPressed,
			 MouseMoved)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseMiddleButtonReleased,
			 ShiftKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseMiddleButtonReleased,
			 ControlKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseMiddleButtonReleasedShiftKeyPressed,
			 ControlKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR_WITH_RETURN_TYPE
			(MouseMiddleButtonReleasedControlKeyPressed,
			 ShiftKeyPressed,
			 MouseMiddleButtonReleasedShiftKeyPressedControlKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseRightButtonPressed,
			 ShiftKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseRightButtonPressed,
			 ControlKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseRightButtonPressed,
			 ShiftKeyPressedControlKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseRightButtonPressedShiftKeyPressed,
			 ControlKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR_WITH_RETURN_TYPE
			(MouseRightButtonPressedControlKeyPressed,
			 ShiftKeyPressed,
			 MouseRightButtonPressedShiftKeyPressedControlKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseRightButtonPressed,
			 MouseMoved)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseRightButtonPressedShiftKeyPressed,
			 MouseMoved)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseRightButtonPressedControlKeyPressed,
			 MouseMoved)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseRightButtonPressedShiftKeyPressed,
			 ControlKeyPressedMouseMoved)

		BALL_VIEW_IMPLEMENT_OPERATOR_WITH_RETURN_TYPE
			(MouseRightButtonPressedControlKeyPressed,
			 ShiftKeyPressedMouseMoved,
			 MouseRightButtonPressedShiftKeyPressedControlKeyPressedMouseMoved)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseRightButtonPressedShiftKeyPressedControlKeyPressed,
			 MouseMoved)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseRightButtonReleased,
			 ShiftKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseRightButtonReleased,
			 ControlKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR
			(MouseRightButtonReleasedShiftKeyPressed,
			 ControlKeyPressed)

		BALL_VIEW_IMPLEMENT_OPERATOR_WITH_RETURN_TYPE
			(MouseRightButtonReleasedControlKeyPressed,
			 ShiftKeyPressed,
			 MouseRightButtonReleasedShiftKeyPressedControlKeyPressed)


		Events::Events()
			:	ShiftKeyPressed(this),
				ShiftKeyPressedMouseMoved(this),
				ControlKeyPressed(this),
				ControlKeyPressedMouseMoved(this),
				ShiftKeyPressedControlKeyPressed(this),
				ShiftKeyPressedControlKeyPressedMouseMoved(this),
				MouseLeftButtonPressed(this),
				MouseLeftButtonPressedShiftKeyPressed(this),
				MouseLeftButtonPressedControlKeyPressed(this),
				MouseLeftButtonPressedShiftKeyPressedControlKeyPressed(this),
				MouseLeftButtonPressedMouseMoved(this),
				MouseLeftButtonPressedShiftKeyPressedMouseMoved(this),
				MouseLeftButtonPressedControlKeyPressedMouseMoved(this),
				MouseLeftButtonPressedShiftKeyPressedControlKeyPressedMouseMoved(this),
				MouseLeftButtonReleased(this),
				MouseLeftButtonReleasedShiftKeyPressed(this),
				MouseLeftButtonReleasedControlKeyPressed(this),
				MouseLeftButtonReleasedShiftKeyPressedControlKeyPressed(this),
				MouseMiddleButtonPressed(this),
				MouseMiddleButtonPressedShiftKeyPressed(this),
				MouseMiddleButtonPressedControlKeyPressed(this),
				MouseMiddleButtonPressedShiftKeyPressedControlKeyPressed(this),
				MouseMiddleButtonPressedMouseMoved(this),
				MouseMiddleButtonPressedShiftKeyPressedMouseMoved(this),
				MouseMiddleButtonPressedControlKeyPressedMouseMoved(this),
				MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedMouseMoved(this),
				MouseMiddleButtonReleased(this),
				MouseMiddleButtonReleasedShiftKeyPressed(this),
				MouseMiddleButtonReleasedControlKeyPressed(this),
				MouseMiddleButtonReleasedShiftKeyPressedControlKeyPressed(this),
				MouseRightButtonPressed(this),
				MouseRightButtonPressedShiftKeyPressed(this),
				MouseRightButtonPressedControlKeyPressed(this),
				MouseRightButtonPressedShiftKeyPressedControlKeyPressed(this),
				MouseRightButtonPressedMouseMoved(this),
				MouseRightButtonPressedShiftKeyPressedMouseMoved(this),
				MouseRightButtonPressedControlKeyPressedMouseMoved(this),
				MouseRightButtonPressedShiftKeyPressedControlKeyPressedMouseMoved(this),
				MouseRightButtonReleased(this),
				MouseRightButtonReleasedShiftKeyPressed(this),
				MouseRightButtonReleasedControlKeyPressed(this),
				MouseRightButtonReleasedShiftKeyPressedControlKeyPressed(this),
				MouseMoved(this),
				RotateSystem(0),
				ZoomSystem(0),
				TranslateSystem(0),
				SelectionPressedMoved(0),
				SelectionPressed(0),
				SelectionReleased(0),
				DeselectionPressedMoved(0),
				DeselectionPressed(0),
				DeselectionReleased(0),
				scene_(0)
		{
		}

		Events::Events(Scene* scene)
			:	ShiftKeyPressed(this),
				ShiftKeyPressedMouseMoved(this),
				ControlKeyPressed(this),
				ControlKeyPressedMouseMoved(this),
				ShiftKeyPressedControlKeyPressed(this),
				ShiftKeyPressedControlKeyPressedMouseMoved(this),
				MouseLeftButtonPressed(this),
				MouseLeftButtonPressedShiftKeyPressed(this),
				MouseLeftButtonPressedControlKeyPressed(this),
				MouseLeftButtonPressedShiftKeyPressedControlKeyPressed(this),
				MouseLeftButtonPressedMouseMoved(this),
				MouseLeftButtonPressedShiftKeyPressedMouseMoved(this),
				MouseLeftButtonPressedControlKeyPressedMouseMoved(this),
				MouseLeftButtonPressedShiftKeyPressedControlKeyPressedMouseMoved(this),
				MouseLeftButtonReleased(this),
				MouseLeftButtonReleasedShiftKeyPressed(this),
				MouseLeftButtonReleasedControlKeyPressed(this),
				MouseLeftButtonReleasedShiftKeyPressedControlKeyPressed(this),
				MouseMiddleButtonPressed(this),
				MouseMiddleButtonPressedShiftKeyPressed(this),
				MouseMiddleButtonPressedControlKeyPressed(this),
				MouseMiddleButtonPressedShiftKeyPressedControlKeyPressed(this),
				MouseMiddleButtonPressedMouseMoved(this),
				MouseMiddleButtonPressedShiftKeyPressedMouseMoved(this),
				MouseMiddleButtonPressedControlKeyPressedMouseMoved(this),
				MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedMouseMoved(this),
				MouseMiddleButtonReleased(this),
				MouseMiddleButtonReleasedShiftKeyPressed(this),
				MouseMiddleButtonReleasedControlKeyPressed(this),
				MouseMiddleButtonReleasedShiftKeyPressedControlKeyPressed(this),
				MouseRightButtonPressed(this),
				MouseRightButtonPressedShiftKeyPressed(this),
				MouseRightButtonPressedControlKeyPressed(this),
				MouseRightButtonPressedShiftKeyPressedControlKeyPressed(this),
				MouseRightButtonPressedMouseMoved(this),
				MouseRightButtonPressedShiftKeyPressedMouseMoved(this),
				MouseRightButtonPressedControlKeyPressedMouseMoved(this),
				MouseRightButtonPressedShiftKeyPressedControlKeyPressedMouseMoved(this),
				MouseRightButtonReleased(this),
				MouseRightButtonReleasedShiftKeyPressed(this),
				MouseRightButtonReleasedControlKeyPressed(this),
				MouseRightButtonReleasedShiftKeyPressedControlKeyPressed(this),
				MouseMoved(this),
				RotateSystem(scene),
				ZoomSystem(scene),
				TranslateSystem(scene),
				SelectionPressedMoved(scene),
				SelectionPressed(scene),
				SelectionReleased(scene),
				DeselectionPressedMoved(scene),
				DeselectionPressed(scene),
				DeselectionReleased(scene),
				scene_(scene)
		{
		}
			
		Events::Events(const Events&  /* events */, bool /* deep */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		Events::~Events()
		{
			clear();
		}

		void Events::clear()
		{
		}
			
		void Events::destroy()
		{
			clear();
		}
			
		void Events::set(const Events&  /* events */, bool /* deep */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);			
		}

		Events& Events::operator = (const Events& events)
		{
			set(events);

			return *this;
		}

		void Events::get(Events& events, bool deep) const
		{
			events.set(*this, deep);
		}

		void Events::swap(Events&  /* events */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);			
		}

		bool Events::isValid() const
		{
			return true;
		}

		Scene* Events::getScene()
		{
			return scene_;
		}

		void Events::dump
			(ostream& /* s */, Size /* depth */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);			
		}

		void Events::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void Events::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/WIDGETS/events.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#include <BALL/VIEW/WIDGETS/HTMLView.h>

#include <BALL/VIEW/WIDGETS/HTMLPage.h>

#ifdef BALL_OS_DARWIN
#	include <OpenGL/gl.h>
#else
#	include <GL/gl.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		HTMLView::HTMLView(QWidget* parent)
			: QWebEngineView(parent)
		{
			setPage(new HTMLPage(this));
			setAttribute(Qt::WA_DontCreateNativeAncestors);
			hide();	// hiding WebEngine widgets seems to be sufficient to prevent nouveau-related crashes
		}

		HTMLView::~HTMLView()
		{}

		HTMLViewDock::HTMLViewDock(HTMLView* view, QWidget* parent, const char* title)
			: DockWidget(parent, title),
			  skip_checks_(false),
			  show_error_(false),
			  html_view_(0)
		{
			checkForIncompatibleDrivers_();
			setHTMLView(view);
			registerWidget(this);
		}

		HTMLViewDock::HTMLViewDock(QWidget* parent, const char* title)
			: DockWidget(parent, title),
			  skip_checks_(false),
			  show_error_(false),
			  html_view_(0)
		{
			checkForIncompatibleDrivers_();
			setHTMLView(new HTMLView(this));
			registerWidget(this);
		}

		void HTMLViewDock::setHTMLView(HTMLView* html_view) {
			if (!html_view)
			{
				return;
			}

			if (html_view != html_view_)
			{
				delete html_view_;
				html_view_ = html_view;
			}

			if (!skip_checks_ && show_error_)
			{
				// In case of an error, replace the original widget with an error message
				// but still keep it as a member (hidden!) so that it can be restored if
				// the user decides to disable the driver checks.
				setWidget(new HTMLViewErrorWidget(this));
				return;
			}

			setWidget(html_view_);
		}

		void HTMLViewDock::resetHTMLView(bool skip_checks)
		{
			skip_checks_ = skip_checks;
			setHTMLView(html_view_);
		}

		void HTMLViewDock::checkForIncompatibleDrivers_()
		{
			show_error_ = false;
			char* vendor = (char*) glGetString(GL_VENDOR);
			if (vendor)
			{
				show_error_ = String(vendor) == "nouveau";
			}
		}
	}
}

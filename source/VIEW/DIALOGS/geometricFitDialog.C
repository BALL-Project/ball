
#include <BALL/VIEW/DIALOGS/geometricFitDialog.h>
#include <BALL/STRUCTURE/DOCKING/geometricFit.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <QtGui/qlineedit.h>
#include <QtGui/qcombobox.h>

//#define BALL_VIEW_DEBUG

namespace BALL
{
	namespace VIEW
	{
		// Constructor
		GeometricFitDialog::GeometricFitDialog(QWidget* parent, const char* name)
			throw()
			:	DockingAlgorithmDialog(parent),
			  Ui_GeometricFitDialogData()
			{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "new GeometricFitDialog " << this << std::endl;
			#endif

				setupUi(this);
				setObjectName(name);
			
				// register QWidgets of Dialog with PreferenceEntry
				// entries of them will be generated in the INIFile
				setINIFileSectionName("GEOMETRIC_FIT_OPTIONS");
				registerObject_(surface_thickness);
				registerObject_(grid_spacing);
				registerObject_(penalty_static);
				registerObject_(penalty_mobile);
				registerObject_(near_radius);
				registerObject_(deg_interval);
				registerObject_(peak_num);
				registerObject_(surface_type);
		    inifile_section_name_backup_ = "GEOMETRIC_FIT_OPTIONS_REDOCK";
				
				connect(reset_button, SIGNAL(pressed()), this, SLOT(reset()));
			}
		
		// Copy constructor.
		GeometricFitDialog::GeometricFitDialog(const GeometricFitDialog& geo_fit_dialog)
			throw()
			: DockingAlgorithmDialog(geo_fit_dialog),
			  Ui_GeometricFitDialogData()
		{}
			
		// Destructor
		GeometricFitDialog::~GeometricFitDialog()
		  throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class GeometricFitDialog" << std::endl;
			#endif 
		}
		
		// Assignment operator
		const GeometricFitDialog& GeometricFitDialog::operator =(const GeometricFitDialog& geo_fit_dialog)
			throw()
		{
			if (&geo_fit_dialog != this)
			{
				DockingAlgorithmDialog::operator=(geo_fit_dialog);
			}
			return *this;
		}
		
		// Fill options with values of the dialog.
		void GeometricFitDialog::getOptions(Options& options)
					throw()
		{
		  try
			{
				options[GeometricFit::Option::NEAR_RADIUS] = ascii(near_radius->text()).toFloat();
				options[GeometricFit::Option::GRID_SPACING] = ascii(grid_spacing->text()).toFloat();
				options[GeometricFit::Option::SURFACE_THICKNESS] = ascii(surface_thickness->text()).toFloat();
				options[GeometricFit::Option::DEGREE_INTERVAL] = ascii(deg_interval->text()).toDouble();
				options[GeometricFit::Option::TOP_N] = ascii(peak_num->text()).toInt();
				options[GeometricFit::Option::PENALTY_STATIC] = ascii(penalty_static->text()).toInt();
				options[GeometricFit::Option::PENALTY_MOBILE] = ascii(penalty_mobile->text()).toInt();
			}
		  catch (Exception::InvalidFormat)
			{
				Log.error() << "Conversion from String to float, double or int failed: invalid format! " << __FILE__ << " " << __LINE__ << std::endl;
				return;
			}
		  
			if (surface_type->currentText() == "Connolly")
			{
			  options[GeometricFit::Option::SURFACE_TYPE] = GeometricFit::CONNOLLY;
			}
		  else if (surface_type->currentText() == "van der Waals")
			{
			  options[GeometricFit::Option::SURFACE_TYPE] = GeometricFit::VAN_DER_WAALS;
			}
		  else
			{
			  options[GeometricFit::Option::SURFACE_TYPE] = GeometricFit::FTDOCK;
			}
		}
	
	} // namespace VIEW
} // namespace BALL

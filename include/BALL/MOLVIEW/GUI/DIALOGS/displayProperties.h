// $Id:

#ifndef BALL_MOLVIEW_GUI_DIALOGS_DISPLAYPROPERTIES_H
#define BALL_MOLVIEW_GUI_DIALOGS_DISPLAYPROPERTIES_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_FORMAT_INIFILE_H
#	include <BALL/FORMAT/INIFile.h>
#endif

#ifndef BALL_VIEW_KERNEL_LOGVIEW_H
#	include <BALL/VIEW/KERNEL/logView.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H
# include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>
#endif

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_MOLECULEOBJECTPROCESSOR_H
# include <BALL/MOLVIEW/GUI/FUNCTOR/moleculeObjectProcessor.h>
#endif

#ifndef BALL_MOLVIEW_KERNEL_MOLECULARMESSAGE_H
# include <BALL/MOLVIEW/KERNEL/molecularMessage.h>
#endif

#ifndef BALL_MOLVIEW_GUI_DIALOGS_DISPLAYPROPERTIESDATA_H
# include <BALL/MOLVIEW/GUI/DIALOGS/displayPropertiesData.h>
#endif

using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::MOLVIEW;

namespace BALL
{

	namespace MOLVIEW
	{

		class DisplayProperties 
			: public BALL::MOLVIEW::DisplayPropertiesData,
			  public ModularWidget
			{
				Q_OBJECT
					
					public:
				
				/**     @name Constructors and Destructors
				 */
				//@{
				
				/** Constructor for the display properties dialog.
						create a dialog for changing the appearance of selected molecular objects.
				*/
				DisplayProperties(QWidget *parent = NULL,
													const char *name = NULL );
				
				/** Destructor for the display properties dialog.
				 */
				virtual ~DisplayProperties();
				//@}
				
				/**     @name Accessors
				 */
				//@{
				
				/** Fetch the widgets preferences from the inifile.
						This method extracts the default values from the given
						inifile.
						This method is called automatically
						immediately before the main application 
						is started. It gets the widget's initial values from the inifile. 
				*/
				virtual void fetchPreferences(INIFile &inifile);
				
				/** Writes the widgets preferences to the inifile.
						This method is called by the widget's destructor.
						It writes all needed values to the given inifile (as read from
						the inifile in the fetchPreferences method).
				*/
				virtual void writePreferences(INIFile &inifile);
				
				/** receive and process messages sent to this dialog.
						If a graphical representation is changed this dialog sents a message to the other
						dialogs and widgets so that they can act upon the possibly changed state of
						some molecular objects. It also receives messages from other dialogs concerning
						the selection of some molecular objects.
						@param message - a pointer to a given message
				*/
				virtual void onNotify(Message *message);
				
				//@}
				
				/** @name Modular Widget related methods
				 */
				//@{
				
				/** Initialize the display properties dialog.
						This method is called automatically	immediately before the main application 
						is started. It adds the dialog's menu entries and connections.
				*/
				virtual void initializeWidget(MainControl& main_control);
				
				/** Remove the dialog.
						This method is called by the dialog's destructor.
						It reverses all actions performed in initializeWidget 
						(remove menu entries and connections).
				*/
				virtual void finalizeWidget(MainControl& main_control);
				
				/** Update all menu entry states.
						This method is called just before a popup menu is shown 
						(via the QT signal aboutToShow()).
						It should be used to update the state of menu entries 
						(e.g. disable or enable entries).
				*/
				virtual void checkMenu(MainControl& main_control);
				//@}
				
				public slots:
					
					/** @name Public slots
					 */
					//@{

					/** All selected objects will be colored uniquely.
					*/
					void select();

					/** The color of the selected objects will be turned to their previously
							chosen color.
					*/
				  void deselect();

					/** The camera will be focused to the center of the selected objects.
					*/
					void centerCamera();

					/** Bonds will be created between the selected objects.
					*/
					void buildBonds();

					/** Hydrogens will be added to the selected objects.
					*/
					void addHydrogens();
					
					/** open the dialog.
					 */
					void openDialog();
				
				//@}
				
				protected slots:
					
					/** @name Protected slots
					 */
					
				/** @name Protected slots
				 */
				
				/** changes the precision of the selected objects.
				 */
				virtual void selectPrecision(const QString& string);
				
				/** changes the model of the selected objects
				 */
				virtual void selectModel(const QString& string);
				
				/** changes the coloring methods of the selected objects.
				 */
				virtual void selectColoringMethod(const QString& string);
				
				/** indicates that the apply button is pressed.
				 */
				virtual void applyButtonClicked();
				
				/** open the edit color dialog.
				 */ 
				virtual void editColor();
				
				
			private:
				
				void setComboBoxIndex_(QComboBox* combo_box, QString& item_string);
				
				int id_;
				int select_id_;
				int deselect_id_;
				int center_camera_id_;
				int build_bonds_id_;
				int add_hydrogens_id_;
				
				QString   model_string_;
				QString   precision_string_;
				QString   coloring_method_string_;
				ColorRGBA custom_color_;
				
				MoleculeObjectProcessor object_processor_;
				
				AtomDistanceColorCalculator distance_color_calculator_;
				bool distance_coloring_;
				
				List<Composite*> selection_;
			};

		//#		ifndef BALL_NO_INLINE_FUNCTIONS
		//#			include <BALL/MOLVIEW/GUI/DIALOGS/displayProperties.iC>
		//#		endif 

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_DIALOGS_DISPLAYPROPERTIES_H

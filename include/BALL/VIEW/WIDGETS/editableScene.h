/*    THIS IS EXPERIMENTAL CODE  
 *
 *    WE GIVE NO WARRANTY
 *    
 *    USE AT YOUR OWN RISK!!!!!!
 */

#ifndef BALL_VIEW_WIDGETS_EDITABLESCENE_H
#define BALL_VIEW_WIDGETS_EDITABLESCENE_H


#ifndef BALL_KERNEL_SYSTME_H
#include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_VIEW_WIDGETS_SCENE_H
#include <BALL/VIEW/WIDGETS/scene.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
#include <BALL/KERNEL/molecule.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif 

#ifndef BALL_MATHS_VECTOR2_H
# include <BALL/MATHS/vector2.h>
#endif

#ifndef BALL_STRUCTURE_FRAGMENTDB_H
# include <BALL/STRUCTURE/fragmentDB.h>
#endif

#include <QtGui/qcursor.h>

namespace BALL
{
	namespace VIEW
	{

class EditSettings;

/** Extended 3D view for editing molecules.
 		The editing functionality is implemented as an own mode.
		\ingroup ViewWidgets
*/
class BALL_VIEW_EXPORT EditableScene
	: public Scene
{
	Q_OBJECT //macro for QT-Messages

	public:

	BALL_EMBEDDABLE(EditableScene, ModularWidget)	

	// This class represents a single edit operation and stores its type and
	// parameters. This can be used to implement undo functionality.
	class BALL_VIEW_EXPORT EditOperation
	{
		public:

		/// Constructor
		EditOperation();

		///
		EditOperation(Atom* atom, Bond* bond, String description= "Added Object", int operation=0);

		///
		EditOperation(const EditOperation& eOperation);

		/// Destructor.
		virtual ~EditOperation();

		///
		enum OperationType
		{
			DEFAULT,
			ADDED__ATOM,
			ADDED__BOND,
			CHANGED__TYPE
				// DELETED__ATOM
				// DELETED__BOND
				//CHANGED__SYSTEM
				//MOVED__ATOM
		};

		int	operationType;
		Atom* atom;
		Bond* bond;
		String description;
	};

	///
	enum EditMode
	{
		// 
		EDIT__MODE = PICKING__MODE + 1
	};

	///
	EditableScene();

	/// Default Constructor
	EditableScene(QWidget* parent_widget, const char* name = NULL, Qt::WFlags w_flags = 0);

	/// Copy constructor.
	EditableScene (const EditableScene& eScene, QWidget* parent_widget = NULL, const char* name = NULL, Qt::WFlags wflags = 0);

	/// Destructor.
	virtual ~EditableScene();

	///
	void initializeWidget(MainControl& main_control);

	///
	void checkMenu(MainControl& main_control);

	///
	void initializePreferencesTab(Preferences &preferences);
	
	///
	void finalizePreferencesTab(Preferences &preferences);

	///
	virtual void applyPreferences();
	
	///
	void showContextMenu(QPoint pos);

	///
	virtual void setMode(ModeType mode);

	void addStructure(String name);

	///
	void setCursor(String c);

	///
	void setElementCursor();

	////////////////////////////////////////
	public slots:

	virtual void mousePressEvent(QMouseEvent* e);
	virtual void mouseMoveEvent(QMouseEvent *e);
	virtual void mouseReleaseEvent(QMouseEvent *e);
	virtual void wheelEvent(QWheelEvent* qmouse_event);
	void createNewMolecule();
	void saturateWithHydrogens();
	void optimizeStructure();
	void computeBondOrders();

	// slots for communication with PTEDialog
	void setEditElementType(int element_number);
	int getEditElementType();
							
	////////////////////////////////////////
	protected slots:

	virtual void editMode_();
	void deleteAtom_();
	void changeElement_();
	void deleteBond_();
	void changeBondOrder_();
	void activatedOrderItem_(QAction* action);
	void moveAtom_();
	void atomProperties_();
	void createMolecule_();
	void addStructure_();
	void setFormalCharge_();

	////////////////////////////////////////
	signals:

	// signal for communication with EditOperationDialog
	void newEditOperation(EditableScene::EditOperation &eo);

	////////////////////////////////////////
	protected:

	virtual void addToolBarEntries(QToolBar* tb);

	/// Catch key events
	virtual void keyPressEvent(QKeyEvent* e);
	virtual void mouseDoubleClickEvent(QMouseEvent* e);
	virtual bool reactToKeyEvent_(QKeyEvent* e);

	virtual void onNotify(Message *message);

	/**
	 * Insert a given Atom in the Scene. Its position is specified by the 2-dim 
	 * Mouseclick coordinates of the Screen, which will be translated into the 
	 * 3-dim space of Viewing Volume.
	 */
	void insert_(int x_, int y_, PDBAtom &atom_);

	void merge_(Composite* a1, Composite* a2);

	/**
	 *  Given a 3-dim. Coordinates (in Viewing Volume) getScreenPosition
	 *  computes the 2-dim Coordinates on Screen.
	 */
	TVector2<float> getScreenPosition_(Vector3 vec);

	void getClickedItems_(int x, int y);

	void init_();

	String getBondOrderString_(Index order);

	List<AtomContainer*> getContainers_();

	void changeBondOrder_(Index delta);
	void deselect_(bool update=true);
	void renderGrid_();

	QAction* edit_id_, *new_molecule_action_, *optimize_action_, *add_hydrogens_action_, *element_action_;	
	QAction* bondorders_action_;
	
	Atom* current_atom_;
	Bond* current_bond_;

	Vector3 atom_pos_;

	// pick atoms/bonds only in highlighted AtomContainer?
	static bool only_highlighted_;
	// element for new atoms
	int atomic_number_;
	// name for newly created atoms
	Position atom_number_;
	// order for new bonds
	int bond_order_;
	Position last_y_;
	Qt::MouseButtons last_buttons_;

	//undo stack
	vector<EditOperation> undo_;
	EditSettings* edit_settings_;
	QPoint 	 menu_point_;
	FragmentDB fragment_db_;
	bool fragment_db_initialized_;
	bool temp_move_;
	QToolBar* toolbar_edit_controls_;
	QList<QAction*> toolbar_actions_edit_controls_;
};

	}//end of namespace
} // end of namespace
#endif

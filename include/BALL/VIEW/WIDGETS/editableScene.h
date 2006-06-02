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

namespace BALL
{
	namespace VIEW
	{

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
		EditOperation()
			throw();

		///
		EditOperation(Atom* atom, Bond* bond, String description= "Added Object", int operation=0)
			throw();

		///
		EditOperation(const EditOperation& eOperation)
			throw();

		/// Destructor.
		virtual ~EditOperation()
			throw();

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
		// inserting atoms
		ATOM__MODE = PICKING__MODE + 1,

		// drawing new bonds
		BOND__MODE	
	};

	///
	EditableScene()
		throw();

	/// Default Constructor
	EditableScene(QWidget* parent_widget, const char* name = NULL, Qt::WFlags w_flags = 0)
		throw();

	/// Copy constructor.
	EditableScene (const EditableScene& eScene, QWidget* parent_widget = NULL, const char* name = NULL, Qt::WFlags wflags = 0)
		throw();

	/// Destructor.
	virtual ~EditableScene()
		throw();

	/// Explicit default initialization.
	virtual void clear()
		throw();

	///
	void initializeWidget(MainControl& main_control)
		throw();

	///
	void finalizeWidget(MainControl& main_control)
		throw();

	///
	void checkMenu(MainControl& main_control)
		throw();

	////////////////////////////////////////
	public slots:

	virtual void mousePressEvent(QMouseEvent* e);
	virtual void mouseMoveEvent(QMouseEvent *e);
	virtual void mouseReleaseEvent(QMouseEvent *e);

	// slots for communication with PTEDialog
	void setEditElementType(int element_number);
	int getEditElementType();
							
	////////////////////////////////////////
	protected slots:

	virtual void atomMode_();
	virtual void bondMode_();

	////////////////////////////////////////
	signals:

	// signal for communication with EditOperationDialog
	void newEditOperation(EditableScene::EditOperation &eo);

	////////////////////////////////////////
	protected:

	/**
	 * Insert a given Atom in the Scene. Its position is specified by the 2-dim 
	 * Mouseclick coordinates of the Screen, which will be translated into the 
	 * 3-dim space of Viewing Volume.
	 */
	void insert_(int x_, int y_, PDBAtom &atom_);

	/**
	 *  Given a 3-dim. Coordinates (in Viewing Volume) getScreenPosition
	 *  computes the 2-dim Coordinates on Screen.
	 */
	TVector2<Position> getScreenPosition_(Vector3 vec);

	/**
	 * Given 2-dim Coordinates of Screen, clickedPointOnViewPlane computes the
	 * appropriate 3-dim Position in Viewing Volume
	 */
	Vector3 clickedPointOnViewPlane_(int x, int y);

	/**
	 * Given 2-dim Coordinates of Screen, getCLickedAtom_ returns the nearest Atom 
	 * within a special radius. If no atom is found, getClickedAtom returns NULL.
	 */
	Atom* getClickedAtom_(int x, int y);

	/**
	 * Given 2-dim Coordinates of Screen, getCLickedBond returns the nearest Bond
	 * within a special radius. If no bond is found, getClickedBond returns NULL.
	 * Note: This code is very similar to that of getClickedAtom and the two might
	 *       be joined in the future.
	 */
	Bond* getClickedBond_(int x, int y);

	/**
	 * Maps the current viewplane to screen coordinates.
	 * Returns false if the projection matrix is not correctly
	 * initialized.
	 */
	bool mapViewplaneToScreen_();

	virtual void paintGL();

	QAction* atom_id_, *bond_id_;	
	Atom* current_atom_;

	Vector3 near_left_bot_;  //TODO:: name in XYZ left_bot_mnear
	Vector3 near_right_bot_;
	Vector3 near_left_top_;

	Vector3 atom_pos_;
	bool draw_line_;

	// search range when looking for atoms/bonds (in angstrom)
	double atom_limit_;			
	double bond_limit_;			
	// element for new atoms
	int 	 atom_type_;

	//undo stack
	vector< EditOperation > undo_;
};

	}//end of namespace
} // end of namespace
#endif

#ifndef BALL_VIEW_KERNEL_MODES_EDITMODE_H
#define BALL_VIEW_KERNEL_MODES_EDITMODE_H

#include <BALL/VIEW/KERNEL/MODES/interactionMode.h>

#include <QtGui/QMouseEvent>

#include <vector>

namespace BALL
{
	class Composite;
	class FragmentDB;
	class PDBAtom;

	namespace VIEW
	{

		class EditMode : public InteractionMode
		{
			Q_OBJECT

			public:
				EditMode(Scene* scene);
				virtual ~EditMode();

				virtual void keyPressEvent(QKeyEvent* evt);
				virtual void keyReleaseEvent(QKeyEvent* evt);

				virtual void mouseDoubleClickEvent(QMouseEvent* evt);

				virtual void wheelEvent(QWheelEvent* evt);

				virtual void addToolBarEntries(QToolBar* tb);

				virtual Qt::Key getKey() const;
				virtual QString getName() const { return "EditMode"; }

				virtual void activate();
				virtual void populateContextMenu(QMenu* menu);

			protected slots:
				virtual void addStructure_();
				virtual void setFormalCharge_();
				virtual void changeBondOrder_();
				virtual void activatedOrderItem_(QAction* action);
				virtual void deleteCurrentBondTriggered_();
				virtual void deleteCurrentAtomTriggered_();
				virtual void atomPropertiesTriggered_();
				virtual void changeAtomElementTriggered_();
				virtual void createBond_();

			protected:
				bool temp_move_;
				int atom_number_;
				int bond_order_;
				Qt::MouseButtons last_buttons_;
				FragmentDB* fragment_db_;
				QAction* element_action_;
				QAction* bond_action_;
				QAction* delete_atom_;
				QAction* change_charge_;
				QAction* atom_properties_;
				QMenu* charge_menu_;
				QMenu* order_menu_;
				QMenu* add_menu_;
				std::vector<QAction*> order_actions_;

				virtual void mouseMoveEventImpl_   (QMouseEvent* evt);
				virtual void mousePressEventImpl_  (QMouseEvent* evt);
				virtual void mouseReleaseEventImpl_(QMouseEvent* evt);

				void insert_(int x, int y, PDBAtom &atom);
				void merge_(Composite* a1, Composite* a2);
				virtual void createActions_();

			private:
				void checkBondOrderActions_(unsigned int bo);
				void createContextMenuEntries_();
		};

	}
}

#endif //BALL_VIEW_KERNEL_MODES_EDITMODE_H

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_KERNEL_REPRESENTATIONCREATOR_H
#define BALL_VIEW_KERNEL_REPRESENTATIONCREATOR_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_MODELS_REPRESENTATIONBUILDER_H
# include <BALL/VIEW/MODELS/representationBuilder.h>
#endif

#include <QtCore/QObject>

namespace BALL
{
	namespace VIEW
	{
		/** Non-dialog NEW_MOLECULE auto-create subscriber.

				Phase 999.67 Plan 03 (LEGACYDEL-01): re-homes the molecule-load
				default-representation behaviour off the now-deleted DisplayProperties
				settings dialog and onto this minimal, headless ModularWidget.

				DisplayProperties was load-bearing only because it was the
				CompositeMessage::NEW_MOLECULE subscriber that auto-built a default
				representation whenever a molecule was added to the scene. Phase 999.65
				had already moved the actual representation-building ENGINE off the dialog
				and onto the headless RepresentationBuilder (driven by a value-object
				RepresentationSpec); the dialog's onNotify merely forwarded NEW_MOLECULE
				into RepresentationBuilder::createRepresentation via a one-method Notifier
				adapter. So the re-home is a subscriber-registration move, not an
				algorithm port: this class registers as a ModularWidget (and therefore as
				a ConnectionObject in the MainControl message tree), receives
				NEW_MOLECULE, builds a DEFAULT RepresentationSpec (== the legacy
				out-of-box default representation, see representationBuilder.h:62), and
				calls the already-wired RepresentationBuilder::createRepresentation reusing
				a trivial Notifier adapter.

				This class derives from QObject (NOT QWidget): it must be a QObject so
				ModularWidget::registerWidget can walk the QObject parent chain to find
				the MainControl and wire it into the message tree, but it contributes no
				visible UI, no menu, no toolbar entry and no preferences tab — it is purely
				the auto-create subscriber. The interactive model/coloring/material
				editing surface lives in the Inspector (v1.7.4 / 999.65 supersession
				surface).

				Both BALLView and the headless DCD2PNG batch renderer construct exactly
				one of these (parented to the MainControl) so that loading a molecule
				(interactive) or feeding a frame (batch) still auto-builds the default
				representation.
				\ingroup ViewKernelConnectivity
		*/
		class BALL_VIEW_EXPORT RepresentationCreator
			: public QObject,
				public ModularWidget
		{
			Q_OBJECT

			public:

			BALL_EMBEDDABLE(RepresentationCreator, ModularWidget)

			/** Default Constructor.
					Must be created with the MainControl as parent. Calls
					ModularWidget::registerWidget so the instance is wired into the
					MainControl message tree as a NEW_MOLECULE subscriber.
					\param parent the MainControl (passed as QObject parent so registration
					              can locate it)
					\param name   the object name
			*/
			RepresentationCreator(QObject* parent = 0, const char* name = "RepresentationCreator");

			/// Destructor.
			virtual ~RepresentationCreator();

			/** Message handling method.
					On CompositeMessage::NEW_MOLECULE (and only when auto-create is enabled)
					builds a default RepresentationSpec and drives
					RepresentationBuilder::createRepresentation, exactly the behaviour
					DisplayProperties used to provide.
					\param message the message to process
			*/
			virtual void onNotify(Message* message);

			/** Set whether representations are automatically created for new molecules.
					Re-homed from DisplayProperties::enableCreationForNewMolecules — the
					suppress-during-load guard MainControl toggles around project-file loads
					(which restore their own saved representations) and the demo tutorial
					toggles around its scripted PDB load.
					\param state true to auto-create on NEW_MOLECULE, false to suppress
			*/
			void enableCreationForNewMolecules(bool state)
				{ create_representations_for_new_molecules_ = state; }

			/** Get whether representations are automatically created for new molecules. */
			bool getCreationForNewMolecules() const
				{ return create_representations_for_new_molecules_; }

			protected:

			/** One-method RepresentationBuilder::Notifier adapter that forwards the
					builder's messages into this object's own protected notify_().

					RepresentationBuilder is pure logic and cannot reach
					ConnectionObject::notify_ on a foreign object (it is protected), so the
					builder takes a Notifier callback (Strategy A, access-adapted; see
					Phase 999.65 Plan 01). As a nested class, Notifier_ may call the
					enclosing RepresentationCreator's protected notify_, forwarding the same
					ADD_TO_GEOMETRIC_CONTROL / CENTER_CAMERA sequence the legacy
					DisplayProperties::createRepresentation fired. */
			class Notifier_ : public RepresentationBuilder::Notifier
			{
				public:
				Notifier_(RepresentationCreator& owner) : owner_(owner) {}
				void notify(Message* message) override { owner_.notify_(message); }

				private:
				RepresentationCreator& owner_;
			};

			bool create_representations_for_new_molecules_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_REPRESENTATIONCREATOR_H

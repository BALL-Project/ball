// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_MODELS_REPRESENTATIONBUILDER_H
#define BALL_VIEW_MODELS_REPRESENTATIONBUILDER_H

#ifndef BALL_VIEW_MODELS_MODELPROCESSORFACTORY_H
#	include <BALL/VIEW/MODELS/modelProcessorFactory.h>
#endif

#ifndef BALL_VIEW_MODELS_COLORPROCESSORFACTORY_H
#	include <BALL/VIEW/MODELS/colorProcessorFactory.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
#	include <BALL/VIEW/KERNEL/common.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
#	include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

#ifndef BALL_DATATYPE_HASHSET_H
#	include <BALL/DATATYPE/hashSet.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#include <list>
#include <vector>

namespace BALL
{
	class Composite;

	namespace VIEW
	{
		class Representation;
		class MainControl;
		class ModelInformation;
		class Message;
		class ModelProcessor;
		class ColorProcessor;

		/** Plain-data specification for RepresentationBuilder::createRepresentation.

				This struct is the headless replacement for every value that the legacy
				createRepresentation engine read from its widgets (model-type /
				coloring-method comboboxes, the mode combobox, the precision combobox +
				slider + custom-precision radio, the transparency slider, the custom-color
				button, and the two model/coloring update-enable flags — see
				the legacy DIALOGS engine, lines 444-563 + 809-833). Relocating
				these values into an explicit value object lets the
				representation-creation ENGINE live in a free-standing builder that has no
				dependency on any Qt widget, settings panel, slider or checkbox
				(VIEW-CLEAN-04).

				Every field is initialized to the legacy out-of-box default so that a
				DEFAULT-CONSTRUCTED RepresentationSpec yields exactly the same fresh
				representation that the NEW_MOLECULE path produces today for a freshly
				opened settings panel (model defaults to MODEL_STICK per the legacy DIALOGS engine line 135; the embedded
				ModelProcessorParams / ColoringOverrides default to the legacy out-of-box
				render per CONFIG-01).
				\see RepresentationBuilder
				\ingroup ViewModels
		*/
		struct BALL_VIEW_EXPORT RepresentationSpec
		{
			/// Model type to build (default MODEL_STICK, the fresh-panel default, legacy DIALOGS engine line 135).
			ModelType model_type = MODEL_STICK;

			/// Coloring method to apply (default COLORING_ELEMENT, the fresh-panel default).
			ColoringMethod coloring_method = COLORING_ELEMENT;

			/// Drawing mode (dots / wireframe / solid / toon); default DRAWING_MODE_SOLID.
			DrawingMode drawing_mode = DRAWING_MODE_SOLID;

			/// Preset drawing precision (low / medium / high / ultra); default DRAWING_PRECISION_HIGH.
			DrawingPrecision drawing_precision = DRAWING_PRECISION_HIGH;

			/** Explicit surface drawing precision, used only when
					@c use_custom_surface_precision is true (the custom-precision branch at
					legacy DIALOGS engine lines 487-490). When false the precision is derived from
					@c drawing_precision via SurfaceDrawingPrecisions[]. */
			float surface_drawing_precision = 6.5f;

			/** Whether to honor @c surface_drawing_precision directly instead of mapping
					@c drawing_precision through SurfaceDrawingPrecisions[] (the
					custom-precision-button-checked branch, legacy DIALOGS engine lines 487). */
			bool use_custom_surface_precision = false;

			/// Transparency (0 - 255); default 0 (fully opaque).
			Size transparency = 0;

			/// Custom default color (used by COLORING_CUSTOM and as the processor default color).
			ColorRGBA custom_color;

			/// Whether the model is rebuilt on update (Representation::enableModelUpdate); default true.
			bool model_update_enabled = true;

			/// Whether the coloring is rebuilt on update (Representation::enableColoringUpdate); default true.
			bool coloring_update_enabled = true;

			/** The model-processor settings the builder bakes into the constructed
					ModelProcessor via ModelProcessorFactory::create. Carries the cartoon
					DNA-ladder and ribbons-enabled knobs that the legacy applyModelSettings_
					re-pushed through the settings panel's applySettingsTo — they are
					explicit fields here, so no widget re-apply is needed. */
			ModelProcessorParams model_params;

			/** The color-processor overrides the builder layers onto the constructed
					ColorProcessor via ColorProcessorFactory::create. Carries the surface
					customizations the legacy applyColoringSettings_ re-pushed through
					the coloring panel's applySettingsTo — explicit fields here, so no
					widget re-apply is needed. A default-constructed value keeps the
					processor's compiled defaults (CONFIG-01). */
			ColoringOverrides coloring_overrides;
		};

		/** Headless engine that builds a fully-configured Representation from a
				RepresentationSpec.

				This class relocates the representation-creation orchestration that used
				to live in the legacy createRepresentation / applyModelSettings_ /
				applyColoringSettings_ trio (the legacy DIALOGS engine, lines 444-611) off the settings panel and onto an explicit RepresentationSpec
				value object. It consumes the headless 999.57 ModelProcessorFactory /
				ColorProcessorFactory and therefore has NO dependency on any Qt widget,
				settings panel, slider or checkbox (VIEW-CLEAN-04).

				MESSAGING STRATEGY: createRepresentation must fire the same message
				sequence the legacy engine fired (ADD_TO_GEOMETRIC_CONTROL, then
				CENTER_CAMERA when fewer than two representations exist) to wire the new
				representation into GeometricControl and center the camera. Because
				ConnectionObject::notify_ is PROTECTED, the builder cannot send messages
				through an arbitrary ConnectionObject& from outside its hierarchy.
				Instead it accepts a lightweight Notifier callback (Strategy A,
				access-adapted): the caller — the settings panel in Plan 02, the
				RepresentationManager in Plan 03, both ConnectionObjects — supplies a
				trivial adapter whose notify() forwards to its own protected notify_().
				This keeps the builder pure logic with zero ConnectionObject lifecycle.
				\see RepresentationSpec
				\ingroup ViewModels
		*/
		class BALL_VIEW_EXPORT RepresentationBuilder
		{
			public:

			/** Message sink for createRepresentation.

					The builder hands fully-constructed Message objects (ownership
					transferred) to notify(); the implementing caller forwards them to its
					own ConnectionObject::notify_(). This indirection exists solely because
					notify_ is protected and cannot be reached on a foreign
					ConnectionObject&. */
			class BALL_VIEW_EXPORT Notifier
			{
				public:
				virtual ~Notifier() {}

				/** Forward @p message into the message system (ownership transferred to
						the implementer, mirroring ConnectionObject::notify_(Message*)). */
				virtual void notify(Message* message) = 0;
			};

			/** Build a fully-configured Representation from @p spec and wire it in.

					Mirrors the legacy createRepresentation engine
					(legacy DIALOGS engine lines 566-611) headlessly: allocates a new
					Representation, applies the coloring then the model settings (same
					order as the legacy 576/577), copies the composites, sets the hidden
					flag, inserts the representation into the RepresentationManager, then
					notifies ADD_TO_GEOMETRIC_CONTROL and — when fewer than two
					representations exist — CENTER_CAMERA. Updates the representation unless
					hidden.
					@param spec         the headless replacement for the dialog widget reads
					@param composites   the composites the representation visualizes
					@param main_control the MainControl owning the RepresentationManager
					@param notifier     message sink (see Notifier); messages are sent
					                    through it because notify_ is protected
					@param model_information used for the surface-model precision branch
					@param hidden       whether the representation is created hidden
					@return the newly created Representation, or NULL on empty/invalid input
			*/
			static Representation* createRepresentation(const RepresentationSpec& spec,
																									const std::list<Composite*>& composites,
																									MainControl& main_control,
																									Notifier& notifier,
																									const ModelInformation& model_information,
																									bool hidden = false);

			/** Apply @p spec's model settings to @p rep.
					Mirrors the legacy applyModelSettings_
					(legacy DIALOGS engine lines 444-510): the recreate-branch guard, the
					ModelProcessorFactory::create call, setModelProcessor / setModelType,
					setDrawingMode and the custom-vs-presets surface-precision branch.
					@param rep               the representation to configure
					@param spec              the model settings to apply
					@param model_information used for isSurfaceModel() in the precision branch
			*/
			static void applyModelSettings(Representation& rep,
																		 const RepresentationSpec& spec,
																		 const ModelInformation& model_information);

			/** Apply @p spec's coloring settings to @p rep.
					Mirrors the legacy applyColoringSettings_
					(legacy DIALOGS engine lines 513-563): the recreate-branch guard, the
					ColorProcessorFactory::create call, setColorProcessor /
					setColoringMethod, setTransparency, the custom-color alpha, the SE/SA
					surface min_spacing rule and setDefaultColor.
					@param rep  the representation to configure
					@param spec the coloring settings to apply
					@param model_information (reserved; the coloring branch needs no surface query)
			*/
			static void applyColoringSettings(Representation& rep,
																				 const RepresentationSpec& spec,
																				 const ModelInformation& model_information);

			/** Result of parseDataString: the spec plus the project-restore fields the
					caller (Plan 03) needs to finish rebuilding a saved representation. */
			struct BALL_VIEW_EXPORT ParsedRepresentation
			{
				/// The reconstructed representation specification (6 parsed fields).
				RepresentationSpec spec;

				/// The composite indices (composite tree positions) the representation covers.
				HashSet<Position> composite_indices;

				/// The system number the representation belongs to (first ';'-split token).
				Position system_index = 0;

				/// Whether the representation was saved hidden (the trailing "|H" flag).
				bool hidden = false;

				/// The custom default color extracted from the "|color|" block (if present).
				ColorRGBA custom_color;

				/** The raw, still-base64-encoded named-property blocks (the trailing
						'\\'-then-'*'-split sequence). They are NOT decoded here — Plan 03
						replays them with TextPersistenceManager + notify_. */
				std::vector<String> named_property_blocks;
			};

			/** Parse a project-file representation data-string into a spec + restore set.

					Reproduces the legacy getSettingsFromString
					(legacy DIALOGS engine lines 809-833) plus the field splitting of the legacy
					createRepresentation(String,...)
					(legacy DIALOGS engine lines 835-913) WITHOUT touching any widget. The data
					string follows the Representation::toString layout
					(representation.C:551-595):

					  "<system>;<model> <mode> <precision> <surf_precision> <coloring> <transparency> [<id,id,...>]|<color>|<H?><named props>"

					The 6 space-separated settings fields are: model type, drawing mode,
					drawing precision, surface drawing precision, coloring method,
					transparency. This helper fills @p out.spec from those, @p
					out.composite_indices from the "[]"-delimited id list, @p
					out.system_index from the leading ';' token, @p out.hidden from the
					trailing "|H", @p out.custom_color from the "|color|" block and @p
					out.named_property_blocks from the raw trailing base64 sequence (left
					encoded for Plan 03).
					@param data_string      the project-file data string to parse
					@param out              receives the parsed fields on success
					@param model_information used for the model-muteable gate (modelMuteableBy... in ModelInformation)
					@return true on a well-formed string (>= 6 settings fields, a muteable
					        model, parseable numerics), false otherwise (mirrors the legacy
					        early-returns)
			*/
			static bool parseDataString(const String& data_string,
																	ParsedRepresentation& out,
																	const ModelInformation& model_information);

			/** @name Owner-narrowing mutation surface (ARCHITECTURE-CONTRACT.md §3c)

					RepresentationBuilder is the SINGLE builder friend of Representation;
					its private setters (setX_) are reachable only here. The builder/file-IO
					call sites enumerated in §3a (gridVisualizationDialog, labelDialog,
					standardDatasets, geometricControl, displayProperties residual,
					molecularControl, fieldLineCreator, scene.C raytracer-material, plus the
					RepresentationManager / scene picking-rep construction paths and the
					ModelController/ColoringController/MaterialController/LabelController
					owner-mutation steps) call these thin static forwarders rather than the
					now-private Representation setters directly. Each forwards verbatim to
					the matching private Representation::setX_(), so behaviour is unchanged —
					the only change is that the mutation now flows through the one legal
					friend instead of a public setter. */
			//@{

			/// §3c — forward to the private Representation::setModelType_.
			static void setModelType(Representation& rep, ModelType type);

			/// §3c — forward to the private Representation::setColoringMethod_.
			static void setColoringMethod(Representation& rep, ColoringMethod method);

			/// §3c — forward to the private Representation::setDrawingMode_.
			static void setDrawingMode(Representation& rep, DrawingMode mode);

			/// §3c — forward to the private Representation::setDrawingPrecision_.
			static void setDrawingPrecision(Representation& rep, DrawingPrecision precision);

			/// §3c — forward to the private Representation::setSurfaceDrawingPrecision_.
			static void setSurfaceDrawingPrecision(Representation& rep, float precision);

			/// §3c — forward to the private Representation::setTransparency_.
			static void setTransparency(Representation& rep, Size value);

			/// §3c — forward to the private Representation::setModelProcessor_.
			static void setModelProcessor(Representation& rep, ModelProcessor* processor);

			/// §3c — forward to the private Representation::setColorProcessor_.
			static void setColorProcessor(Representation& rep, ColorProcessor* processor);

			/// §3c — forward to the private Representation::setComposites_.
			static void setComposites(Representation& rep, const std::list<const Composite*>& composites);

			/// §3c — forward to the private Representation::setComposite_.
			static void setComposite(Representation& rep, const Composite* composite);

			/// §3c — forward to the private Representation::setHidden_.
			static void setHidden(Representation& rep, bool state);

			/**
			 * Mark the color processor dirty so the next update(false) re-walks
			 * it WITHOUT a geometry rebuild (999.63 fix). A value-range-only
			 * change (ColoringController::setRange) mutates the live
			 * InterpolateColorProcessor's min/max but swaps no processor, so
			 * Representation's changed_color_processor_ gate stays false and the
			 * recolor is skipped. This friend flips that gate — a cheap recolor,
			 * not the heavy rebuild update(true) would force (critical for the
			 * SE-surface <50ms target). §3c friend access.
			 */
			static void markColorProcessorChanged(Representation& rep);

			//@}
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_MODELS_REPRESENTATIONBUILDER_H

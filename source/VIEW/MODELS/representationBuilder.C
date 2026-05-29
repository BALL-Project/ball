// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/MODELS/representationBuilder.h>

#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/representationManager.h>
#include <BALL/VIEW/KERNEL/modelInformation.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/MODELS/colorProcessor.h>
#include <BALL/VIEW/MODELS/surfaceModel.h>

#include <BALL/CONCEPT/composite.h>

using namespace BALL;
using namespace BALL::VIEW;

namespace BALL
{
	namespace VIEW
	{

		void RepresentationBuilder::applyModelSettings(Representation& rep,
																									 const RepresentationSpec& spec,
																									 const ModelInformation& model_information)
		{
			// Mirrors the legacy applyModelSettings_ (DIALOGS engine lines 444-510),
			// with every dialog widget read replaced by the matching RepresentationSpec
			// field. The recreate-branch guard is preserved verbatim so the processor is
			// (re)constructed under exactly the same conditions as the legacy code.
			if (rep.getModelProcessor() == 0 ||
					rep.getModelType() != spec.model_type ||
					!rep.modelUpdateEnabled())
			{
				// Construct the model processor through the headless ModelProcessorFactory
				// (999.57). The params carried by the spec mirror exactly the mapping the
				// legacy ModelSettingsDialog::createModelProcessor used, so the rendered
				// model is byte-identical.
				rep.setModelProcessor_(ModelProcessorFactory::create(spec.model_type, spec.model_params));
				rep.setModelType_(spec.model_type);
			}

			rep.setDrawingMode_(spec.drawing_mode);

			if (spec.use_custom_surface_precision)
			{
				rep.setSurfaceDrawingPrecision_(spec.surface_drawing_precision);
			}
			else
			{
				rep.setDrawingPrecision_(spec.drawing_precision);

				if (model_information.isSurfaceModel(spec.model_type))
				{
					rep.setSurfaceDrawingPrecision_(SurfaceDrawingPrecisions[spec.drawing_precision]);
				}
			}

			// NOTE: the legacy applyModelSettings_ followed this with a settings-panel
			// applySettingsTo(*rep.getModelProcessor()) re-apply. That re-pushed the
			// panel's live widget state (incl. the cartoon DNA-ladder + ribbons checkboxes
			// that had no public getter) onto the processor. Here those knobs are explicit
			// ModelProcessorParams fields already baked in by the factory above, and there
			// is no panel — so the re-apply is dropped. Behavior is preserved because
			// re-applying the same params is idempotent.
		}


		void RepresentationBuilder::applyColoringSettings(Representation& rep,
																											const RepresentationSpec& spec,
																											const ModelInformation& /* model_information */)
		{
			// Mirrors the legacy applyColoringSettings_ (DIALOGS engine lines 513-563),
			// driven by the spec instead of dialog widgets. Recreate-branch guard kept
			// verbatim.
			if (rep.getColorProcessor() == 0 ||
					rep.getColoringMethod() != spec.coloring_method ||
					!rep.coloringUpdateEnabled())
			{
				// Construct the color processor through the headless ColorProcessorFactory
				// (999.57). The overrides carried by the spec are the exact set the legacy
				// dialog packed, so the rendered colors are byte-identical.
				rep.setColorProcessor_(
					ColorProcessorFactory::create(spec.coloring_method, spec.coloring_overrides));
				rep.setColoringMethod_(spec.coloring_method);
			}

			Size transparency = spec.transparency;
			ColorRGBA custom_color = spec.custom_color;
			// custom_color alpha = 255 - transparency (legacy DIALOGS engine line 542).
			custom_color.setAlpha((unsigned char)(255 - transparency));
			rep.setTransparency_(transparency);

			ColorProcessor* cp = rep.getColorProcessor();

			// NOTE: the legacy applyColoringSettings_ followed this with a coloring-panel
			// applySettingsTo(*cp) re-apply. That re-pushed the panel's live overrides onto
			// the processor. Here the spec's coloring_overrides already carry those
			// customizations (baked in by the factory above), and there is no panel — so
			// the re-apply is dropped. Re-applying the same overrides is idempotent, so
			// behavior is preserved.

			// SE/SA surface min_spacing rules (legacy DIALOGS engine lines 552-560).
			float min_spacing = 2.;
			if (rep.getModelType() == MODEL_SE_SURFACE)
			{
				min_spacing = 2.62;
			}
			else if (rep.getModelType() == MODEL_SA_SURFACE)
			{
				min_spacing = 2.62 + ((AddSurfaceModel*)rep.getModelProcessor())->getProbeRadius();
			}
			cp->setMinGridSpacing(min_spacing);
			cp->setDefaultColor(custom_color);
		}


		Representation* RepresentationBuilder::createRepresentation(const RepresentationSpec& spec,
																																const std::list<Composite*>& composites,
																																MainControl& main_control,
																																Notifier& notifier,
																																const ModelInformation& model_information,
																																bool hidden)
		{
			// Mirrors the legacy createRepresentation (DIALOGS engine lines 566-611).
			if (composites.size() == 0) return 0;

			// workaround for MSVC: crashed otherwise
			Composite* first_composite = *composites.begin();
			if (first_composite == 0) return 0;

			// create a new Representation
			Representation* rep = new Representation();

			// The legacy recreate guards in applyModelSettings_/applyColoringSettings_
			// read modelUpdateEnabled()/coloringUpdateEnabled(); set them BEFORE the apply
			// helpers so the guards evaluate identically. A fresh Representation enables
			// both by default, so for a default spec this is a no-op (preserving legacy
			// behavior), but an explicit spec can carry the saved flags.
			rep->enableModelUpdate(spec.model_update_enabled);
			rep->enableColoringUpdate(spec.coloring_update_enabled);

			applyColoringSettings(*rep, spec, model_information);
			applyModelSettings(*rep, spec, model_information);

			// stupid, but must be: create a List with const Composites!
			std::list<const Composite*> temp_composites;
			std::list<Composite*>::const_iterator it = composites.begin();
			for (; it != composites.end(); it++)
			{
				temp_composites.push_back(*it);
			}
			rep->setComposites_(temp_composites);
			if (hidden) rep->setHidden_(true);

			// this is not straight forward, but we have to prevent a second rendering run
			// in the Scene... the insertion into the RepresentationManager is needed to
			// allow the Representation::update
			main_control.getRepresentationManager().insert(*rep, false);

			// now we can add the Representation to the GeometricControl
			notifier.notify(new RepresentationMessage(*rep, RepresentationMessage::ADD_TO_GEOMETRIC_CONTROL));

			// no refocus, if this is not the only Representation
			if (main_control.getRepresentationManager().getRepresentations().size() < 2)
			{
				notifier.notify(new CompositeMessage(*first_composite, CompositeMessage::CENTER_CAMERA));
			}

			if (hidden) return rep;

			rep->update(true);

			return rep;
		}


		bool RepresentationBuilder::parseDataString(const String& data_string,
																								ParsedRepresentation& out,
																								const ModelInformation& model_information)
		{
			// Reproduces the legacy getSettingsFromString (DIALOGS engine lines 809-833)
			// plus the field splitting of the legacy createRepresentation(String,...)
			// (DIALOGS engine lines 835-913) WITHOUT touching any widget. The full layout
			// (matching Representation::toString, representation.C:551-595) is:
			//
			//   Representation0=1;3 2 2 6.500000 0 0 [2]|Color|H\...
			//                    ^                                   System Number
			//                       ^                                Model Settings (6 space-separated fields)
			//                                            ^           Composites numbers
			//                                                ^       Custom Color
			//                                                    ^   Hidden Flag
			//                                                      ^ Named Properties
			try
			{
				String working = data_string;
				std::vector<String> string_vector;

				// split off information of system number
				working.split(string_vector, ";");
				if (string_vector.size() < 2)
				{
					return false;
				}

				out.system_index = string_vector[0].toUnsignedInt();

				// split off between representation settings and composite numbers
				String settings_and_rest = string_vector[1];
				std::vector<String> string_vector2;
				settings_and_rest.split(string_vector2, "[]");
				if (string_vector2.size() < 3)
				{
					return false;
				}

				// trailing "|H" hidden flag (legacy DIALOGS engine line 869)
				out.hidden = string_vector2[2].hasSubstring("|H");

				// Composite id's per number (the "[...]" block; legacy DIALOGS engine 873-880)
				String ids = string_vector2[1];
				ids.split(string_vector2, ",");
				out.composite_indices.clear();
				for (Position p = 0; p < string_vector2.size(); p++)
				{
					out.composite_indices.insert(string_vector2[p].toUnsignedInt());
				}

				if (out.composite_indices.size() == 0)
				{
					return false;
				}

				// custom color (the "|color|" block; legacy DIALOGS engine 894-902)
				String settings = string_vector[1];
				if (settings.has('|'))
				{
					std::vector<String> color_split;
					settings.split(color_split, "|");
					if (color_split.size() >= 2)
					{
						out.custom_color = ColorRGBA(color_split[1]);
						out.spec.custom_color = out.custom_color;
					}
				}

				// The 6-field settings parse (legacy getSettingsFromString, DIALOGS engine
				// 809-833) operates on the rep-settings token (string_vector[1] / "settings").
				std::vector<String> fields;
				if (settings.split(fields) < 6 ||
						!model_information.modelMuteableByDisplayProperties((ModelType)fields[0].toUnsignedInt()))
				{
					return false;
				}

				// field 0: model type, 1: drawing mode, 2: drawing precision,
				// 3: surface drawing precision, 4: coloring method, 5: transparency.
				out.spec.model_type        = (ModelType)fields[0].toUnsignedInt();
				out.spec.drawing_mode      = (DrawingMode)fields[1].toUnsignedInt();
				out.spec.drawing_precision = (DrawingPrecision)fields[2].toUnsignedInt();
				out.spec.surface_drawing_precision = fields[3].toFloat();
				out.spec.coloring_method   = (ColoringMethod)fields[4].toUnsignedInt();
				out.spec.transparency      = (Size)fields[5].toInt();

				// Raw, still-base64-encoded named-property blocks (legacy DIALOGS engine
				// 924-933): the '\\'-then-'*'-split sequence. NOT decoded here — Plan 03
				// replays them with TextPersistenceManager + notify_.
				out.named_property_blocks.clear();
				std::vector<String> properties;
				settings.split(properties, "\\");
				if (properties.size() > 1)
				{
					std::vector<String> blocks;
					properties[1].split(blocks, "*");
					for (Position i = 0; i < blocks.size(); ++i)
					{
						out.named_property_blocks.push_back(blocks[i]);
					}
				}

				return true;
			}
			catch (...)
			{
			}

			return false;
		}

		// ARCHITECTURE-CONTRACT.md §3c — owner-narrowing mutation forwarders.
		// RepresentationBuilder is the single builder friend of Representation, so
		// these are the only translation point that can reach the private setX_().
		// Each forwards verbatim to the matching private setter; behaviour is
		// unchanged — the narrowing is purely about who is allowed to call them.

		void RepresentationBuilder::setModelType(Representation& rep, ModelType type)
		{
			rep.setModelType_(type);
		}

		void RepresentationBuilder::setColoringMethod(Representation& rep, ColoringMethod method)
		{
			rep.setColoringMethod_(method);
		}

		void RepresentationBuilder::setDrawingMode(Representation& rep, DrawingMode mode)
		{
			rep.setDrawingMode_(mode);
		}

		void RepresentationBuilder::setDrawingPrecision(Representation& rep, DrawingPrecision precision)
		{
			rep.setDrawingPrecision_(precision);
		}

		void RepresentationBuilder::setSurfaceDrawingPrecision(Representation& rep, float precision)
		{
			rep.setSurfaceDrawingPrecision_(precision);
		}

		void RepresentationBuilder::setTransparency(Representation& rep, Size value)
		{
			rep.setTransparency_(value);
		}

		void RepresentationBuilder::setModelProcessor(Representation& rep, ModelProcessor* processor)
		{
			rep.setModelProcessor_(processor);
		}

		void RepresentationBuilder::setColorProcessor(Representation& rep, ColorProcessor* processor)
		{
			rep.setColorProcessor_(processor);
		}

		void RepresentationBuilder::setComposites(Representation& rep, const std::list<const Composite*>& composites)
		{
			rep.setComposites_(composites);
		}

		void RepresentationBuilder::setComposite(Representation& rep, const Composite* composite)
		{
			rep.setComposite_(composite);
		}

		void RepresentationBuilder::setHidden(Representation& rep, bool state)
		{
			rep.setHidden_(state);
		}

		void RepresentationBuilder::markColorProcessorChanged(Representation& rep)
		{
			// §3c friend access — flip the recolor gate so update(false) re-walks
			// the (unchanged-pointer) color processor. Used for value-range-only
			// coloring changes where no processor swap occurs (999.63).
			rep.changed_color_processor_ = true;
		}

	} // namespace VIEW
} // namespace BALL

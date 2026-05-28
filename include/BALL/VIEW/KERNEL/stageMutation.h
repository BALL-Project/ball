// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.59-04 (v1.7.4) — StageMutation owner-side facade.
//
// ARCHITECTURE-CONTRACT.md §3b. Stage is mutated by FIVE domains
// (background / fog / camera / lights / stereo). Granting five
// per-domain friends would make the setters "private but not narrow"
// (Codex MEDIUM). Instead Stage exposes a SINGLE friend — StageMutation —
// and that token is the only legal Stage-setter surface. Controllers and
// the legitimate owner-side callers mutate Stage by holding a
// StageMutation and chaining its typed mutators.

#ifndef BALL_VIEW_KERNEL_STAGEMUTATION_H
#define BALL_VIEW_KERNEL_STAGEMUTATION_H

#ifndef BALL_VIEW_KERNEL_STAGE_H
#	include <BALL/VIEW/KERNEL/stage.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
#	include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

#ifndef BALL_VIEW_RENDERING_CAMERA_H
#	include <BALL/VIEW/RENDERING/camera.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/** Owner-side mutation facade for Stage (ARCHITECTURE-CONTRACT.md §3b).

				StageMutation is the SINGLE friend of Stage and therefore the only
				translation point that can reach the private Stage setters. A caller
				constructs a StageMutation from a live Stage and chains the typed
				mutators; direct external @c stage.setBackgroundColor() no longer
				compiles. The chained mutators return @c *this so multiple fields can
				be set in one expression, mirroring the legacy multi-setter sequence:

				@code
				StageMutation(stage)
				  .backgroundColor(color)
				  .fogIntensity(fog)
				  .eyeDistance(eye)
				  .focalDistance(focal);
				@endcode

				\see Stage
				\ingroup ViewKernelStage
		*/
		class BALL_VIEW_EXPORT StageMutation
		{
			public:

			/** Construct a mutation token bound to @p s. */
			explicit StageMutation(Stage& s)
				: stage_(s)
			{
			}

			/** Set the stage background color. */
			StageMutation& backgroundColor(const ColorRGBA& color)
			{
				stage_.setBackgroundColor_(color);
				return *this;
			}

			/** Replace the stage camera. */
			StageMutation& camera(const Camera& camera)
			{
				stage_.setCamera_(camera);
				return *this;
			}

			/** Set the stereo eye distance. */
			StageMutation& eyeDistance(float value)
			{
				stage_.setEyeDistance_(value);
				return *this;
			}

			/** Set the stereo focal distance. */
			StageMutation& focalDistance(float value)
			{
				stage_.setFocalDistance_(value);
				return *this;
			}

			/** Set the fog intensity. */
			StageMutation& fogIntensity(float value)
			{
				stage_.setFogIntensity_(value);
				return *this;
			}

			private:

			//_ The stage this token mutates.
			Stage& stage_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_STAGEMUTATION_H

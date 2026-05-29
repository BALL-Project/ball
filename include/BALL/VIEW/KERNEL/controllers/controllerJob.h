// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.59-05 (v1.7.x-24) — Controller async-job base
// (ARCHITECTURE-CONTRACT.md §6, referenced by the §4 thread-safety model).
//
// ControllerJob is the minimal async scaffold that JUSTIFIES apply()'s
// "GUI-thread-only" contract: a Controller whose mutation needs long work
// does NOT run it inside apply() (which holds the nest-aware guard and
// touches the owner / GL context). Instead it kicks a ControllerJob, returns
// false from apply() (nothing mutated synchronously), and the job — when it
// finishes on its worker thread — re-enters the GUI thread (Qt::QueuedConnection
// / QMetaObject::invokeMethod) before the §2 apply() contract runs under the
// guard. NO concrete job ships in v1.7.4: this is the base + the queued
// re-entry plumbing only.
//

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_CONTROLLERJOB_H
#define BALL_VIEW_KERNEL_CONTROLLERS_CONTROLLERJOB_H

#ifndef BALL_COMMON_GLOBAL_H
#	include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_APPLYPAYLOAD_H
#	include <BALL/VIEW/KERNEL/controllers/applyPayload.h>
#endif

#include <QtCore/QObject>

namespace BALL
{
	namespace VIEW
	{

		/**
		 * Minimal async-job base for the command contract (§6).
		 *
		 * A ControllerJob runs long work OFF the GUI thread, then delivers its
		 * result back ON the GUI thread so the owning Controller can run the
		 * §2 apply() contract under the re-entrancy guard. The worker thread
		 * must NOT touch the owner or the GL context (the macOS UFG-26 class of
		 * crash); only the queued <tt>finished()</tt> result, handled on the GUI
		 * thread, mutates anything.
		 *
		 * <b>v1.7.4 scope.</b> This is the base + signals + queued re-entry
		 * plumbing only — NO concrete job ships. <tt>start()</tt> is a scaffold
		 * that emits <tt>started()</tt> and queues a <tt>finished()</tt> result
		 * via <tt>QMetaObject::invokeMethod</tt>, proving the worker→GUI hand-off
		 * path compiles and is exercised. A v2.0 concrete job overrides the work
		 * step and populates the ApplyPayload it emits.
		 */
		class BALL_VIEW_EXPORT ControllerJob
			: public QObject
		{
			Q_OBJECT

			public:

				/**
				 * Construct an (optionally parented) job. A real job is typically
				 * moved to a worker QThread by the caller before start().
				 * @param parent the owning QObject, or nullptr.
				 */
				explicit ControllerJob(QObject* parent = nullptr);

				/// Destructor.
				~ControllerJob() override;

				/**
				 * Kick the job. Emits started(), then (v1.7.4 scaffold) queues a
				 * finished() result back onto this object's thread via
				 * QMetaObject::invokeMethod — the same worker→GUI re-entry a real
				 * job uses. A v2.0 concrete job runs its work here and emits
				 * finished() with a populated ApplyPayload.
				 */
				void start();

			Q_SIGNALS:

				/// Emitted when the job begins.
				void started();

				/// Coarse progress in percent [0, 100].
				void progress(int pct);

				/**
				 * Emitted with the captured result when the job completes. The
				 * owning Controller connects this with Qt::QueuedConnection so the
				 * handler runs on the GUI thread (the §2 apply() contract).
				 * @param payload the captured before/after intent.
				 */
				void finished(const ApplyPayload& payload);
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CONTROLLERS_CONTROLLERJOB_H

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 — LegacySettingsHelper implementation.
//

#include <BALL/VIEW/KERNEL/legacySettingsHelper.h>

#include <QtCore/QCoreApplication>

namespace BALL
{
	namespace VIEW
	{

		QStringList LegacySettingsHelper::legacyStackNames()
		{
			// Inventory taken from `grep setWidgetStackName source/VIEW/
			// DIALOGS/*.C` at 999.44 plan-time (12 stacks). The 3
			// preferences-only stacks (General/Open-Save/Network/etc.)
			// are intentionally INCLUDED — `Tools › Legacy Settings`
			// duplicates ALL preferences stacks during the migration
			// window so users have a single fallback route until the
			// Inspector covers every setting.
			return QStringList()
				// Render / scene settings — replaced by Inspector Scene tab.
				<< QStringLiteral("Display")
				<< QStringLiteral("Lighting")
				// Representation settings — replaced by Inspector Representation tab.
				<< QStringLiteral("Models")
				<< QStringLiteral("Model Colors")
				<< QStringLiteral("Materials")
				// General / app-level (no Inspector equivalent — these stay
				// reachable via this menu indefinitely; the cut-over plan
				// decides which to promote).
				<< QStringLiteral("General")
				<< QStringLiteral("Open/Save")
				<< QStringLiteral("Editing")
				<< QStringLiteral("Shortcuts")
				<< QStringLiteral("Network")
				<< QStringLiteral("Client/Server")
				<< QStringLiteral("Python");
		}

		QString LegacySettingsHelper::displayName(const QString& stackName)
		{
			// Forward stack names verbatim — the design system uses
			// the legacy strings unchanged for the migration window.
			return QCoreApplication::translate("LegacySettingsHelper",
			                                   stackName.toUtf8().constData());
		}

		QString LegacySettingsHelper::migrationNoticeTitle()
		{
			return QCoreApplication::translate("LegacySettingsHelper",
				"BALLView settings have moved");
		}

		QString LegacySettingsHelper::migrationNoticeBody()
		{
			return QCoreApplication::translate("LegacySettingsHelper",
				"Display, Model, Material, Light, and other settings are now in "
				"the Inspector on the right. Old dialogs are still accessible "
				"under Tools › Legacy Settings.");
		}

	} // namespace VIEW
} // namespace BALL

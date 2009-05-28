set(path VIEW/KERNEL)


SET(MOC_SOURCES "")
SET(MOC_SOURCES 
	mainControl.h
	serverWidget.h
	shortcutRegistry.h
)

foreach(i ${MOC_SOURCES})
	wrap_MOC(VIEW ${path} ${i})
endforeach(i)


list(APPEND VIEW_SOURCES 
	VIEW/KERNEL/clippingPlane.C
	VIEW/KERNEL/common.C
	VIEW/KERNEL/compositeManager.C
	VIEW/KERNEL/connectionObject.C
	VIEW/KERNEL/geometricObject.C
	VIEW/KERNEL/mainControl.C
	VIEW/KERNEL/message.C
	VIEW/KERNEL/modelInformation.C
	VIEW/KERNEL/modularWidget.C
	VIEW/KERNEL/preferencesEntry.C
	VIEW/KERNEL/representation.C
	VIEW/KERNEL/representationManager.C
	VIEW/KERNEL/serverWidget.C
	VIEW/KERNEL/shortcutRegistry.C
	VIEW/KERNEL/stage.C
	VIEW/KERNEL/threads.C
)


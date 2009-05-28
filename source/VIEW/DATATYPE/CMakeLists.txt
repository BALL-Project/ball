set(path VIEW/DATATYPE)


SET(MOC_SOURCES "")
SET(MOC_SOURCES 
	dataset.h
	standardDatasets.h
)

foreach(i ${MOC_SOURCES})
	wrap_MOC(VIEW ${path} ${i})
endforeach(i)


list(APPEND VIEW_SOURCES 
	VIEW/DATATYPE/colorExtensions.C
	VIEW/DATATYPE/colorHSV.C
	VIEW/DATATYPE/colorMap.C
	VIEW/DATATYPE/colorRGBA.C
	VIEW/DATATYPE/colorUnit.C
	VIEW/DATATYPE/colorUnitHue.C
	VIEW/DATATYPE/dataset.C
	VIEW/DATATYPE/raytraceableGrid.C
	VIEW/DATATYPE/standardDatasets.C
	VIEW/DATATYPE/vertex1.C
	VIEW/DATATYPE/vertex2.C
)


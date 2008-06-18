DIRECTORY=APPLICATIONS/QSAR_GUI

EXECUTABLE=QSARTool
INC_PATH=../../../include/BALL/APPLICATIONS/QSAR_GUI/

all:	$(EXECUTABLE)
		
clean:	default_clean

include ../../common.mak
-include .Dependencies


####### Files

SOURCES       = dataItem.C\
		dataItemView.C \
		dataItemScene.C \
		edge.C \
		dottedEdge.C\
		main.C \
		mainWindow.C \
		moc_dataItemView.C \
		moc_dataItemScene.C \
		moc_mainWindow.C \
		inputDataItem.C \
		modelItem.C\
		validationItem.C\
		featureSelectionItem.C\
		SDFInputDialog.C\
		CSVInputDialog.C\
		moc_CSVInputDialog.C\
		fileBrowser.C\
		exception.C\
		modelConfigurationDialog.C\
		modelConfigurationDialogPages.C\
		featureSelectionDialog.C\
		validationDialog.C\
		moc_modelConfigurationDialog.C\
		moc_SDFInputDialog.C\
		moc_modelConfigurationDialogPages.C\
		moc_featureSelectionDialog.C\
		moc_validationDialog.C\
		predictionItem.C\
		predictionResultDialog.C\
		moc_predictionResultDialog.C\
		validationResultDialog.C\
		moc_validationResultDialog.C\
		inputDataDialog.C\
		moc_inputDataDialog.C\
		connectionManager.C\
		moc_connectionManager.C\
		CSVInputDataItem.C\
		SDFInputDataItem.C\
		plotter.C\
		moc_plotter.C\
		predictionPlotter.C\
		inputPlotter.C\
		moc_inputPlotter.C\
		moc_modelItem.C\
		inputDataItemIO.C\
		partitioningItem.C\
		inputPartitionItem.C\
		pipeline.C\
		coefficientPlotter.C\
		bayesPlotter
#	featurePlotter.C	
# 		connectionsList.C\
# 		moc_connectionsList.C
	

OBJECTS       = dataItem.o \
		dataItemView.o \
		dataItemScene.o \
		edge.o \
		dottedEdge.o\
		main.o \
		mainWindow.o \
		moc_dataItemView.o \
		moc_dataItemScene.o \
		moc_mainWindow.o \
		inputDataItem.o \
		modelItem.o\
		validationItem.o\
		featureSelectionItem.o\
		SDFInputDialog.o\
		CSVInputDialog.o\
		moc_CSVInputDialog.o\
		fileBrowser.o\
		exception.o\
		modelConfigurationDialog.o\
		modelConfigurationDialogPages.o\
		featureSelectionDialog.o\
		validationDialog.o\
		moc_modelConfigurationDialog.o\
		moc_SDFInputDialog.o\
		moc_modelConfigurationDialogPages.o\
		moc_featureSelectionDialog.o\
		moc_validationDialog.o\
		predictionItem.o\
		predictionResultDialog.o\
		moc_predictionResultDialog.o\
		validationResultDialog.o\
		moc_validationResultDialog.o\
		inputDataDialog.o\
		moc_inputDataDialog.o\
		connectionManager.o\
		moc_connectionManager.o\
		CSVInputDataItem.o\
		SDFInputDataItem.o\
		plotter.o\
		moc_plotter.o\
		predictionPlotter.o\
		inputPlotter.o\
		moc_inputPlotter.o\
		moc_modelItem.o\
		inputDataItemIO.o\
		partitioningItem.o\
		inputPartitionItem.o\
		pipeline.o\
		coefficientPlotter.o\
		bayesPlotter.o
#		featurePlotter.o
# 		connectionsList.o\
# 		moc_connectionsList.o


QSARTool:	$(OBJECTS)	$(SPECIAL_OBJECTS)
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(BALL_LIBS)
	
QSARTool-static:	$(OBJECTS)	$(SPECIAL_OBJECTS)
	$(CXX)	$(LDFLAGS) -o $@ $(OBJECTS) $(SPECIAL_OBJECTS) $(BALL_PATH)/lib/Linux-x86_64-g++_4.1.1/libBALL.a $(BALL_PATH)/lib/Linux-x86_64-g++_4.1.1/libVIEW.a $(BALL_PATH)/contrib/newmat/libnewmat.a $(BALL_PATH)/contrib/libsvm/libsvm.a $(BALL_PATH)/contrib/gsl-1.8/gsl-1.8/lib/libgsl.a  $(BALL_PATH)/contrib/gsl-1.8/gsl-1.8/lib/libgslcblas.a $(BALL_PATH)/contrib/qwt-5.0.2/lib/libqwt.a -L/share/opt/x86_64_sl4/qt-4.4.0-gcc_4.2.1/lib -lQtOpenGL -lQtGui -lQtCore -lQtTest -lQtSql -lGLU -lGL

depend: $(MOC_SOURCES)

moc_dataItemView.C: $(INC_PATH)dataItemView.h
	$(MOC) $(INC_PATH)dataItemView.h -o moc_dataItemView.C

moc_dataItemScene.C: $(INC_PATH)dataItemScene.h
	$(MOC) $(INC_PATH)dataItemScene.h -o moc_dataItemScene.C

moc_modelConfigurationDialogPages.C: $(INC_PATH)modelConfigurationDialogPages.h
	$(MOC) $(INC_PATH)modelConfigurationDialogPages.h -o moc_modelConfigurationDialogPages.C

moc_modelConfigurationDialog.C: $(INC_PATH)modelConfigurationDialog.h\
	$(INC_PATH)modelConfigurationDialogPages.h
	$(MOC) $(INC_PATH)modelConfigurationDialog.h -o moc_modelConfigurationDialog.C

moc_featureSelectionDialog.C: $(INC_PATH)featureSelectionDialog.h
	$(MOC) $(INC_PATH)featureSelectionDialog.h -o moc_featureSelectionDialog.C

moc_validationDialog.C: $(INC_PATH)validationDialog.h
	$(MOC) $(INC_PATH)validationDialog.h -o moc_validationDialog.C

moc_predictionResultDialog.C: $(INC_PATH)predictionResultDialog.h
	$(MOC) $(INC_PATH)predictionResultDialog.h -o moc_predictionResultDialog.C

moc_validationResultDialog.C: $(INC_PATH)validationResultDialog.h
	$(MOC) $(INC_PATH)validationResultDialog.h -o moc_validationResultDialog.C

moc_inputDataDialog.C: $(INC_PATH)inputDataDialog.h
	$(MOC) $(INC_PATH)inputDataDialog.h -o moc_inputDataDialog.C

moc_CSVInputDataDialog.C: $(INC_PATH)CSVInputDataDialog.h
	$(MOC) $(INC_PATH)CSVInputDataDialog.h -o moc_CSVInputDataDialog.C

moc_connectionManager.C: $(INC_PATH)connectionManager.h
	$(MOC) $(INC_PATH)connectionManager.h -o moc_connectionManager.C

moc_connectionsList.C: $(INC_PATH)connectionsList.h
	$(MOC) $(INC_PATH)connectionsList.h -o moc_connectionsList.C

moc_mainWindow.C: $(INC_PATH)dataItemView.h \
		$(INC_PATH)dataItemScene.h\
		$(INC_PATH)dataItem.h \
		$(INC_PATH)edge.h \
		$(INC_PATH)mainWindow.h \
		$(INC_PATH)inputDataItem.h \
		$(INC_PATH)modelItem.h\
		$(INC_PATH)featureSelectionItem.h

	$(MOC) $(INC_PATH)mainWindow.h -o moc_mainWindow.C

moc_SDFInputDialog.C: $(INC_PATH)SDFInputDialog.h
	$(MOC) $(INC_PATH)SDFInputDialog.h -o moc_SDFInputDialog.C
	
moc_CSVInputDialog.C: $(INC_PATH)CSVInputDialog.h
	$(MOC) $^ -o $@
		
moc_plotter.C: $(INC_PATH)plotter.h
	$(MOC) $^ -o $@

moc_modelItem.C: $(INC_PATH)modelItem.h
	$(MOC) $^ -o $@	

moc_inputPlotter.C: $(INC_PATH)inputPlotter.h
	$(MOC) $^ -o $@	



moc_inputPlotter.o: moc_inputPlotter.C 
	$(CXX) -c $(CXXFLAGS) $(BALL_INCLUDES) $^ -o $@

moc_modelItem.o: moc_modelItem.C 
	$(CXX) -c $(CXXFLAGS) $(BALL_INCLUDES) $^ -o $@

moc_dataItemView.o: moc_dataItemView.C 
	$(CXX) -c $(CXXFLAGS) $(BALL_INCLUDES) -o moc_dataItemView.o moc_dataItemView.C

moc_dataItemScene.o: moc_dataItemScene.C 
	$(CXX) -c $(CXXFLAGS) $(BALL_INCLUDES) -o moc_dataItemScene.o moc_dataItemScene.C

moc_mainWindow.o: moc_mainWindow.C 
	$(CXX) -c $(CXXFLAGS) $(BALL_INCLUDES) -o moc_mainWindow.o moc_mainWindow.C

moc_modelConfigurationDialog.o: moc_modelConfigurationDialog.C
	$(CXX) -c $(CXXFLAGS) $(BALL_INCLUDES) -o moc_modelConfigurationDialog.o moc_modelConfigurationDialog.C

moc_modelConfigurationDialogPages.o: moc_modelConfigurationDialogPages.C
	$(CXX) -c $(CXXFLAGS) $(BALL_INCLUDES) -o moc_modelConfigurationDialogPages.o moc_modelConfigurationDialogPages.C

moc_featureSelectionDialog.o: moc_featureSelectionDialog.C
	$(CXX) -c $(CXXFLAGS) $(BALL_INCLUDES) -o moc_featureSelectionDialog.o moc_featureSelectionDialog.C

moc_validationDialog.o: moc_validationDialog.C
	$(CXX) -c $(CXXFLAGS) $(BALL_INCLUDES) -o moc_validationDialog.o moc_validationDialog.C

moc_SDFInputDialog.o: moc_SDFInputDialog.C
	$(CXX) -c $(CXXFLAGS) $(BALL_INCLUDES) -o moc_SDFInputDialog.o moc_SDFInputDialog.C

moc_CSVInputDialog.o: moc_CSVInputDialog.C
	$(CXX) -c $(CXXFLAGS) $(BALL_INCLUDES) -o moc_CSVInputDialog.o moc_CSVInputDialog.C

moc_predictionResultDialog.o: moc_predictionResultDialog.C
	$(CXX) -c $(CXXFLAGS) $(BALL_INCLUDES) -o moc_predictionResultDialog.o moc_predictionResultDialog.C

moc_validationResultDialog.o: moc_validationResultDialog.C
	$(CXX) -c $(CXXFLAGS) $(BALL_INCLUDES) -o moc_validationResultDialog.o moc_validationResultDialog.C

moc_inputDataDialog.o: moc_inputDataDialog.C
	$(CXX) -c $(CXXFLAGS) $(BALL_INCLUDES) -o moc_inputDataDialog.o moc_inputDataDialog.C

moc_connectionManager.o: moc_connectionManager.C
	$(CXX) -c $(CXXFLAGS) $(BALL_INCLUDES) -o moc_connectionManager.o moc_connectionManager.C

moc_connectionsList.o: moc_connectionsList.C
	$(CXX) -c $(CXXFLAGS) $(BALL_INCLUDES) -o moc_connectionsList.o moc_connectionsList.C

moc_plotter.o: moc_plotter.C
	$(CXX) -c $(CXXFLAGS) $(BALL_INCLUDES) $^ -o $@


clean: 
	@$(RM) -f *.o *moc*

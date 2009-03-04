DIRECTORY=APPLICATIONS/QSAR_GUI

EXECUTABLE=QSARTool
INC_PATH=../../../include/BALL/APPLICATIONS/QSAR_GUI/

all:	$(EXECUTABLE)
		
clean:	default_clean

include ../../common.mak
-include .Dependencies


####### Files

SOURCES       = dataItem.C\
		moc_dataItem.C\
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
		moc_inputDataIntem.C\
		modelItem.C\
		validationItem.C\
		moc_validationItem.C\
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
		moc_predictionItem.C\
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
		bayesPlotter\
		featurePlotter.C\
		componentPlotter.C\
		moc_componentPlotter.C\
		moc_featurePlotter.C
# 		connectionsList.C\
# 		moc_connectionsList.C
	

OBJECTS       = dataItem.o \
		moc_dataItem.o\
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
		moc_inputDataItem.o\
		modelItem.o\
		validationItem.o\
		moc_validationItem.o\
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
		moc_predictionItem.o\
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
		bayesPlotter.o\
		featurePlotter.o\
		componentPlotter.o\
		moc_componentPlotter.o\
		moc_featurePlotter.o
# 		connectionsList.o\
# 		moc_connectionsList.o


QSARTool:	$(OBJECTS)	$(SPECIAL_OBJECTS)
	$(CXX)	$(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(SPECIAL_OBJECTS) $(BALL_LIBS)
	
QSARTool-static:	$(OBJECTS)	$(SPECIAL_OBJECTS)
	$(CXX)	$(LDFLAGS) -o $@ $(OBJECTS) $(SPECIAL_OBJECTS) $(BALL_PATH)/lib/libBALL.a $(BALL_PATH)/lib/libVIEW.a $(BALL_PATH)/contrib/libsvm/libsvm.a $(BALL_PATH)/contrib/gsl-1.8/gsl-1.8/lib/libgsl.a  $(BALL_PATH)/contrib/gsl-1.8/gsl-1.8/lib/libgslcblas.a $(BALL_PATH)/contrib/qwt-5.0.2/lib/libqwt.a -L/share/opt/x86_64_sl4/qt-4.4.0-gcc_4.2.1/lib -lQtOpenGL -lQtGui -lQtCore -lQtTest -lQtSql -lGLU -lGL /share/opt/x86_64_sl5/ATLAS-3.9.4/install/lib/liblapack.a /share/opt/x86_64_sl5/ATLAS-3.9.4/install/lib/libf77blas.a /share/opt/x86_64_sl5/ATLAS-3.9.4/install/lib/libcblas.a /share/opt/x86_64_sl5/ATLAS-3.9.4/install/lib/libatlas.a /share/opt/x86_64_sl4/g95-0.91/lib/gcc-lib/x86_64-suse-linux-gnu/4.0.3/libf95.a -lgfortran


QSARTool-static2:	$(OBJECTS)	$(SPECIAL_OBJECTS)
	$(CXX)	$(LDFLAGS) -o $@ $(OBJECTS) $(SPECIAL_OBJECTS) $(BALL_PATH)/lib/libBALL.a $(BALL_PATH)/lib/libVIEW.a $(BALL_PATH)/contrib/libsvm/libsvm.a $(BALL_PATH)/contrib/gsl-1.8/gsl-1.8/lib/libgsl.a  $(BALL_PATH)/contrib/gsl-1.8/gsl-1.8/lib/libgslcblas.a $(BALL_PATH)/contrib/qwt-5.0.2/lib/libqwt.a -L/media/sda6/qt-x11-opensource-src-4.4.0/lib -lQtOpenGL -lQtGui -lQtCore -lQtTest -lQtSql -lGLU -lGL /media/sda3/BALL-git/contrib/lapack-3.1.1/lapack_LINUX.a /media/sda3/BALL-git/contrib/lapack-3.1.1/blas_LINUX.a /media/sda3/BALL-git/contrib/lapack-3.1.1/tmglib_LINUX.a /usr/lib/gcc/i486-linux-gnu/4.3.2/libgfortran.a

	
# 	/media/sda6/clapack/lapack_LINUX.a  /media/sda6/clapack/blas_LINUX.a /media/sda6/clapack/F2CLIBS/libF77.a /media/sda6/clapack/F2CLIBS/libI77.a /media/sda7/BALL-git/contrib/gsl-1.8/gsl-1.8/lib/libgslcblas.a


depend: $(MOC_SOURCES)


moc_%.C:	$(INC_PATH)%.h
	$(MOC) $^ -o $@

moc_%.o: moc_%.C 
	$(CXX) -c $(CXXFLAGS) $(BALL_INCLUDES) $^ -o $@



clean: 
	@$(RM) -f *.o *moc*

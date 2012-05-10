### the directory name ###
SET(DIRECTORY source/QSAR)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	aromaticityProcessor.C
	automaticModelCreator.C
	bayesModel.C
	classificationModel.C
	classificationValidation.C
	configIO.C
	connectivityBase.C
	connectivityDescriptors.C
	descriptor.C
	exception.C
	kernel.C
	kernelModel.C
	latentVariableModel.C
	linearModel.C
	logitModel.C
	Model.C
	modelFactory.C
	nBModel.C
	nonlinearModel.C
	partialChargeBase.C
	partialChargeDescriptors.C
	partialChargeProcessor.C
	QSARData.C
	registry.C
	regressionModel.C
	regressionValidation.C
	ringPerceptionProcessor.C
	simpleBase.C
	simpleDescriptors.C
	snBModel.C
	statistics.C
	surfaceBase.C
	surfaceDescriptors.C
	svmModel.C
	svrModel.C
	validation.C
	allModel.C
	featureSelection.C
	gpModel.C
	knnModel.C
	kpcrModel.C
	kplsModel.C
	ldaModel.C
	mlrModel.C
	oplsModel.C
	pcrModel.C
	plsModel.C
	rrModel.C
)	

IF(BALL_HAS_GSL)
	LIST(APPEND SOURCES_LIST
		fitModel.C
	)
ENDIF()

IF (BALL_HAS_LIBSVM)
 SET(SOURCES_LIST ${SOURCES_LIST} libsvmModel.C)
ENDIF()

ADD_BALL_SOURCES("QSAR" "${SOURCES_LIST}")

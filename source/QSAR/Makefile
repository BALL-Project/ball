include ../config.mak

DIRECTORY=QSAR

THISLIB=$(LIBNAME)


CPP_SOURCES=\
	aromaticityProcessor.C\
	connectivityBase.C\
	connectivityDescriptors.C\
	descriptor.C\
	partialChargeBase.C\
	partialChargeDescriptors.C\
	partialChargeProcessor.C\
	ringPerceptionProcessor.C\
	simpleBase.C\
	simpleDescriptors.C\
	surfaceBase.C\
	surfaceDescriptors.C
	

ifeq ($(BALL_HAS_CBLAS),true)
	ifeq ($(BALL_HAS_GSL),true)
		CPP_SOURCES+=\
		configIO.C\
		classificationModel.C\
		classificationValidation.C\
		exception.C\
		statistics.C\
		featureSelection.C\
		allModel.C\
		gpModel.C\
		kernel.C\
		kernelModel.C\
		kpcrModel.C\
		kplsModel.C\
		ldaModel.C\
		linearModel.C\
		logitModel.C\
		mlrModel.C\
		Model.C\
		nonlinearModel.C\
		oplsModel.C\
		pcrModel.C\
		plsModel.C\
		QSARData.C\
		regressionModel.C\
		regressionValidation.C\
		sortedList.C\
		validation.C\
		modelFactory.C\
		registry.C\
		knnModel.C\
		rrModel.C\
		fitModel.C\
		bayesModel.C\
		snBModel.C\
		nBModel.C\
		latentVariableModel.C
		
		ifeq ($(BALL_HAS_LIBSVM),true)
			CPP_SOURCES+=\
			svmModel.C\
			svrModel.C\
			libsvmModel.C
		endif			
	endif
endif


include ../common.mak
-include .Dependencies

// Copyright (c) 1999, 2000 Phil Thompson (phil@river-bank.demon.co.uk)
//
// This is the SIP interface definition for QWidgetList.


%ExportedDoc
<Sect2><Title>QWidgetList</Title>
<Para>
This class isn't implemented.  Whenever a <Literal>QWidgetList</Literal> is the
return type of a function or the type of an argument, a Python list of
instances is used instead.
</Para>
</Sect2>
%End


class QWidgetList
{
%HeaderCode
#include <qwidgetlist.h>
#include "sipqtQWidget.h"
%End

%ConvertFromClassCode
	PyObject *pl;
	QWidget *obj;

	if ((pl = PyList_New(0)) == NULL)
		return NULL;

	// Convert the list.

	for (QWidgetListIt it(*sipCpp); (obj = it.current()) != NULL; ++it)
	{
		PyObject *inst;

		if ((inst = sipMapCppToSelf(obj,sipClass_QWidget)) == NULL || PyList_Append(pl,inst) < 0)
		{
			Py_DECREF(pl);
			return NULL;
		}
	}

	return pl;
%End
 
%CanConvertToClassCode
	return PyList_Check(sipPy);
%End
 
%ConvertToClassCode
	// Convert a Python list of QWidget instances to a QWidgetList on the
	// heap.
 
	QWidgetList *qwl = new QWidgetList;
 
	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
		QWidget *qw;
 
		qw = sipForceConvertTo_QWidget(PyList_GET_ITEM(sipPy,i),sipIsErr);
 
		if (*sipIsErr)
		{
			delete qwl;
			return 0;
		}
 
		qwl -> append(qw);
	}
 
	*sipCppPtr = qwl;

	return 1;
%End
};

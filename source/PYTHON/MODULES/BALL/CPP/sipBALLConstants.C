#include "sipBALLDeclBALL.h"
#include "sipBALLConstants.h"


PyObject *sipClass_Constants;

static void sipDealloc_Constants(sipThisType *);

static PyTypeObject sipType_Constants = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	sipName_BALL_Constants,
	sizeof (sipThisType),
	0,
	(destructor)sipDealloc_Constants,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	Py_TPFLAGS_DEFAULT,
	0,
	0,
	0,
};

// Cast a pointer to a type somewhere in its superclass hierachy.

extern "C" const void *sipCast_Constants(const void *ptr,PyObject *targetClass)
{
	if (targetClass == sipClass_Constants)
		return ptr;

	return NULL;
}

static void sipDealloc_Constants(sipThisType *sipThis)
{
	if (sipThis -> u.cppPtr != NULL)
	{
		if (sipIsPyOwned(sipThis))
			delete (Constants *)sipThis -> u.cppPtr;
	}

	sipDeleteThis(sipThis);
}

PyObject *sipNew_Constants(PyObject *sipSelf,PyObject *sipArgs)
{
	static sipExtraType et = {
		sipCast_Constants
	};

	sipThisType *sipThis = NULL;
	const void *sipNew = NULL;
	int sipFlags = SIP_PY_OWNED;
	int sipArgsParsed = 0;

	// See if there is something pending.

	sipNew = sipGetPending(&sipFlags);

	if (sipNew == NULL)
	{
		if (sipParseArgs(&sipArgsParsed,sipArgs,"-"))
		{
   try
   {
			sipNew = new Constants();
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}
		}
	}

	if (sipNew == NULL)
	{
		const Constants * a0;
		PyObject *a0obj;

		if (sipParseArgs(&sipArgsParsed,sipArgs,"-I",sipCanConvertTo_Constants,&a0obj))
		{
			int iserr = 0;

			sipConvertTo_Constants(a0obj,(Constants **)&a0,1,&iserr);

			if (iserr)
				return NULL;

   try
   {
			sipNew = new Constants(* a0);
   }
   catch (...)
    {
      PyErr_SetString(PyExc_Exception, "unknown");
      return NULL;
		}
		}
	}

	if (sipNew == NULL)
	{
		sipNoCtor(sipArgsParsed,sipName_BALL_Constants);
		return NULL;
	}

	// Wrap the object.

	if ((sipThis = sipCreateThis(sipSelf,sipNew,&sipType_Constants,sipFlags,&et)) == NULL)
	{
		if (sipFlags & SIP_PY_OWNED)
			delete (Constants *)sipNew;

		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_CAL_PER_JOULE(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> CAL_PER_JOULE;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_CAL_PER_JOULE);
		return NULL;
	}

	ptr -> CAL_PER_JOULE = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_JOULE_PER_CAL(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> JOULE_PER_CAL;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_JOULE_PER_CAL);
		return NULL;
	}

	ptr -> JOULE_PER_CAL = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_M_PER_FOOT(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> M_PER_FOOT;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_M_PER_FOOT);
		return NULL;
	}

	ptr -> M_PER_FOOT = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_MM_PER_INCH(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> MM_PER_INCH;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_MM_PER_INCH);
		return NULL;
	}

	ptr -> MM_PER_INCH = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_RAD_PER_DEG(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> RAD_PER_DEG;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_RAD_PER_DEG);
		return NULL;
	}

	ptr -> RAD_PER_DEG = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_DEG_PER_RAD(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> DEG_PER_RAD;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_DEG_PER_RAD);
		return NULL;
	}

	ptr -> DEG_PER_RAD = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_FINE_STRUCTURE_CONSTANT(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> FINE_STRUCTURE_CONSTANT;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_FINE_STRUCTURE_CONSTANT);
		return NULL;
	}

	ptr -> FINE_STRUCTURE_CONSTANT = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_GRAVITATIONAL_CONSTANT(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> GRAVITATIONAL_CONSTANT;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_GRAVITATIONAL_CONSTANT);
		return NULL;
	}

	ptr -> GRAVITATIONAL_CONSTANT = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_c(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> c;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_c);
		return NULL;
	}

	ptr -> c = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_SPEED_OF_LIGHT(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> SPEED_OF_LIGHT;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_SPEED_OF_LIGHT);
		return NULL;
	}

	ptr -> SPEED_OF_LIGHT = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_VACUUM_PERMEABILITY(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> VACUUM_PERMEABILITY;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_VACUUM_PERMEABILITY);
		return NULL;
	}

	ptr -> VACUUM_PERMEABILITY = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_VACUUM_PERMITTIVITY(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> VACUUM_PERMITTIVITY;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_VACUUM_PERMITTIVITY);
		return NULL;
	}

	ptr -> VACUUM_PERMITTIVITY = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_a0(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> a0;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_a0);
		return NULL;
	}

	ptr -> a0 = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_BOHR_RADIUS(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> BOHR_RADIUS;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_BOHR_RADIUS);
		return NULL;
	}

	ptr -> BOHR_RADIUS = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_F(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> F;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_F);
		return NULL;
	}

	ptr -> F = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_FARADAY(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> FARADAY;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_FARADAY);
		return NULL;
	}

	ptr -> FARADAY = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_R(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> R;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_R);
		return NULL;
	}

	ptr -> R = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_GAS_CONSTANT(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> GAS_CONSTANT;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_GAS_CONSTANT);
		return NULL;
	}

	ptr -> GAS_CONSTANT = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_h(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> h;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_h);
		return NULL;
	}

	ptr -> h = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_PLANCK(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> PLANCK;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_PLANCK);
		return NULL;
	}

	ptr -> PLANCK = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_k(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> k;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_k);
		return NULL;
	}

	ptr -> k = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_BOLTZMANN(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> BOLTZMANN;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_BOLTZMANN);
		return NULL;
	}

	ptr -> BOLTZMANN = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_MOL(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> MOL;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_MOL);
		return NULL;
	}

	ptr -> MOL = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_NA(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> NA;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_NA);
		return NULL;
	}

	ptr -> NA = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_AVOGADRO(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> AVOGADRO;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_AVOGADRO);
		return NULL;
	}

	ptr -> AVOGADRO = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_NEUTRON_MASS(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> NEUTRON_MASS;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_NEUTRON_MASS);
		return NULL;
	}

	ptr -> NEUTRON_MASS = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_PROTON_MASS(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> PROTON_MASS;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_PROTON_MASS);
		return NULL;
	}

	ptr -> PROTON_MASS = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_ELECTRON_MASS(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> ELECTRON_MASS;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_ELECTRON_MASS);
		return NULL;
	}

	ptr -> ELECTRON_MASS = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_e0(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> e0;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_e0);
		return NULL;
	}

	ptr -> e0 = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_ELEMENTARY_CHARGE(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> ELEMENTARY_CHARGE;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_ELEMENTARY_CHARGE);
		return NULL;
	}

	ptr -> ELEMENTARY_CHARGE = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_EPSILON(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> EPSILON;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_EPSILON);
		return NULL;
	}

	ptr -> EPSILON = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_E(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> E;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_E);
		return NULL;
	}

	ptr -> E = val;

	Py_INCREF(Py_None);
	return Py_None;
}

extern "C" PyObject *sipGetSetVar_Constants_PI(PyObject *sipThisObj,PyObject *valobj)
{
	double val;
	Constants *ptr;

	if ((ptr = (Constants *)sipGetCppPtr((sipThisType *)sipThisObj,sipClass_Constants)) == NULL)
		return NULL;

	if (valobj == NULL)
	{
		val = ptr -> PI;

		valobj = PyFloat_FromDouble(val);

		return valobj;
	}

	val = PyFloat_AsDouble(valobj);

	if (PyErr_Occurred() != NULL)
	{
		sipBadSetType(sipName_BALL_Constants,sipName_BALL_PI);
		return NULL;
	}

	ptr -> PI = val;

	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef sipClassAttrTab_Constants[] = {
	{NULL}
};

PyMethodDef sipClassVarTab_Constants[] = {
	{sipName_BALL_CAL_PER_JOULE, sipGetSetVar_Constants_CAL_PER_JOULE, 0, NULL},
	{sipName_BALL_JOULE_PER_CAL, sipGetSetVar_Constants_JOULE_PER_CAL, 0, NULL},
	{sipName_BALL_M_PER_FOOT, sipGetSetVar_Constants_M_PER_FOOT, 0, NULL},
	{sipName_BALL_MM_PER_INCH, sipGetSetVar_Constants_MM_PER_INCH, 0, NULL},
	{sipName_BALL_RAD_PER_DEG, sipGetSetVar_Constants_RAD_PER_DEG, 0, NULL},
	{sipName_BALL_DEG_PER_RAD, sipGetSetVar_Constants_DEG_PER_RAD, 0, NULL},
	{sipName_BALL_FINE_STRUCTURE_CONSTANT, sipGetSetVar_Constants_FINE_STRUCTURE_CONSTANT, 0, NULL},
	{sipName_BALL_GRAVITATIONAL_CONSTANT, sipGetSetVar_Constants_GRAVITATIONAL_CONSTANT, 0, NULL},
	{sipName_BALL_c, sipGetSetVar_Constants_c, 0, NULL},
	{sipName_BALL_SPEED_OF_LIGHT, sipGetSetVar_Constants_SPEED_OF_LIGHT, 0, NULL},
	{sipName_BALL_VACUUM_PERMEABILITY, sipGetSetVar_Constants_VACUUM_PERMEABILITY, 0, NULL},
	{sipName_BALL_VACUUM_PERMITTIVITY, sipGetSetVar_Constants_VACUUM_PERMITTIVITY, 0, NULL},
	{sipName_BALL_a0, sipGetSetVar_Constants_a0, 0, NULL},
	{sipName_BALL_BOHR_RADIUS, sipGetSetVar_Constants_BOHR_RADIUS, 0, NULL},
	{sipName_BALL_F, sipGetSetVar_Constants_F, 0, NULL},
	{sipName_BALL_FARADAY, sipGetSetVar_Constants_FARADAY, 0, NULL},
	{sipName_BALL_R, sipGetSetVar_Constants_R, 0, NULL},
	{sipName_BALL_GAS_CONSTANT, sipGetSetVar_Constants_GAS_CONSTANT, 0, NULL},
	{sipName_BALL_h, sipGetSetVar_Constants_h, 0, NULL},
	{sipName_BALL_PLANCK, sipGetSetVar_Constants_PLANCK, 0, NULL},
	{sipName_BALL_k, sipGetSetVar_Constants_k, 0, NULL},
	{sipName_BALL_BOLTZMANN, sipGetSetVar_Constants_BOLTZMANN, 0, NULL},
	{sipName_BALL_MOL, sipGetSetVar_Constants_MOL, 0, NULL},
	{sipName_BALL_NA, sipGetSetVar_Constants_NA, 0, NULL},
	{sipName_BALL_AVOGADRO, sipGetSetVar_Constants_AVOGADRO, 0, NULL},
	{sipName_BALL_NEUTRON_MASS, sipGetSetVar_Constants_NEUTRON_MASS, 0, NULL},
	{sipName_BALL_PROTON_MASS, sipGetSetVar_Constants_PROTON_MASS, 0, NULL},
	{sipName_BALL_ELECTRON_MASS, sipGetSetVar_Constants_ELECTRON_MASS, 0, NULL},
	{sipName_BALL_e0, sipGetSetVar_Constants_e0, 0, NULL},
	{sipName_BALL_ELEMENTARY_CHARGE, sipGetSetVar_Constants_ELEMENTARY_CHARGE, 0, NULL},
	{sipName_BALL_EPSILON, sipGetSetVar_Constants_EPSILON, 0, NULL},
	{sipName_BALL_E, sipGetSetVar_Constants_E, 0, NULL},
	{sipName_BALL_PI, sipGetSetVar_Constants_PI, 0, NULL},
	{NULL}
};

PyMethodDef *sipClassVarHierTab_Constants[] = {
	sipClassVarTab_Constants,
	NULL
};

int sipCanConvertTo_Constants(PyObject *sipPy)
{
	return sipIsSubClassInstance(sipPy,sipClass_Constants);
}

void sipConvertTo_Constants(PyObject *sipPy,Constants **sipCppPtr,int sipWillDeref,int *sipIsErr)
{
	if (*sipIsErr || sipPy == NULL)
		return;

	if (sipPy == Py_None)
	{
		sipCheckNone(sipWillDeref,sipIsErr,sipName_BALL_Constants);
		*sipCppPtr = NULL;

		return;
	}

	*sipCppPtr = (Constants *)sipConvertToCpp(sipPy,sipClass_Constants,sipIsErr);
}

Constants *sipForceConvertTo_Constants(PyObject *valobj,int *iserrp)
{
	if (*iserrp || valobj == NULL || valobj == Py_None)
		return NULL;

	if (sipCanConvertTo_Constants(valobj))
	{
		Constants *val;

		sipConvertTo_Constants(valobj,&val,0,iserrp);

		return val;
	}

	sipBadClass(sipName_BALL_Constants);

	*iserrp = 1;

	return NULL;
}

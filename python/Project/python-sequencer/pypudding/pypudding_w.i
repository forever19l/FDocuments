%module pypudding_w
%include <cstring.i>

%{
#include "InstantPudding_API.h"
extern IP_API_Reply IP_getJsonResultsObj(IP_UUTHandle uutHandle);
%}

%ignore IP_addBlobData;
%ignore IP_addBlobDataWithPolicy;

IP_API_Reply IP_getJsonResultsObj(IP_UUTHandle uutHandle);

%apply (char *STRING, int LENGTH) { (const char* name, size_t nameLength) };
%apply (char *STRING, int LENGTH) { (const char* lowerLimit, size_t lowerLimitLength) };
%apply (char *STRING, int LENGTH) { (const char* upperLimit, size_t upperLimitLength) };
%apply (char *STRING, int LENGTH) { (const char* units, size_t unitsLength) };
%apply (char *STRING, int LENGTH) { (const char* value, size_t valueLength) };
%apply (char *STRING, int LENGTH) { (const char* message, size_t messageLength ) };


%typemap(in, numinputs=0) IP_UUTHandle* outHandle (IP_UUTHandle UID) {
    $1=&UID;
}

%typemap(argout) IP_UUTHandle* outHandle {
    PyObject *o, *o2, *o3;
    o=SWIG_NewPointerObj(*$1, $descriptor(IP_UUTHandle), SWIG_POINTER_EXCEPTION);
    if ((!$result) || ($result == Py_None)) {
        $result = o;
    } else {
        if (!PyTuple_Check($result)) {
	    o2 = $result;
	    $result = PyTuple_New(1);
	    PyTuple_SetItem($result,0,o2);
	    //PyTupel_SetItem takes over the responsiblity of O2 and automatically DECREF o2's reference count to exactly 1, so Py_DECREF on O2 must not be called
	}
	o3 = PyTuple_New(1);
	PyTuple_SetItem(o3,0,o);
	o2 = $result;
	$result = PySequence_Concat(o2,o3);
	Py_XDECREF(o2);
	Py_XDECREF(o3);
    } 
}


//input must be a floating value representing seconds
//from the epoch, the value returned by time.time
%typemap(in) time_t rawTimeToUse {
	$1 = PyFloat_AsDouble($input);
}

%typemap(in, numinputs=0, noblock=1) (char** cppValue, size_t *sLength)(char* pInfo, size_t length) {
    length=0;
    pInfo = 0;
    $1=NULL;
    $2 = &length;

}
%typemap(argout)(enum IP_ENUM_GHSTATIONINFO eGHStationInfo, char** cppValue, size_t *sLength) {
    PyObject *o, *o2, *o3;
    if ( !IP_success( result ) )
    {
      o= PyTuple_New(2);
      PyTuple_SetItem(o, 0, $result);
      PyTuple_SetItem(o, 1, Py_None);
      return o;
    } 
    Py_XDECREF($result);
    IP_reply_destroy(result);

    char* info = (char*)malloc(*$3+1);
    IP_API_Reply result2 = IP_getGHStationInfo(arg1,$1,&info,$3);
    o = SWIG_NewPointerObj(SWIG_as_voidptr(result2), SWIGTYPE_p_void, 0 |  0 );
    o2 = PyString_FromStringAndSize(info, *$3);
    free(info);
    
    $result = PyTuple_New(2);
    PyTuple_SetItem($result, 0, o);
    PyTuple_SetItem($result, 1, o2);
    
}
%include "InstantPudding_API.h"

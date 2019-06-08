//============================================================================
// Name        : test.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Testing calling matplolib from c++ using the python api
//============================================================================

// 

#include <iostream>
#include <python3.6m\Python.h>
#include <vector>
#include <stdexcept>
using namespace std;

PyObject* vectorToList_Float(const vector<float> &data) {
  PyObject* listObj = PyList_New( data.size() );
	if (!listObj) throw logic_error("Unable to allocate memory for Python list");
	for (unsigned int i = 0; i < data.size(); i++) {
		PyObject *num = PyFloat_FromDouble( (double) data[i]);
		if (!num) {
			Py_DECREF(listObj);
			throw logic_error("Unable to allocate memory for Python list");
		}
		PyList_SET_ITEM(listObj, i, num);
	}
	return listObj;
}

const char *text = "import matplotlib\n"
		"matplotlib.use('Agg')\n"
		"import matplotlib.pyplot as plt\n"
		"import numpy as np\n"
		"import sys, os\n"
		"sys.path.insert(0, '/cygdrive/c/Users/moham/eclipse-workspace/test/src')\n"
		"import plotRewards\n"
		"plt.violinplot(dataset=np.random.randn(200,10) + np.random.randn(10))\n"
	    "plt.xlabel('Action')\n"
	    "plt.ylabel('Reward distribution')\n"
	    "plt.savefig('/cygdrive/c/Users/moham/eclipse-workspace/test/images/figure_2_1.png')\n"
		"plt.clf()";

int main()
{
Py_Initialize();
PyRun_SimpleString("from time import time,ctime\n"
                   "print('Today is',ctime(time()))\n");
PyRun_SimpleString(text);
PyRun_SimpleString("print('Hello Python C/API')");
PyObject *pName = PyUnicode_DecodeFSDefault("plotRewards");
PyObject *pModule = PyImport_Import(pName);
PyObject *pArgs, *pValue;
Py_DECREF(pName);
if (pModule!= NULL) {
	PyObject *pFunc = PyObject_GetAttrString(pModule, "plot_rewards");
	if (pFunc && PyCallable_Check(pFunc)) {
		pArgs = PyTuple_New(1);
		static const float arr[] = {16.1,2.2,77.1,29.6};
		vector<float> vec (arr, arr + sizeof(arr) / sizeof(arr[0]) );
		PyObject * pythonList = vectorToList_Float(vec);
		PyTuple_SetItem(pArgs, 0, pythonList);
		pValue = PyObject_CallObject(pFunc, pArgs);
		Py_DECREF(pArgs);
	}
}
Py_Finalize();
return 0;
}

/*
 * Plot.cpp
 *
 *  Created on: Jun 23, 2019
 *      Author: moham
 */

#include "Plot.h"
#include "Bandit.h"
#include <vector>
#include <iostream>
using namespace std;

// Convert a 2D xarray to a 2D python list
PyObject* xtensorToList_Double(const xt::xarray<double> &data, int row, int column) {
	PyObject* listRow = PyList_New(row);
	for (int i=0; i<row;i++) {
		PyObject* listCol = PyList_New(column);
		for (int j = 0; j < column; j++) {
			PyObject *num = PyFloat_FromDouble( (double) data(i, j));
			if (!num) {
				Py_DECREF(listCol);
				throw logic_error("Unable to allocate memory for Python list");
			}
			PyList_SET_ITEM(listCol, j, num);
		}
		PyList_SET_ITEM(listRow, i, listCol);
	}
	return listRow;
}

// Convert a 1D xarray to a 1D python list
PyObject* xtensorToList(const xt::xarray<double> &data, int size) {
	PyObject* listObj = PyList_New(size);
	if (!listObj) throw logic_error("Unable to allocate memory for Python list");
	for (unsigned int i = 0; i < size; i++) {
		PyObject *num = PyFloat_FromDouble( (double) data(i));
		if (!num) {
			Py_DECREF(listObj);
			throw logic_error("Unable to allocate memory for Python list");
		}
		PyList_SET_ITEM(listObj, i, num);
	}
	return listObj;
}

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

PyObject* arrayToList_Float(float data[], int num) {
	PyObject* listObj = PyList_New(num);
	if (!listObj) throw logic_error("Unable to allocate memory for Python list");
	for (unsigned int i = 0; i < num; i++) {
		PyObject *num = PyFloat_FromDouble( (double) data[i]);
		if (!num) {
			Py_DECREF(listObj);
			throw logic_error("Unable to allocate memory for Python list");
		}
		PyList_SET_ITEM(listObj, i, num);
	}
	return listObj;
}

Plot::Plot() {
	// TODO Auto-generated constructor stub
	std::cout << "Initializing python interpreter" << std::endl;
	Py_Initialize();
	const char *text = "import matplotlib\n"
						"matplotlib.use('Agg')\n"
						"import matplotlib.pyplot as plt\n"
						"import numpy as np\n"
						"import sys, os\n"
						"sys.path.insert(0, os.environ['WORKING_DIR'] + '/src')\n"
						"import plotRewards\n";
	PyRun_SimpleString(text);
}

Plot::~Plot() {
	// TODO Auto-generated destructor stub
	std::cout << "Call Py_Finalize" << std::endl;
	Py_Finalize();
}

void Plot::figure_2_1() {
	const char *text = 	"plt.violinplot(dataset=np.random.randn(200,10) + np.random.randn(10))\n"
						"plt.xlabel('Action')\n"
						"plt.ylabel('Reward distribution')\n"
						"plt.savefig(os.environ['WORKING_DIR'] + '/images/figure_2_1.png')\n"
						"plt.clf()";
	PyRun_SimpleString(text);
}

void Plot::figure_2_2(retSimulate retsimulate, float epsilons[], int rows, int time) {
	PyObject * pythonListOptimal = xtensorToList_Double(retsimulate.mean_best_action_counts, rows, time);
	PyObject * pythonListRewards = xtensorToList_Double(retsimulate.mean_rewards, rows, time);
	PyObject * pythonListEpsilon = arrayToList_Float(epsilons, rows);
	//xt::xarray<double> a =  xt::zeros<double>({5,5});
	//PyObject * pythonList = xtensorToList_Double(a, 5, 5);
	PyObject *pName = PyUnicode_DecodeFSDefault("plotRewards");
	PyObject *pModule = PyImport_Import(pName);
	PyObject *pArgs, *pValue;
	Py_DECREF(pName);
	if (pModule!= NULL) {
		PyObject *pFunc = PyObject_GetAttrString(pModule, "plot_figure_2_2");
		if (pFunc && PyCallable_Check(pFunc)) {
			pArgs = PyTuple_New(3);
			PyTuple_SetItem(pArgs, 0, pythonListRewards);
			PyTuple_SetItem(pArgs, 1, pythonListOptimal);
			PyTuple_SetItem(pArgs, 2, pythonListEpsilon);
			pValue = PyObject_CallObject(pFunc, pArgs);
			Py_DECREF(pArgs);
			Py_DECREF(pFunc);
			Py_DECREF(pModule);
			Py_DECREF(pValue);
		}
	}
}

void Plot::figure_2_3(retSimulate retsimulate, int rows, int time) {
	PyObject * pythonListOptimal = xtensorToList_Double(retsimulate.mean_best_action_counts, rows, time);
	PyObject *pName = PyUnicode_DecodeFSDefault("plotRewards");
	PyObject *pModule = PyImport_Import(pName);
	PyObject *pArgs, *pValue;
	Py_DECREF(pName);
	if (pModule!= NULL) {
		PyObject *pFunc = PyObject_GetAttrString(pModule, "plot_figure_2_3");
		if (pFunc && PyCallable_Check(pFunc)) {
			pArgs = PyTuple_New(1);
			PyTuple_SetItem(pArgs, 0, pythonListOptimal);
			pValue = PyObject_CallObject(pFunc, pArgs);
			Py_DECREF(pArgs);
			Py_DECREF(pFunc);
			Py_DECREF(pModule);
			Py_DECREF(pValue);
		}
	}
}

void Plot::figure_2_4(retSimulate retsimulate, int rows, int time) {
	PyObject * pythonListRewards = xtensorToList_Double(retsimulate.mean_rewards, rows, time);
	PyObject *pName = PyUnicode_DecodeFSDefault("plotRewards");
	PyObject *pModule = PyImport_Import(pName);
	PyObject *pArgs, *pValue;
	Py_DECREF(pName);
	if (pModule!= NULL) {
		PyObject *pFunc = PyObject_GetAttrString(pModule, "plot_figure_2_4");
		if (pFunc && PyCallable_Check(pFunc)) {
			pArgs = PyTuple_New(1);
			PyTuple_SetItem(pArgs, 0, pythonListRewards);
			pValue = PyObject_CallObject(pFunc, pArgs);
			Py_DECREF(pArgs);
			Py_DECREF(pFunc);
			Py_DECREF(pModule);
			Py_DECREF(pValue);
		}
	}
}

void Plot::figure_2_5(retSimulate retsimulate, int rows, int time) {
	PyObject * pythonListRewards = xtensorToList_Double(retsimulate.mean_best_action_counts, rows, time);
	PyObject *pName = PyUnicode_DecodeFSDefault("plotRewards");
	PyObject *pModule = PyImport_Import(pName);
	PyObject *pArgs, *pValue;
	Py_DECREF(pName);
	if (pModule!= NULL) {
		PyObject *pFunc = PyObject_GetAttrString(pModule, "plot_figure_2_5");
		if (pFunc && PyCallable_Check(pFunc)) {
			pArgs = PyTuple_New(1);
			PyTuple_SetItem(pArgs, 0, pythonListRewards);
			pValue = PyObject_CallObject(pFunc, pArgs);
			Py_DECREF(pArgs);
			Py_DECREF(pFunc);
			Py_DECREF(pModule);
			Py_DECREF(pValue);
		}
	}
}

void Plot::figure_2_6(xt::xarray<double> rewards, xt::xarray<double> parameters, int size) {
	PyObject * pythonListRewards = xtensorToList(rewards, size);
	PyObject * pythonListParameters = xtensorToList_Double(parameters, 4, 7);
	PyObject *pName = PyUnicode_DecodeFSDefault("plotRewards");
	PyObject *pModule = PyImport_Import(pName);
	PyObject *pArgs, *pValue;
	Py_DECREF(pName);
	if (pModule!= NULL) {
		PyObject *pFunc = PyObject_GetAttrString(pModule, "plot_figure_2_6");
		if (pFunc && PyCallable_Check(pFunc)) {
			pArgs = PyTuple_New(2);
			PyTuple_SetItem(pArgs, 0, pythonListRewards);
			PyTuple_SetItem(pArgs, 1, pythonListParameters);
			pValue = PyObject_CallObject(pFunc, pArgs);
			Py_DECREF(pArgs);
			Py_DECREF(pFunc);
			Py_DECREF(pModule);
			Py_DECREF(pValue);
		}
	}
}

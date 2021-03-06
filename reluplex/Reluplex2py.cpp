#include <Python.h>
#include <vector>
#include <NeuralReluplex.h>
#include <Reluplex.h>
#include <numpy/ndarraytypes.h>
#include <numpy/ufuncobject.h>
#include <numpy/npy_3kcompat.h>

PyObject* construct(PyObject* self, PyObject* args) {
        NeuralReluplex* neuralReluplex = new NeuralReluplex();

        PyObject* neuralReluplexCapsule = PyCapsule_New((void *)neuralReluplex, "NeuralReluplexPtr", NULL);
        PyCapsule_SetPointer(neuralReluplexCapsule, (void *)neuralReluplex);

        return Py_BuildValue("O", neuralReluplexCapsule);
}
//
// PyObject* initializeCell(PyObject* self, PyObject* args) {
//         PyObject* neuralReluplexCapsule_;
//         unsigned int row_;
//         unsigned int column_;
//         double value_;
//
//         PyArg_ParseTuple(args, "OIId",
//                          &neuralReluplexCapsule_,
//                          &row_,
//                          &column_,
//                          &value_);
//
//         NeuralReluplex* neuralReluplex = (NeuralReluplex*)PyCapsule_GetPointer(neuralReluplexCapsule_, "NeuralReluplexPtr");
//
//         neuralReluplex->initializeCell(row_, column_, value_);
//
//         Py_RETURN_NONE;
// }
//
// PyObject* markBasic(PyObject* self, PyObject* args) {
//         PyObject* neuralReluplexCapsule_;
//         unsigned int variable_;
//
//         PyArg_ParseTuple(args, "OI",
//                          &neuralReluplexCapsule_,
//                          &variable_);
//
//         NeuralReluplex* neuralReluplex = (NeuralReluplex*)PyCapsule_GetPointer(neuralReluplexCapsule_, "NeuralReluplexPtr");
//
//         neuralReluplex->markBasic(variable_);
//
//         Py_RETURN_NONE;
// }
//
// PyObject* setLowerBound(PyObject* self, PyObject* args) {
//         PyObject* neuralReluplexCapsule_;
//         unsigned int variable_;
//         double bound_;
//
//         PyArg_ParseTuple(args, "OId",
//                          &neuralReluplexCapsule_,
//                          &variable_,
//                          &bound_);
//
//         NeuralReluplex* neuralReluplex = (NeuralReluplex*)PyCapsule_GetPointer(neuralReluplexCapsule_, "NeuralReluplexPtr");
//
//         neuralReluplex->setLowerBound(variable_, bound_);
//
//         Py_RETURN_NONE;
// }
//
// PyObject* setUpperBound(PyObject* self, PyObject* args) {
//         PyObject* neuralReluplexCapsule_;
//         unsigned int variable_;
//         double bound_;
//
//         PyArg_ParseTuple(args, "OId",
//                          &neuralReluplexCapsule_,
//                          &variable_,
//                          &bound_);
//
//         NeuralReluplex* neuralReluplex = (NeuralReluplex*)PyCapsule_GetPointer(neuralReluplexCapsule_, "NeuralReluplexPtr");
//
//         neuralReluplex->setUpperBound(variable_, bound_);
//
//         Py_RETURN_NONE;
// }
//
// PyObject* setReluPair(PyObject* self, PyObject* args) {
//         PyObject* neuralReluplexCapsule_;
//         unsigned int backward_;
//         unsigned int forward_;
//
//         PyArg_ParseTuple(args, "OII",
//                          &neuralReluplexCapsule_,
//                          &backward_,
//                          &forward_);
//
//         NeuralReluplex* neuralReluplex = (NeuralReluplex*)PyCapsule_GetPointer(neuralReluplexCapsule_, "NeuralReluplexPtr");
//
//         neuralReluplex->setReluPair(backward_, forward_);
//
//         Py_RETURN_NONE;
// }
//
// PyObject* setName(PyObject* self, PyObject* args) {
//         PyObject* neuralReluplexCapsule_;
//         unsigned int variable_;
//         const char* name_;
//
//         PyArg_ParseTuple(args, "OIs",
//                          &neuralReluplexCapsule_,
//                          &variable_,
//                          &name_);
//
//         NeuralReluplex* neuralReluplex = (NeuralReluplex*)PyCapsule_GetPointer(neuralReluplexCapsule_, "NeuralReluplexPtr");
//
//         neuralReluplex->setName(variable_, name_);
//
//         Py_RETURN_NONE;
// }
//
// PyObject* setLogging(PyObject* self, PyObject* args) {
//         PyObject* neuralReluplexCapsule_;
//         int flag_;
//
//         PyArg_ParseTuple(args, "Op",
//                          &neuralReluplexCapsule_,
//                          &flag_);
//
//         NeuralReluplex* neuralReluplex = (NeuralReluplex*)PyCapsule_GetPointer(neuralReluplexCapsule_, "NeuralReluplexPtr");
//
//         neuralReluplex->reluplex->setLogging(flag_);
//
//         Py_RETURN_NONE;
// }
//
// PyObject* setDumpStates(PyObject* self, PyObject* args) {
//         PyObject* neuralReluplexCapsule_;
//         int flag_;
//
//         PyArg_ParseTuple(args, "Op",
//                          &neuralReluplexCapsule_,
//                          &flag_);
//
//         NeuralReluplex* neuralReluplex = (NeuralReluplex*)PyCapsule_GetPointer(neuralReluplexCapsule_, "NeuralReluplexPtr");
//
//         neuralReluplex->reluplex->setDumpStates(flag_);
//
//         Py_RETURN_NONE;
// }
//
// PyObject* toggleAlmostBrokenReluEliminiation(PyObject* self, PyObject* args) {
//         PyObject* neuralReluplexCapsule_;
//         int flag_;
//
//         PyArg_ParseTuple(args, "Op",
//                          &neuralReluplexCapsule_,
//                          &flag_);
//
//         NeuralReluplex* neuralReluplex = (NeuralReluplex*)PyCapsule_GetPointer(neuralReluplexCapsule_, "NeuralReluplexPtr");
//
//         neuralReluplex->reluplex->toggleAlmostBrokenReluEliminiation(flag_);
//
//         Py_RETURN_NONE;
// }

// PyObject* getAssignment(PyObject* self, PyObject* args) {
//         PyObject* neuralReluplexCapsule_;
//         unsigned int variable_;
// 
//         PyArg_ParseTuple(args, "OI",
//                          &neuralReluplexCapsule_,
//                          &variable_);
// 
//         NeuralReluplex* neuralReluplex = (NeuralReluplex*)PyCapsule_GetPointer(neuralReluplexCapsule_, "NeuralReluplexPtr");
// 
//         double assignment = neuralReluplex->reluplex->getAssignment(variable_);
// 
//         return Py_BuildValue("d", assignment);
// }

PyObject* getInputAssignment(PyObject* self, PyObject* args) {
        PyObject* neuralReluplexCapsule_;

        PyArg_ParseTuple(args, "O",
                         &neuralReluplexCapsule_);

        NeuralReluplex* neuralReluplex = (NeuralReluplex*)PyCapsule_GetPointer(neuralReluplexCapsule_, "NeuralReluplexPtr");

        std::vector<double> assignment = neuralReluplex->getInputAssignment();

        int nd = 1;
        npy_intp dims[] = {(long)assignment.size()};

        PyObject* np_assignment = PyArray_SimpleNew(nd, dims, NPY_DOUBLE);

        for(auto i = 0ul; i < assignment.size(); ++i) {
                auto ptr = (npy_double*)PyArray_GETPTR1(np_assignment,i);
                *ptr = assignment[i];
        }

        return np_assignment;
}

PyObject* inputFromIntervalBox(PyObject* self, PyObject* args) {
        PyObject* neuralReluplexCapsule_;
        PyObject *arg_=NULL;
        PyArrayObject *arr_=NULL;

        PyArg_ParseTuple(args, "OO", &neuralReluplexCapsule_, &arg_);

        // // arr_ = PyArray_FROM_OTF(arg_, NPY_DOUBLE, NPY_IN_ARRAY);
        NeuralReluplex* neuralReluplex = (NeuralReluplex*)PyCapsule_GetPointer(neuralReluplexCapsule_, "NeuralReluplexPtr");
        arr_ = (PyArrayObject*) arg_;

        npy_intp *sp=PyArray_SHAPE(arr_);

        std::vector<double> l_bound;
        std::vector<double> u_bound;
        for (int i=0; i<*sp; i++) {
                l_bound.push_back(*((npy_double*)PyArray_GETPTR2(arr_,i, 0)));
                u_bound.push_back(*((npy_double*)PyArray_GETPTR2(arr_,i, 1)));
        }

        neuralReluplex->input_interval_box(l_bound, u_bound);

        Py_RETURN_NONE;
}

PyObject* inputNotFromIntervalBox(PyObject* self, PyObject* args) {
        PyObject* neuralReluplexCapsule_;
        PyObject *arg_=NULL;
        PyArrayObject *arr_=NULL;

        PyArg_ParseTuple(args, "OO", &neuralReluplexCapsule_, &arg_);

        // // arr_ = PyArray_FROM_OTF(arg_, NPY_DOUBLE, NPY_IN_ARRAY);
        NeuralReluplex* neuralReluplex = (NeuralReluplex*)PyCapsule_GetPointer(neuralReluplexCapsule_, "NeuralReluplexPtr");
        arr_ = (PyArrayObject*) arg_;

        npy_intp *sp=PyArray_SHAPE(arr_);

        std::vector<double> l_bound;
        std::vector<double> u_bound;
        for (int i=0; i<*sp; i++) {
                l_bound.push_back(*((npy_double*)PyArray_GETPTR2(arr_,i, 0)));
                u_bound.push_back(*((npy_double*)PyArray_GETPTR2(arr_,i, 1)));
        }

        neuralReluplex->input_neg_interval_box(l_bound, u_bound);

        Py_RETURN_NONE;
}

PyObject* i_does_not_win(PyObject* self, PyObject* args) {
        PyObject* neuralReluplexCapsule_;
        unsigned i;

        PyArg_ParseTuple(args, "OI", &neuralReluplexCapsule_, &i);

        NeuralReluplex* neuralReluplex = (NeuralReluplex*)PyCapsule_GetPointer(neuralReluplexCapsule_, "NeuralReluplexPtr");

        neuralReluplex->i_output_does_not_win(i);

        Py_RETURN_NONE;
}

PyObject* getOutputAssignment(PyObject* self, PyObject* args) {
        PyObject* neuralReluplexCapsule_;

        PyArg_ParseTuple(args, "O",
                         &neuralReluplexCapsule_);

        NeuralReluplex* neuralReluplex = (NeuralReluplex*)PyCapsule_GetPointer(neuralReluplexCapsule_, "NeuralReluplexPtr");

        std::vector<double> assignment = neuralReluplex->getOutputAssignment();

        int nd = 1;
        npy_intp dims[] = {(long)assignment.size()};

        PyObject* np_assignment = PyArray_SimpleNew(nd, dims, NPY_DOUBLE);

        for(auto i = 0ul; i < assignment.size(); ++i) {
                auto ptr = (npy_double*)PyArray_GETPTR1(np_assignment,i);
                *ptr = assignment[i];
        }

        return np_assignment;
}

PyObject* solve(PyObject* self, PyObject* args) {
        PyObject* neuralReluplexCapsule_;

        PyArg_ParseTuple(args, "O",
                         &neuralReluplexCapsule_);

        NeuralReluplex* neuralReluplex = (NeuralReluplex*)PyCapsule_GetPointer(neuralReluplexCapsule_, "NeuralReluplexPtr");

        neuralReluplex->build_reluplex();
        Reluplex::FinalStatus result = neuralReluplex->solve();

        if ( result == Reluplex::SAT )
                return Py_BuildValue("I", 0);
        else if ( result == Reluplex::UNSAT )
                return Py_BuildValue("I", 1);
        else if ( result == Reluplex::ERROR )
                return Py_BuildValue("I", 2);
        else
                return Py_BuildValue("I", 3);
}

PyObject* delete_object(PyObject* self, PyObject* args) {
        PyObject* neuralReluplexCapsule_;

        PyArg_ParseTuple(args, "O",
                         &neuralReluplexCapsule_);

        NeuralReluplex* neuralReluplex = (NeuralReluplex*)PyCapsule_GetPointer(neuralReluplexCapsule_, "NeuralReluplexPtr");

        delete neuralReluplex;

        Py_RETURN_NONE;
}

PyObject* build_network(PyObject* self, PyObject* args) {
        PyObject* neuralReluplexCapsule_;
        PyObject* list_;
        PyArrayObject* arr;

        PyArg_ParseTuple(args, "OO", &neuralReluplexCapsule_, &list_);

        std::vector<std::vector<std::vector<double> > > weights;


        Py_ssize_t size = PyList_GET_SIZE(list_);

        for(Py_ssize_t i = 0; i < size; ++i) {
                std::vector<std::vector<double> > layer_weight;
                arr = (PyArrayObject*)PySequence_GetItem(list_, i);
                int nd=PyArray_NDIM(arr);
                if (nd == 1) {
                        npy_intp *sp=PyArray_SHAPE(arr);
                        std::vector<double> layer_weight_i;
                        for (int i = 0; i < *sp; i++) {
                                layer_weight_i.push_back(*((npy_double*)PyArray_GETPTR1(arr,i)));
                        }
                        layer_weight.push_back(layer_weight_i);
                } else if (nd == 2) {
                        npy_intp *sp=PyArray_SHAPE(arr);
                        for (int i=0; i<*sp; i++) {
                                std::vector<double> layer_weight_i;
                                for (int j=0; j<*(sp+1); j++) {
                                        layer_weight_i.push_back(*((npy_double*)PyArray_GETPTR2(arr,i, j)));
                                }
                                layer_weight.push_back(layer_weight_i);
                        }
                } else {
                        // should not reach
                }
                weights.push_back(layer_weight);
                Py_CLEAR(arr);
        }

        NeuralReluplex* neuralReluplex = (NeuralReluplex*)PyCapsule_GetPointer(neuralReluplexCapsule_, "NeuralReluplexPtr");

        neuralReluplex->load_weights(weights);


        Py_RETURN_NONE;
}

PyMethodDef cNeuralReluplexFunctions[] = {

        {"construct",
         construct, METH_VARARGS,
         "Create `NeuralReluplex` object"},

        // {"initializeCell",
        //  initializeCell, METH_VARARGS,
        //  "Initialize cells in NeuralReluplex"},
        //
        // {"markBasic",
        //  markBasic, METH_VARARGS,
        //  "Mark Basic variables in NeuralReluplex"},
        //
        // {"setLowerBound",
        //  setLowerBound, METH_VARARGS,
        //  "Set lower bound of a variable in NeuralReluplex"},
        //
        // {"setUpperBound",
        //  setUpperBound, METH_VARARGS,
        //  "Set upper bound of a variable in NeuralReluplex"},
        //
        // {"setReluPair",
        //  setReluPair, METH_VARARGS,
        //  "Set ReluPair of two forward and backward variables in NeuralReluplex"},
        //
        // {"setName",
        //  setName, METH_VARARGS,
        //  "Set name of a variable in NeuralReluplex"},
        //
        // {"getAssignment",
        //  getAssignment, METH_VARARGS,
        //  "Get assignment of a variable in NeuralReluplex"},
        //
        // {"setLogging",
        //  setLogging, METH_VARARGS,
        //  "Set logging flag in NeuralReluplex"},
        //
        // {"setDumpStates",
        //  setDumpStates, METH_VARARGS,
        //  "Set DumpStates flag in NeuralReluplex"},
        //
        // {"toggleAlmostBrokenReluEliminiation",
        //  toggleAlmostBrokenReluEliminiation, METH_VARARGS,
        //  "toggle AlmostBrokenReluEliminiation flag in NeuralReluplex"},

        {"solve",
         solve, METH_VARARGS,
         "Solve NeuralReluplex"},

        {"getInputAssignment",
         getInputAssignment, METH_VARARGS,
         "Get assignment of input layer in NeuralReluplex"},

        {"getOutputAssignment",
         getOutputAssignment, METH_VARARGS,
         "Get assignment of output layer in NeuralReluplex"},

        {"delete_object",
         delete_object, METH_VARARGS,
         "Delete `NeuralReluplex` object"},

        {"build_network",
         build_network, METH_VARARGS,
         "Build neural network from weights in `NeuralReluplex` object"},

        {"inputFromIntervalBox",
         inputFromIntervalBox, METH_VARARGS,
         "Input are from this interval box"},

        {"inputNotFromIntervalBox",
         inputNotFromIntervalBox, METH_VARARGS,
         "Input are not from this interval box"},

        {"i_does_not_win",
         i_does_not_win, METH_VARARGS,
         "i-th output does not win"},

        {NULL, NULL, 0, NULL}
};


struct PyModuleDef cNeuralReluplexModule = {
        PyModuleDef_HEAD_INIT,
        "cReluplex",
        NULL,
        -1,
        cNeuralReluplexFunctions
};


PyMODINIT_FUNC PyInit_cNeuralReluplex(void) {
        import_array();
        return PyModule_Create(&cNeuralReluplexModule);
}

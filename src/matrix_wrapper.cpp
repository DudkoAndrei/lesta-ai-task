#include <Python.h>

#include "matrix.h"

extern PyTypeObject MatrixWrapper_Type;

struct MatrixWrapper
{
    PyObject_HEAD
    Matrix<double>* matrix = nullptr;
};

static PyObject* MatrixWrapper_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    auto* self = (MatrixWrapper*)type->tp_alloc(type, 0);

    return (PyObject*)self;
}

static int MatrixWrapper_init(MatrixWrapper* self, PyObject* args, PyObject* kwds)
{
    size_t rows;
    size_t cols;
    PyObject* data = nullptr;
    static char* kwlist[] = {"rows", "cols", "data", nullptr};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "nn|O!", kwlist,
                                     &rows, &cols, &PyList_Type, &data))
    {
        PyErr_SetString(PyExc_ValueError, "Invalid arguments");
        return -1;
    }

    if (data)
    {
        std::vector<double> vec;
        size_t size = PyList_Size(data);
        for (size_t i = 0; i < size; ++i)
        {
            PyObject* item = PyList_GetItem(data, i);
            if (!PyFloat_Check(item) && !PyInt_Check(item))
            {
                PyErr_SetString(PyExc_ValueError, "Invalid arguments");
                return -1;
            }

            vec.push_back(PyFloat_AsDouble(item));
        }

        try {
            self->matrix = new Matrix<double>(rows, cols, vec);
        }
        catch (const std::exception& e) 
        {
            PyErr_SetString(PyExc_ValueError, e.what());
            return -1;
        }
    } 
    else 
    {
        self->matrix = new Matrix<double>(rows, cols);
    }

    return 0;
}

static void MatrixWrapper_dealloc(MatrixWrapper* self)
{
    delete self->matrix;
    Py_TYPE(self)->tp_free(self);
}

static PyObject* MatrixWrapper_get_rows(MatrixWrapper* self, void* closure)
{
    return PyInt_FromSize_t(self->matrix->GetRows());
}

static PyObject* MatrixWrapper_get_cols(MatrixWrapper* self, void* closure)
{
    return PyInt_FromSize_t(self->matrix->GetColumns());
}

static PyObject* MatrixWrapper_get_data(MatrixWrapper* self, void* closure)
{
    auto& matrix_data = self->matrix->GetData();

    PyObject* list = PyList_New(matrix_data.size());
    for (size_t i = 0; i < matrix_data.size(); ++i)
    {
        PyList_SetItem(list, i, PyFloat_FromDouble(matrix_data[i]));
    }

    return list;
}

static PyObject* MatrixWrapper_multiply(MatrixWrapper* self, PyObject* other)
{
    if (!PyObject_TypeCheck(other, &MatrixWrapper_Type))
    {
        PyErr_SetString(PyExc_TypeError, "Argument must be a Matrix");
        return nullptr;
    }

    try
    {
        auto* other_matrix = (MatrixWrapper*)other;

        auto* result = new Matrix<double>(self->matrix->operator*(*other_matrix->matrix));

        auto* py_result = (MatrixWrapper*)MatrixWrapper_new(&MatrixWrapper_Type, nullptr, nullptr);
        py_result->matrix = result;

        return (PyObject*)py_result;
    }
    catch (const std::exception& e)
    {
        PyErr_SetString(PyExc_ValueError, e.what());
        return nullptr;
    }
}

static PyObject* MatrixWrapper_richcompare(PyObject* self, PyObject* other, int op)
{
    if (op != Py_EQ && op != Py_NE)
    {
        PyErr_SetString(PyExc_NotImplementedError, "Only equal and not equal is implemented");
        return nullptr;
    }

    if (!PyObject_TypeCheck(other, &MatrixWrapper_Type))
    {
        PyErr_SetString(PyExc_TypeError, "Wrong type, expected Matrix");
        return nullptr;
    }

    auto* self_matrix = (MatrixWrapper*)self;
    auto* other_matrix = (MatrixWrapper*)other;

    bool is_equal = self_matrix->matrix->operator==(*other_matrix->matrix);

    if (op == Py_EQ)
    {
        return PyBool_FromLong(is_equal);
    }
    else
    {
        return PyBool_FromLong(!is_equal);
    }
}

PyMethodDef MatrixWrapper_methods[] = {
    {"multiply", (PyCFunction)MatrixWrapper_multiply, METH_O, "Multiply two matrices"},
    {nullptr}
};

PyGetSetDef MatrixWrapper_getset[] = {
    {"rows", (getter)MatrixWrapper_get_rows, nullptr, "Number of rows", nullptr},
    {"cols", (getter)MatrixWrapper_get_cols, nullptr, "Number of columns", nullptr},
    {"data", (getter)MatrixWrapper_get_data, nullptr, "Matrix data", nullptr},
    {nullptr}
};

PyTypeObject MatrixWrapper_Type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0) 
    "matrix_lib.Matrix",                                        /* tp_name */
    sizeof(MatrixWrapper),                                      /* tp_basicsize */
    0,                                                          /* tp_itemsize */
    (destructor)MatrixWrapper_dealloc,                          /* tp_dealloc */
    0,                                                          /* tp_print */
    0,                                                          /* tp_getattr */
    0,                                                          /* tp_setattr */
    0,                                                          /* tp_compare */
    0,                                                          /* tp_repr */
    0,                                                          /* tp_as_number */
    0,                                                          /* tp_as_sequence */
    0,                                                          /* tp_as_mapping */
    0,                                                          /* tp_hash */
    0,                                                          /* tp_call */
    0,                                                          /* tp_str */
    0,                                                          /* tp_getattro */
    0,                                                          /* tp_setattro */
    0,                                                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                                         /* tp_flags */
    "Matrix objects",                                           /* tp_doc */
    0,                                                          /* tp_traverse */
    0,                                                          /* tp_clear */
    MatrixWrapper_richcompare,                                  /* tp_richcompare */
    0,                                                          /* tp_weaklistoffset */
    0,                                                          /* tp_iter */
    0,                                                          /* tp_iternext */
    MatrixWrapper_methods,                                      /* tp_methods */
    0,                                                          /* tp_members */
    MatrixWrapper_getset,                                       /* tp_getset */
    0,                                                          /* tp_base */
    0,                                                          /* tp_dict */
    0,                                                          /* tp_descr_get */
    0,                                                          /* tp_descr_set */
    0,                                                          /* tp_dictoffset */
    (initproc)MatrixWrapper_init,                               /* tp_init */
    0,                                                          /* tp_alloc */
    MatrixWrapper_new,                                          /* tp_new */
};

PyMODINIT_FUNC initmatrix_lib()
{
    if (PyType_Ready(&MatrixWrapper_Type) < 0)
    {
        return;
    }

    PyObject* module = Py_InitModule3("matrix_lib", nullptr, "Matrix multiplication module");
    if (module == nullptr)
    {
        return;
    }

    Py_INCREF(&MatrixWrapper_Type);
    PyModule_AddObject(module, "Matrix", (PyObject*)&MatrixWrapper_Type);
}

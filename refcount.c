#include <Python.h>
#include <structmember.h>
#include <stddef.h>

typedef struct MyObj {
    PyObject_HEAD
    PyObject *other;
    int extra;
    char *name;
} MyObj;

static void
myobj_dealloc(MyObj *self)
{
    MyObj *other = (MyObj *)self->other;
    if (other) {
        if (Py_REFCNT(other) + other->extra > 0) {
            Py_REFCNT(other) += other->extra;
            other->extra = 0;

            Py_REFCNT(self) = 1;
            self->extra = -1;
            return;
        }
        self->other = NULL;
        other->other = NULL;
        Py_DECREF(other);
    }
    printf("dealloc: %s (%p)\n", self->name, self);
    PyObject_Del(self);
}

static PyMemberDef members[] = {
    {"other", T_OBJECT, offsetof(MyObj, other), 0, NULL },
    {NULL}
};

static PyTypeObject MyObjType = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    .tp_name = "MyObj",
    .tp_basicsize = sizeof(MyObj),
    .tp_dealloc = (destructor)myobj_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_members = members
};

static PyObject *
create_pair(PyObject *mod, PyObject *_unused)
{
    MyObj *a = PyObject_New(MyObj, &MyObjType);
    if (a == NULL) return NULL;

    MyObj *b = PyObject_New(MyObj, &MyObjType);
    if (b == NULL) return NULL;

    a->extra = b->extra = 0;
    a->name = "a";
    b->name = "b";
    a->other = (PyObject *)b;
    b->other = (PyObject *)a;

    PyObject *tuple = PyTuple_New(2);
    if (tuple == NULL) return NULL;

    PyTuple_SET_ITEM(tuple, 0, (PyObject *)a);
    PyTuple_SET_ITEM(tuple, 1, (PyObject *)b);
    return tuple;
}

static PyMethodDef module_methods[] = {
    {"create_pair", create_pair, METH_NOARGS, NULL},
    { NULL, NULL }
};

static struct PyModuleDef refcountmodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "_refcount",
    .m_methods = module_methods
};

PyMODINIT_FUNC
PyInit__refcount(void)
{
    if (PyType_Ready(&MyObjType) < 0) {
        return NULL;
    }
    return PyModuleDef_Init(&refcountmodule);
}

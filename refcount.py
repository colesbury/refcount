import _refcount

def test1():
    print('\ntest1')
    a, b = _refcount.create_pair()
    assert a.other is b
    assert b.other is a
    print('test1: a and b out of scope', hex(id(a)), hex(id(b)))

def test2():
    print('\ntest2')
    a, b = _refcount.create_pair()
    del b
    assert a.other is not None
    print('test2: deleting a', hex(id(a)))


def test3():
    print('\ntest3')
    a, b = _refcount.create_pair()
    del b
    b = a.other
    del a
    print('test3: deleting b', hex(id(b)))
    del b

def test4():
    print('\ntest4')
    a, b = _refcount.create_pair()
    d = {"a": a}
    del a, b
    print("d['a']", hex(id(d["a"])))

test1()
test2()
test3()
test4()
from distutils.core import setup, Extension

module = Extension('_refcount',
                   sources = ['refcount.c'])

setup (name = 'RefCount',
       version = '1.0',
       description = 'For Ed',
       ext_modules = [module])

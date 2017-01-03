from distutils.core import setup
setup(name='x527',
      version='1.0.0',
      packages=['x527',
                'x527.engine',
                'x527.loggers',
                'x527.sequencer',
                'x527.tinyrpc',
                'x527.tinyrpc.dispatch',
                'x527.tinyrpc.protocols',
                'x527.tinyrpc.proxy',
                'x527.tinyrpc.server',
                'x527.tinyrpc.transports',
                'x527.util',
                'pypudding'],
      py_modules=['fctplugin'],
      package_dir={'pypudding': 'pypudding'},
      package_data={'pypudding': ['_pypudding_w.so']}
      )

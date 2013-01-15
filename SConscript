import os

env = Environment(
  ENV = {'PATH' : os.environ['PATH']},
  CCFLAGS = ['-std=c++0x', '-O3'],
  )

rcv_env = env.Clone()
rcv_env.Append(CPPPATH = ['RCF/include/'])
rcv_env.Append(CCFLAGS = ['-DRCF_USE_PROTOBUF', '-w'])

rcv_env.Program('grabcut_test_server',
  [ 'GrabcutMaskJudge.cc',
    'grabcut_test.pb.cc',
    'RCF/src/RCF/RCF.cpp'],
  LIBS = ['gflags', 'glog', 'protobuf'])

rcv_env.Program('mock_client',
  [ 'mock_client.cc',
    'grabcut_test.pb.cc',
    'RCF/src/RCF/RCF.cpp'],
  LIBS = ['gflags', 'glog', 'protobuf', 'opencv_core', 'opencv_highgui'])

swig_env = rcv_env.Clone()
swig_env.ParseConfig('pkg-config --cflags --libs python')

swig_env.SharedLibrary('_GrabcutTestClient',
  [ 'grabcut_test.pb.cc',
    'GrabcutTestClient.cc',
    'GrabcutTestClient_wrap.cxx',
    'RCF/src/RCF/RCF.cpp'
  ],
  LIBS = ['protobuf', 'gflags', 'glog'])

env.Program('floodfill',
  [ 'floodfillmain.cc',
    'MaskFloodfill.cc',
  ],
  LIBS = ['gflags', 'glog', 'opencv_core', 'opencv_highgui'])

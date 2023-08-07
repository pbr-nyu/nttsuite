#! /bin/csh -f
setenv SYSTEMC_HOME /opt/mentorgraphics/Catapult_10.5c/Mgc_home//shared
setenv SYSTEMC_LIB_DIR /opt/mentorgraphics/Catapult_10.5c/Mgc_home//shared/lib
setenv CXX_FLAGS "-g -DCALYPTO_SKIP_SYSTEMC_VERSION_CHECK"
setenv LD_FLAGS "-lpthread"
setenv OSSIM ddd
setenv PATH /opt/mentorgraphics/Catapult_10.5c/Mgc_home//bin:$PATH

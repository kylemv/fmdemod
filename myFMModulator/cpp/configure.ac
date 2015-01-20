AC_INIT(myFMModulator, 1.0.0)
AM_INIT_AUTOMAKE(nostdinc)

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL

AC_CORBA_ORB
OSSIE_CHECK_OSSIE
OSSIE_SDRROOT_AS_PREFIX

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Dependencies
export PKG_CONFIG_PATH="$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig"
PKG_CHECK_MODULES([PROJECTDEPS], [ossie >= 1.10 omniORB4 >= 4.1.0  ])
PKG_CHECK_MODULES([INTERFACEDEPS], [bulkio >= 1.10])
OSSIE_ENABLE_LOG4CXX
AX_BOOST_BASE([1.41])
AX_BOOST_SYSTEM
AX_BOOST_THREAD
AX_BOOST_REGEX

export PKG_CONFIG_PATH="$PKG_CONFIG_PATH:${SDR_ROOT}/dom/deps/liquid_dsp_v1/cpp/lib/pkgconfig"
PKG_CHECK_MODULES(liquid_dsp_v1, liquid_dsp_v1 >= 1.2.0, , AC_MSG_ERROR("Unable to find x86_64 liquid DSP Shared Package"))
AC_SUBST(liquid_dsp_v1_CFLAGS)
AC_SUBST(liquid_dsp_v1_LIBS)
CXXFLAGS="$CXXFLAGS $liquid_dsp_v1_CFLAGS"
LIBS="$LIBS $liquid_dsp_v1_LIBS"

AC_CONFIG_FILES([Makefile])
AC_OUTPUT


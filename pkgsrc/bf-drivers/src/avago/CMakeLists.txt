project(libavago VERSION 0.1 LANGUAGES C)

file(COPY ../../libavago.i686.a DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
file(COPY ../../libavago.i686.so DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
file(COPY ../../libavago.a DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
file(COPY ../../libavago.so DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

add_library(avago SHARED IMPORTED GLOBAL)

# sde built with PIC enabled. Please refer to top-level CMakeLists.txt
if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
  set_property(TARGET avago PROPERTY IMPORTED_LOCATION "${CMAKE_INSTALL_PREFIX}/lib/libavago.a")
else()
  set_property(TARGET avago PROPERTY IMPORTED_LOCATION "${CMAKE_INSTALL_PREFIX}/lib/libavago.i686.a")
endif()

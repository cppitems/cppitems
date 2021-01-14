#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Widget::widget" for configuration "Debug"
set_property(TARGET Widget::widget APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Widget::widget PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libwidget.so"
  IMPORTED_SONAME_DEBUG "libwidget.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS Widget::widget )
list(APPEND _IMPORT_CHECK_FILES_FOR_Widget::widget "${_IMPORT_PREFIX}/lib/libwidget.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

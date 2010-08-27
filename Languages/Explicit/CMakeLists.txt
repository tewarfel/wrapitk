###############################################################################
# install the files requires for explicit
IF(NOT EXPLICITAL_WRAP_ITK_PROJECT)
  WRAP_ITK_INSTALL(/Configuration/Languages/Explicit CMakeLists.txt)
  WRAP_ITK_INSTALL(/Configuration/Languages/Explicit gcc_xml.inc.in)
  WRAP_ITK_INSTALL(/Configuration/Languages/Explicit wrap_.cxx.in)
ENDIF(NOT EXPLICITAL_WRAP_ITK_PROJECT)


###############################################################################
# hide cable swig dir in explicital projects - it is safer to use the same as the
# one used to build wrapitk
IF(EXPLICITAL_WRAP_ITK_PROJECT)
  MARK_AS_ADVANCED(CableSwig_DIR)
ENDIF(EXPLICITAL_WRAP_ITK_PROJECT)


###############################################################################
# store the current dir, so it can be reused later
SET(WRAP_ITK_EXPLICIT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "explicit source dir")
SET(WRAP_ITK_EXPLICIT_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}" CACHE INTERNAL "explicit binary dir")


SET(WRAPPER_MASTER_INDEX_OUTPUT_DIR "${PROJECT_BINARY_DIR}/Typedefs")
SET(WRAPPER_SWIG_LIBRARY_OUTPUT_DIR "${PROJECT_BINARY_DIR}/SWIG")



MACRO(WRAP_INCLUDE_EXPLICIT include_file)
  IF("${include_file}" MATCHES "<.*>")
    SET(EXPLICIT_EXPLICITS "${EXPLICIT_EXPLICITS}#include ${include_file}\n")
  ELSE("${include_file}" MATCHES "<.*>")
    SET(EXPLICIT_EXPLICITS "${EXPLICIT_EXPLICITS}#include \"${include_file}\"\n")
  ENDIF("${include_file}" MATCHES "<.*>")
ENDMACRO(WRAP_INCLUDE_EXPLICIT)


MACRO(ADD_SIMPLE_TYPEDEF_EXPLICIT wrap_class swig_name)
  IF("${wrap_class}" MATCHES "<.*>")
    SET(EXPLICIT_EXPLICITS "${EXPLICIT_EXPLICITS}extern template class ${wrap_class};\n")
    SET(EXPLICIT_INSTANTIATES "${EXPLICIT_INSTANTIATES}template class ${wrap_class};\n")
  ENDIF("${wrap_class}" MATCHES "<.*>")
ENDMACRO(ADD_SIMPLE_TYPEDEF_EXPLICIT)


MACRO(WRAP_MODULE_EXPLICIT module)
  # clear the typedefs and the includes
  SET(EXPLICIT_INSTANTIATES )
ENDMACRO(WRAP_MODULE_EXPLICIT)

MACRO(END_WRAP_MODULE_EXPLICIT module)
  # Create the cxx file.
  SET(file_name "${module}.explicit.cxx")
  SET(cxx_file "${WRAPPER_LIBRARY_OUTPUT_DIR}/${file_name}")
  SET(CONFIG_EXPLICIT_CONTENT "#include \"${WRAPPER_LIBRARY_NAME}.explicit.h\"\n")
  SET(CONFIG_EXPLICIT_CONTENT "${CONFIG_EXPLICIT_CONTENT}${EXPLICIT_INSTANTIATES}")
  CONFIGURE_FILE("${WRAP_ITK_EXPLICIT_SOURCE_DIR}/explicit.in"
    "${cxx_file}" @ONLY IMMEDIATE)
  SET(EXPLICIT_EXPLICITS_FILES ${EXPLICIT_EXPLICITS_FILES} ${cxx_file})
ENDMACRO(END_WRAP_MODULE_EXPLICIT)


MACRO(ADD_ONE_TYPEDEF_EXPLICIT  wrap_method wrap_class swig_name template_params)
  # insert a blank line to separate the classes
  SET(EXPLICIT_EXPLICITS "${EXPLICIT_EXPLICITS}\n")
  SET(EXPLICIT_INSTANTIATES "${EXPLICIT_INSTANTIATES}\n")
  # Add one  typedef to WRAPPER_TYPEDEFS
  # 'wrap_method' is the one of the valid WRAPPER_WRAP_METHODS from WRAP_CLASS,
  # 'wrap_class' is the fully-qualified C++ name of the class
  # 'swig_name' is what the swigged class should be called
  # The optional last argument is the template parameters that should go between 
  # the < > brackets in the C++ template definition.
  # Only pass 3 parameters to wrap a non-templated class
  #
  # Global vars used: none
  # Global vars modified: WRAPPER_TYPEDEFS
  
  # get the base C++ class name (no namespaces) from wrap_class:
  STRING(REGEX REPLACE "(.*::)" "" base_name "${wrap_class}")

  SET(wrap_pointer 0)
  SET(template_parameters "${ARGV3}")
  IF(template_parameters)
    SET(full_class_name "${wrap_class}< ${template_parameters} >")
  ELSE(template_parameters)
    SET(full_class_name "${wrap_class}")
  ENDIF(template_parameters)
  
  # ADD_ONE_TYPEDEF_ALL_LANGUAGES("${wrap_method}" "${wrap_class}" "${swig_name}" "${ARGV3}")
  
  # Add a typedef for the class. We have this funny looking full_name::base_name
  # thing (it expands to, for example "typedef itk::Foo<baz, 2>::Foo") used
  # for gccxml typedefs

# don't do superclasses for now

#   IF("${wrap_method}" MATCHES "2_SUPERCLASSES")
#     ADD_SIMPLE_TYPEDEF_EXPLICIT("${full_class_name}::Superclass::Superclass" "${swig_name}_Superclass_Superclass")
#     ADD_SIMPLE_TYPEDEF_EXPLICIT("${full_class_name}::Superclass::Superclass::Pointer::SmartPointer" "${swig_name}_Superclass_Superclass_Pointer")
#   ENDIF("${wrap_method}" MATCHES "2_SUPERCLASSES")
# 
#   IF("${wrap_method}" MATCHES "SUPERCLASS")
#     ADD_SIMPLE_TYPEDEF_EXPLICIT("${full_class_name}::Superclass" "${swig_name}_Superclass")
#     ADD_SIMPLE_TYPEDEF_EXPLICIT("${full_class_name}::Superclass::Pointer::SmartPointer" "${swig_name}_Superclass_Pointer")
#   ENDIF("${wrap_method}" MATCHES "SUPERCLASS")

  # the same output with or without FORCE_INSTANTIATE
  ADD_SIMPLE_TYPEDEF_EXPLICIT("${full_class_name}" "${swig_name}")

# don't do pointers for now

#   IF("${wrap_method}" MATCHES "POINTER")
#     IF("${wrap_method}" STREQUAL "AUTOPOINTER")
#       # add a pointer typedef if we are so asked
#       ADD_SIMPLE_TYPEDEF_EXPLICIT("${full_class_name}::SelfAutoPointer" "${swig_name}_AutoPointer")
#     ELSE("${wrap_method}" STREQUAL "AUTOPOINTER")
#       # add a pointer typedef if we are so asked
#       ADD_SIMPLE_TYPEDEF_EXPLICIT("itk::SmartPointer< ${full_class_name} >" "${swig_name}_Pointer")
#     ENDIF("${wrap_method}" STREQUAL "AUTOPOINTER")
#   ENDIF("${wrap_method}" MATCHES "POINTER")
ENDMACRO(ADD_ONE_TYPEDEF_EXPLICIT)

MACRO(WRAP_LIBRARY_EXPLICIT library_name)
  SET(EXPLICIT_EXPLICITS )
  SET(EXPLICIT_EXPLICITS_FILES )
ENDMACRO(WRAP_LIBRARY_EXPLICIT)

MACRO(END_WRAP_LIBRARY_EXPLICIT)
  SET(file_name "${WRAPPER_LIBRARY_NAME}.explicit.h")
  SET(cxx_file "${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/${file_name}")
  SET(CONFIG_EXPLICIT_CONTENT )
  FOREACH(dep ${WRAPPER_LIBRARY_DEPENDS})
    SET(CONFIG_EXPLICIT_CONTENT "${CONFIG_EXPLICIT_CONTENT}#include \"${dep}.explicit.h\"\n")
  ENDFOREACH(dep)
  SET(CONFIG_EXPLICIT_CONTENT "${CONFIG_EXPLICIT_CONTENT}${EXPLICIT_EXPLICITS}")
  CONFIGURE_FILE("${WRAP_ITK_EXPLICIT_SOURCE_DIR}/explicit.in"
    "${cxx_file}" @ONLY IMMEDIATE)
  IF(EXPLICIT_EXPLICITS_FILES)
    ADD_LIBRARY(${WRAPPER_LIBRARY_NAME}Explicit SHARED ${EXPLICIT_EXPLICITS_FILES})
    FOREACH(dep ${WRAPPER_LIBRARY_DEPENDS})
      TARGET_LINK_LIBRARIES(${WRAPPER_LIBRARY_NAME}Explicit ${dep}Explicit)
    ENDFOREACH(dep)
  ENDIF(EXPLICIT_EXPLICITS_FILES)
ENDMACRO(END_WRAP_LIBRARY_EXPLICIT)

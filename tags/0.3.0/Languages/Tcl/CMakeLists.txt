###############################################################################
# configure tcl
FIND_PACKAGE(TCL REQUIRED)
# Hide useless settings provided by FindTCL.
FOREACH(entry TCL_LIBRARY_DEBUG
              TK_LIBRARY_DEBUG
              TCL_STUB_LIBRARY
              TCL_STUB_LIBRARY_DEBUG
              TK_STUB_LIBRARY
              TK_STUB_LIBRARY_DEBUG
              TK_WISH)
  SET(${entry} "${${entry}}" CACHE INTERNAL "This value is not used by ITK.")
ENDFOREACH(entry)

INCLUDE_DIRECTORIES(${TCL_INCLUDE_PATH} ${TK_INCLUDE_PATH})


###############################################################################
# store the current dir, so it can be reused later
SET(WRAP_ITK_TCL_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "tcl source dir")
SET(WRAP_ITK_TCL_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}" CACHE INTERNAL "tcl binary dir")


###############################################################################
# create the tcl directory in the classindex dir
FILE(MAKE_DIRECTORY ${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/tcl)


###############################################################################
# Configure pkgIndex.tcl for the build tree.
IF(CMAKE_CONFIGURATION_TYPES)
  FOREACH(config ${CMAKE_CONFIGURATION_TYPES})
    SET(WRAP_ITK_TCL_PACKAGE_DIR "${LIBRARY_OUTPUT_PATH}/${config}")
    CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/pkgIndex.tcl.in"
                   "${CMAKE_CURRENT_BINARY_DIR}/${config}/pkgIndex.tcl"
                   @ONLY IMMEDIATE)
  ENDFOREACH(config)
ELSE(CMAKE_CONFIGURATION_TYPES)
  SET(WRAP_ITK_TCL_PACKAGE_DIR "${LIBRARY_OUTPUT_PATH}")
  CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/pkgIndex.tcl.in"
                 "${CMAKE_CURRENT_BINARY_DIR}/pkgIndex.tcl"
                 @ONLY IMMEDIATE)
ENDIF(CMAKE_CONFIGURATION_TYPES)

# configure pkgIndex.tcl for the installed tree
SET(WRAP_ITK_TCL_PACKAGE_DIR "${CMAKE_INSTALL_PREFIX}/${WRAP_ITK_INSTALL_PREFIX}/lib")
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/pkgIndex.tcl.in"
               "${CMAKE_CURRENT_BINARY_DIR}/InstallOnly/itkwish_wont_find_me_here/pkgIndex.tcl"
               @ONLY IMMEDIATE)
WRAP_ITK_INSTALL(/Tcl "${CMAKE_CURRENT_BINARY_DIR}/InstallOnly/itkwish_wont_find_me_here/pkgIndex.tcl")


###############################################################################
# Configure itkwish
SET(WRAP_ITK_TCL_EXE_DIR_BUILD "${ITK_EXECUTABLE_PATH}")
SET(WRAP_ITK_TCL_EXE_NAME_ROOT "itkwish")

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/itkTclConfigure.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/itkTclConfigure.h
               @ONLY IMMEDIATE)


# Configure itkTclModules.h with the names of the initializer functions for the
# selected modules. For some pathological reason, the names are in the format:
# Modulename_Init
# where the first character is uppercase and the rest of the module name is 
# lower case. Which will entail some amount of pain to create in CMake.
SET(module_init_names )
FOREACH(module ${WRAP_ITK_LIBRARIES})
  STRING(REGEX REPLACE "^(.)(.+)$" "\\1" first "${module}")
  STRING(REGEX REPLACE "^(.)(.+)$" "\\2" rest "${module}")
  STRING(TOUPPER "${first}" first)
  STRING(TOLOWER "${rest}" rest)
  SET(module_init_names ${module_init_names} "${first}${rest}tcl_Init")
ENDFOREACH(module)

# Now take those init functions and make a list of symbol declarations like:
# int Modulename_Init(Tcl_Interp*);
SET(WRAP_ITK_TCL_LIBRARIES_INIT_DECLARATIONS )
FOREACH(module ${module_init_names})
  SET(WRAP_ITK_TCL_LIBRARIES_INIT_DECLARATIONS "${WRAP_ITK_TCL_LIBRARIES_INIT_DECLARATIONS}  int ${module}(Tcl_Interp*);\n")
ENDFOREACH(module)

# Now we pack the addresses of the init function names into a comma-separated 
# string, to be used in an array initializer
SET(initializer )
FOREACH(module ${module_init_names})
  SET(initializer "${initializer}  &${module},\n")
ENDFOREACH(module)
# Let's trim off that last ',\n':
STRING(REGEX REPLACE ",\n$" "" WRAP_ITK_TCL_LIBRARIES_ARRAY_INITIALIZER "${initializer}")

# Let's get the number of modules configured.
LIST(LENGTH WRAP_ITK_LIBRARIES WRAP_ITK_TCL_LIBRARIES_COUNT)

# And finally configure the file
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/itkTclModules.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/itkTclModules.h
               @ONLY IMMEDIATE)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
ADD_EXECUTABLE(itkwish itkTclAppInit.cxx)
FOREACH(module ${WRAP_ITK_LIBRARIES})
  TARGET_LINK_LIBRARIES(itkwish ${module}Tcl)
ENDFOREACH(module)

TARGET_LINK_LIBRARIES(itkwish ${TCL_LIBRARY} ${TK_LIBRARY})

# configure and install the driver itkwish script
SET(WRAP_ITK_TCL_DIR "${CMAKE_INSTALL_PREFIX}/${WRAP_ITK_INSTALL_PREFIX}/Tcl")
SET(WRAP_ITK_TCL_SWIG_DIR "${CMAKE_INSTALL_PREFIX}/${WRAP_ITK_INSTALL_PREFIX}/lib")
SET(WRAP_ITK_TCL_ITKWISH_DIR "${CMAKE_INSTALL_PREFIX}/${WRAP_ITK_INSTALL_PREFIX}/bin")
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/itkwish.in
               ${CMAKE_CURRENT_BINARY_DIR}/InstallOnly/itkwish
               @ONLY IMMEDIATE )
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/InstallOnly/itkwish
  DESTINATION bin
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

# configure a itkwish usable in build tree
# use WrapITK_BINARY_DIR instead of the current bianry dir because tcl search pkgIndex.tcl in 
# the subdirs before searching in the current dir, and so find the pikIndex.tcl file in InstallOnly
# first
SET(WRAP_ITK_TCL_DIR "${WrapITK_BINARY_DIR}/Languages")
SET(WRAP_ITK_TCL_SWIG_DIR "${LIBRARY_OUTPUT_PATH}")
SET(WRAP_ITK_TCL_ITKWISH_DIR "${EXECUTABLE_OUTPUT_PATH}")
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/itkwish.in
               ${CMAKE_CURRENT_BINARY_DIR}/itkwish
               @ONLY IMMEDIATE )


#install the actual executable
INSTALL(TARGETS itkwish DESTINATION "${WRAP_ITK_INSTALL_PREFIX}/bin")
INSTALL(FILES itkinteraction.tcl itktesting.tcl itkdata.tcl itkutils.tcl
  DESTINATION "${WRAP_ITK_INSTALL_PREFIX}/Tcl"
)








MACRO(WRAP_LIBRARY_TCL library_name)
  SET(WRAP_ITK_TCL_LIBRARY_DEPS )
  SET(WRAP_ITK_TCL_LIBRARY_DECLS )
  SET(WRAP_ITK_TCL_LIBRARY_CALLS )
  SET(WRAP_ITK_TCL_CXX_FILES )
ENDMACRO(WRAP_LIBRARY_TCL)


MACRO(END_WRAP_LIBRARY_TCL)

  # Loop over the extra swig input files and add them to the generated files 
  # lists. Guess that the generated cxx output will have the same name as 
  # the .i input file.
  FOREACH(source ${WRAPPER_LIBRARY_SWIG_INPUTS})
    GET_FILENAME_COMPONENT(base_name ${source} NAME_WE)
    STRING(REPLACE "wrap_" "" base_name "${base_name}")
    WRAP_MODULE_TCL("${base_name}")
    END_WRAP_MODULE_TCL("${base_name}")
  ENDFOREACH(source)


    
  # create the tcl customization stuff in the main module
  # it allow to group the tcls module in a single shared lib, by loading the int
  # functions of the module. I also import the objects from the submodules in the
  # main module.
  #
  # It uses WRAP_ITK_TCL_LIBRARY_DECLS, WRAP_ITK_TCL_LIBRARY_CALLS
  CONFIGURE_FILE("${WRAP_ITK_TCL_SOURCE_DIR}/main_module_ext.i.in"
    "${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/tcl/${WRAPPER_LIBRARY_NAME}_ext.i"
    @ONLY IMMEDIATE)
  WRAP_ITK_INSTALL("/Configuration/Typedefs/tcl"
    "${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/tcl/${WRAPPER_LIBRARY_NAME}_ext.i"
  )


  # set some var reused later
  SET(interface_file "${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/${WRAPPER_LIBRARY_NAME}.i")
  SET(lib ${WRAPPER_LIBRARY_NAME}Tcl)
  SET(cpp_file "${CMAKE_CURRENT_BINARY_DIR}/${WRAPPER_LIBRARY_NAME}Tcl.cpp")

  SET(swig_command ${SWIG_EXECUTABLE})
  IF(WRAP_ITK_USE_CCACHE)
    SET(swig_command ${CCACHE_EXECUTABLE} ${swig_command})
  ENDIF(WRAP_ITK_USE_CCACHE)  

  # and generate c++ code from the main module swig interface.
  ADD_CUSTOM_COMMAND(
    OUTPUT ${cpp_file}
    COMMAND ${swig_command} -c++ -tcl -O -features autodoc=1 # -Werror
#       -fcompact
    -w509 # Overloaded method
    -w365 # operator+= ignored
    -w366 # operator-= ignored
    -w367 # operator*= ignored
    -w368 # operator/= ignored
    -w378 # operator!= ignored
    -w503 # Can't wrap 'operator []' unless renamed to a valid identifier.
    -w508 # Declaration of '???' shadows declaration accessible via operator->()
    ### TODO: remove that once the bug for std::vector is fixed ###
    -w401 # Maybe you forgot to instantiate 'XXX' using %template.
    ###
    -o ${cpp_file}
    -I${LANGUAGES_SRC_DIR}
    -I${WRAP_ITK_TYPEDEFS_DIRECTORY}/tcl
    -I${WRAP_ITK_TYPEDEFS_DIRECTORY}
    -outdir ${LIBRARY_OUTPUT_PATH}
    ${interface_file}
    WORKING_DIRECTORY ${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/tcl
    DEPENDS ${DEPS} ${WRAP_ITK_TCL_LIBRARY_DEPS} ${interface_file} ${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/tcl/${WRAPPER_LIBRARY_NAME}_ext.i ${SWIG_EXECUTABLE}
  )
  WRAP_ITK_INSTALL("/lib" "${tcl_file}")

  # build all the c++ files from this module in a common lib
  SET(lib ${WRAPPER_LIBRARY_NAME}Tcl)
  ADD_LIBRARY(${lib} SHARED ${cpp_file} ${WRAP_ITK_TCL_CXX_FILES} ${WRAPPER_LIBRARY_CXX_SOURCES})
  TARGET_LINK_LIBRARIES(${lib} ${WRAPPER_LIBRARY_LINK_LIBRARIES} ${TCL_LIBRARY})
  ADD_DEPENDENCIES(${lib} ${WRAPPER_LIBRARY_NAME}Swig)
  INSTALL(TARGETS "${lib}" DESTINATION "${WRAP_ITK_INSTALL_PREFIX}/lib")
  IF(NOT EXTERNAL_WRAP_ITK_PROJECT)
    # don't depends on the targets from wrapitk in external projects
    FOREACH(dep ${WRAPPER_LIBRARY_DEPENDS})
      ADD_DEPENDENCIES(${lib} ${dep}Swig)
    ENDFOREACH(dep)
  ENDIF(NOT EXTERNAL_WRAP_ITK_PROJECT)

ENDMACRO(END_WRAP_LIBRARY_TCL)


MACRO(END_WRAP_MODULE_TCL group_name)
  
  SET(base_name wrap_${group_name})

  # the default typemaps, exception handler, and includes
  SET(WRAP_ITK_TCL_SWIG_EXT "%import wrap_tclBase.i\n\n${WRAP_ITK_TCL_SWIG_EXT}")

  
  # create the swig interface for all the groups in the module
  #
  SET(interface_file "${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/${base_name}.i")
  SET(lib ${group_name}Tcl)
  SET(cpp_file "${CMAKE_CURRENT_BINARY_DIR}/${base_name}Tcl.cpp")

  # create the tcl customization for that wrap_*.cmake file.
  CONFIGURE_FILE("${WRAP_ITK_TCL_SOURCE_DIR}/module_ext.i.in"
  "${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/tcl/wrap_${group_name}_ext.i"
  @ONLY IMMEDIATE)
  WRAP_ITK_INSTALL("/Configuration/Typedefs/tcl" "${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/tcl/wrap_${group_name}_ext.i"
  )

  # prepare dependencies
  SET(DEPS )
  FOREACH(dep ${WRAPPER_LIBRARY_DEPENDS})
    SET(DEPS ${DEPS} ${${dep}SwigFiles})
  ENDFOREACH(dep)

  SET(swig_command ${SWIG_EXECUTABLE})
  IF(WRAP_ITK_USE_CCACHE)
    SET(swig_command ${CCACHE_EXECUTABLE} ${swig_command})
  ENDIF(WRAP_ITK_USE_CCACHE)  

  # and run swig to produce the c++ file and the .tcl file
  ADD_CUSTOM_COMMAND(
    OUTPUT ${cpp_file}
    COMMAND ${swig_command} -c++ -tcl -O -features autodoc=1 # -Werror
#       -fcompact
    -w509 # Overloaded method
    -w365 # operator+= ignored
    -w366 # operator-= ignored
    -w367 # operator*= ignored
    -w368 # operator/= ignored
    -w378 # operator!= ignored
    -w503 # Can't wrap 'operator []' unless renamed to a valid identifier
    -w508 # Declaration of '???' shadows declaration accessible via operator->()
    ### TODO: remove that once the bug for std::vector is fixed ###
    -w401 # Maybe you forgot to instantiate 'XXX' using %template.
    ###
    -o ${cpp_file}
    -I${LANGUAGES_SRC_DIR}
    -I${WRAP_ITK_TYPEDEFS_DIRECTORY}/tcl
    -I${WRAP_ITK_TYPEDEFS_DIRECTORY}
    -outdir ${LIBRARY_OUTPUT_PATH}
    ${interface_file}
    WORKING_DIRECTORY ${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/tcl
    DEPENDS ${DEPS} ${interface_file} ${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/tcl/${base_name}_ext.i ${SWIG_EXECUTABLE}
    # ${WRAP_ITK_TCL_LIBRARY_DEPS}
  )
    
# gcc visibility can't be used without getting errors when passing objects
# from one module to an other
#
#    IF(CMAKE_COMPILER_IS_GNUCC)
#      SET_TARGET_PROPERTIES(${lib} PROPERTIES COMPILE_FLAGS "-fvisibility=hidden")
#    ENDIF(CMAKE_COMPILER_IS_GNUCC)

  # add the c++ files which will be generated by the swig command to the
  # list of tcl related c++ files, so they can be built at the end
  # of the current module.
  SET(WRAP_ITK_TCL_CXX_FILES ${WRAP_ITK_TCL_CXX_FILES} ${cpp_file})

  # add needed files to the deps list
  SET(WRAP_ITK_TCL_LIBRARY_DEPS ${WRAP_ITK_TCL_LIBRARY_DEPS} "${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/tcl/${base_name}_ext.i" "${cpp_file}")

  # add this wrap_*.cmake stuff to the list of modules to init in the main module.
  # first the extern c declaration
  STRING(REGEX REPLACE "^(.)(.+)$" "\\1" first "${group_name}")
  STRING(REGEX REPLACE "^(.)(.+)$" "\\2" rest "${group_name}")
  STRING(TOUPPER "${first}" first)
  STRING(TOLOWER "${rest}" rest)
  SET(capital_group_name "${first}${rest}tcl")
  SET(WRAP_ITK_TCL_LIBRARY_DECLS "${WRAP_ITK_TCL_LIBRARY_DECLS}extern \"C\" int ${capital_group_name}_Init( Tcl_Interp* );\n")
  # and the call of the extern function
  SET(WRAP_ITK_TCL_LIBRARY_CALLS "${WRAP_ITK_TCL_LIBRARY_CALLS}  ${capital_group_name}_Init( interp );\n")

ENDMACRO(END_WRAP_MODULE_TCL group_name)



MACRO(ADD_ONE_TYPEDEF_TCL wrap_method wrap_class swig_name template_params)
ENDMACRO(ADD_ONE_TYPEDEF_TCL)


MACRO(ADD_TCL_CONFIG_TEMPLATE base_name wrap_class swig_name template_params)
ENDMACRO(ADD_TCL_CONFIG_TEMPLATE)


MACRO(WRAP_MODULE_TCL module)
  SET(WRAP_ITK_TCL_SWIG_EXT "")
ENDMACRO(WRAP_MODULE_TCL)


MACRO(WRAP_NAMED_CLASS_TCL class swig_name)
  # store the current class wrapped, so we can generate the typemaps for itk::ImageSource
  SET(WRAP_ITK_TCL_CURRENT_CLASS "${class}")
  SET(WRAP_ITK_TCL_CURRENT_SWIG_NAME "${swig_name}")
ENDMACRO(WRAP_NAMED_CLASS_TCL)


MACRO(WRAP_TEMPLATE_TCL name types)
ENDMACRO(WRAP_TEMPLATE_TCL name types)


MACRO(ADD_SIMPLE_TYPEDEF_TCL wrap_class swig_name)
  IF("${wrap_class}" MATCHES "<.*>")
    STRING(REGEX REPLACE "^([^<]+)< *(.+) *>([^>]*)$" "\\1" cpp_name "${wrap_class}")
    STRING(REGEX REPLACE "^([^<]+)< *(.+) *>([^>]*)$" "\\2" template_params "${wrap_class}")
    STRING(REGEX REPLACE "^([^<]+)< *(.+) *>([^>]*)$" "\\3" ext_def "${wrap_class}")
  ELSE("${wrap_class}" MATCHES "<.*>")
    SET(cpp_name "${wrap_class}")
    SET(template_params NO_TEMPLATE)
    SET(ext_def "")
  ENDIF("${wrap_class}" MATCHES "<.*>")
  STRING(REGEX REPLACE ".*::" "" simple_name "${cpp_name}")
#  MESSAGE("${wrap_class} -- ${swig_name}")
#  MESSAGE("${cpp_name} -- ${template_params} -- ${ext_def}")

# can't use that because std_vector.i complains loudly (with an error) about invalid superclass when that superclass is a std::vector
# not wrapped with %template
# but we can't declare them outside the current file without making swig use function defined outside
# TODO: reenable that once that bug is fixed
# 
#   IF("${cpp_name}" STREQUAL "itk::VectorContainer" AND NOT "${swig_name}" MATCHES "Pointer$")
#     # add a template definition for the superclass which is not in ITK
#     STRING(REGEX REPLACE "^[^,]+, *(.+) *$" "\\1" superclass_template_param "${template_params}")
#     IF("${superclass_template_param}" MATCHES "::")
#       SET(param "${superclass_template_param}")
#       STRING(REPLACE "::" "" superclass_template_param "${superclass_template_param}")
#       STRING(REPLACE "unsigned" "U" superclass_template_param "${superclass_template_param}")
#       STRING(REPLACE "signed" "S" superclass_template_param "${superclass_template_param}")
#       STRING(REPLACE "char" "C" superclass_template_param "${superclass_template_param}")
#       STRING(REPLACE "short" "S" superclass_template_param "${superclass_template_param}")
#       STRING(REPLACE "long" "L" superclass_template_param "${superclass_template_param}")
#       STRING(REPLACE "float" "F" superclass_template_param "${superclass_template_param}")
#       STRING(REPLACE "double" "D" superclass_template_param "${superclass_template_param}")
#       STRING(REPLACE " " "" superclass_template_param "${superclass_template_param}")
#       STRING(REPLACE "<" "" superclass_template_param "${superclass_template_param}")
#       STRING(REPLACE ">" "" superclass_template_param "${superclass_template_param}")
#       STRING(REPLACE "," "" superclass_template_param "${superclass_template_param}")
#       SET(WRAP_ITK_TCL_SWIG_EXT "${WRAP_ITK_TCL_SWIG_EXT}%template(${swig_name}_Superclass) std::vector< ${superclass_template_param} >;\n")
#     ENDIF("${superclass_template_param}" MATCHES "::")
#   ENDIF("${cpp_name}" STREQUAL "itk::VectorContainer" AND NOT "${swig_name}" MATCHES "Pointer$")

# This part is useless because swig doesn't recognize that std::map< X, Y > is the same than
# std::map< X, Y, std::less< X > > and doesn't support a third parameter in the template
# specification.
# TODO: make a bugreport
#
#   IF("${cpp_name}" STREQUAL "itk::MapContainer" AND NOT "${swig_name}" MATCHES "Pointer$")
#     # add a template definition for the superclass which is not in ITK
#     STRING(REGEX REPLACE "^[^,]+, *(.+) *$" "\\1" superclass_template_param "${template_params}")
#     IF("${superclass_template_param}" MATCHES "::")
#       SET(param "${superclass_template_param}")
#       STRING(REPLACE "::" "" superclass_template_param "${superclass_template_param}")
#       STRING(REPLACE "unsigned" "U" superclass_template_param "${superclass_template_param}")
#       STRING(REPLACE "signed" "S" superclass_template_param "${superclass_template_param}")
#       STRING(REPLACE "char" "C" superclass_template_param "${superclass_template_param}")
#       STRING(REPLACE "short" "S" superclass_template_param "${superclass_template_param}")
#       STRING(REPLACE "long" "L" superclass_template_param "${superclass_template_param}")
#       STRING(REPLACE "float" "F" superclass_template_param "${superclass_template_param}")
#       STRING(REPLACE "double" "D" superclass_template_param "${superclass_template_param}")
#       STRING(REPLACE " " "" superclass_template_param "${superclass_template_param}")
#       STRING(REPLACE "<" "" superclass_template_param "${superclass_template_param}")
#       STRING(REPLACE ">" "" superclass_template_param "${superclass_template_param}")
#       STRING(REPLACE "," "" superclass_template_param "${superclass_template_param}")
#       SET(WRAP_ITK_TCL_SWIG_EXT "${WRAP_ITK_TCL_SWIG_EXT}%template(${swig_name}_Superclass) std::map< unsigned long, ${superclass_template_param}, std::less< unsigned long > >;\n")
#     ENDIF("${superclass_template_param}" MATCHES "::")
#   ENDIF("${cpp_name}" STREQUAL "itk::MapContainer" AND NOT "${swig_name}" MATCHES "Pointer$")
ENDMACRO(ADD_SIMPLE_TYPEDEF_TCL)




###############################################################################
# Create the TclUtils library

IF(NOT EXTERNAL_WRAP_ITK_PROJECT)
  SUBDIRS(Tests)
  
  MACRO(END_WRAP_LIBRARIES_TCL)
#    ADD_SUBDIRECTORY(${WRAP_ITK_TCL_SOURCE_DIR}/TclUtils)
  ENDMACRO(END_WRAP_LIBRARIES_TCL)

  MACRO(WRAP_LIBRARIES_TCL)
    ADD_SUBDIRECTORY(${WRAP_ITK_TCL_SOURCE_DIR}/TclBase)
  ENDMACRO(WRAP_LIBRARIES_TCL)

ELSE(NOT EXTERNAL_WRAP_ITK_PROJECT)
  MACRO(END_WRAP_LIBRARIES_TCL)
    # just do nothing
  ENDMACRO(END_WRAP_LIBRARIES_TCL)

  MACRO(WRAP_LIBRARIES_TCL)
    # just do nothing
  ENDMACRO(WRAP_LIBRARIES_TCL)

ENDIF(NOT EXTERNAL_WRAP_ITK_PROJECT)

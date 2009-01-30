FIND_PACKAGE(Doxygen REQUIRED)

OPTION(WRAP_ITK_DOC_MAN "Generate unix manual pages." ON)

###############################################################################
# install the files requires for doxygen
IF(NOT EXTERNAL_WRAP_ITK_PROJECT)
  WRAP_ITK_INSTALL(/Configuration/Languages/Doc CMakeLists.txt)
  WRAP_ITK_INSTALL(/Configuration/Languages/Doc doxygen.config.in)
ENDIF(NOT EXTERNAL_WRAP_ITK_PROJECT)

###############################################################################
# store the current dir, so it can be reused later
SET(WRAP_ITK_DOC_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "doc source dir")
SET(WRAP_ITK_DOC_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}" CACHE INTERNAL "doc binary dir")

SET(WRAPPER_MASTER_INDEX_OUTPUT_DIR "${PROJECT_BINARY_DIR}/Typedefs" CACHE INTERNAL "typedefs dir")


###############################################################################
# the var to store the file produced by doxygen
SET(WRAP_ITK_DOC_DOXYGEN_INSTALLED_PAGES "" CACHE INTERNAL "man pages produced by doxygen and already installed")


###############################################################################
MACRO(WRAP_LIBRARY_DOC library_name)
  SET(WRAP_ITK_DOC_DOXYGEN_HEADERS )  # doxygen headers to process in this lib
  SET(WRAP_ITK_DOC_DOXYGEN_PAGES )  # pages produced by doxygen in this lib
  SET(WRAP_ITK_DOC_DOXYGEN_XML_FILES )  # xml files produced by doxygen in this lib
  SET(WRAP_ITK_DOC_DOCSTRING_FILES )  # swig docstring files produced by doxygen in this lib
ENDMACRO(WRAP_LIBRARY_DOC)


###############################################################################
MACRO(WRAP_NAMED_CLASS_DOC class swig_name)
  SET(WRAP_ITK_DOC_GENERATE_DOXY2SWIG_INPUT OFF)
  GET_PROPERTY(dirs DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
  SET(paths )
  FOREACH(dir ${dirs})
    SET(paths ${paths} "${dir}/${swig_name}.h")
  ENDFOREACH(dir)
  FILE(GLOB doc_path ${paths})
  IF(doc_path AND "${class}" MATCHES "^itk::")
    # store the header
    SET(WRAP_ITK_DOC_DOXYGEN_HEADERS ${WRAP_ITK_DOC_DOXYGEN_HEADERS} "${doc_path}")
    # and the produced file
    STRING(REPLACE "::" "_" base_name "${class}")
    SET(page "${CMAKE_CURRENT_BINARY_DIR}/Doc/man3/${base_name}.3")
    SET(WRAP_ITK_DOC_DOXYGEN_PAGES "${WRAP_ITK_DOC_DOXYGEN_PAGES};${page}")
    # and in install the manpage, if requested, and if not yet installed from another dir
    IF(WRAP_ITK_DOC_MAN AND "${WRAP_ITK_DOC_DOXYGEN_INSTALLED_PAGES}" MATCHES "(^|;)${name}(;|$)")
      WRAP_ITK_INSTALL(/Doc/man3 "${page}")
      SET(WRAP_ITK_DOC_DOXYGEN_INSTALLED_PAGES ${WRAP_ITK_DOC_DOXYGEN_INSTALLED_PAGES} "${base_name}.3" CACHE INTERNAL "man pages produced by doxygen and already installed")
    ENDIF(WRAP_ITK_DOC_MAN AND "${WRAP_ITK_DOC_DOXYGEN_INSTALLED_PAGES}" MATCHES "(^|;)${name}(;|$)")
    
    # some simple computations to find the xml file produced for this class
    STRING(REGEX REPLACE "([A-Z])" "_\\1" xmlname ${class})
    STRING(REGEX REPLACE ":" "_1" xmlname ${xmlname})
    STRING(TOLOWER  ${xmlname} xmlname)
    SET(WRAP_ITK_DOC_DOXYGEN_XML_FILES ${WRAP_ITK_DOC_DOXYGEN_XML_FILES} "${CMAKE_CURRENT_BINARY_DIR}/Doc/xml/class${xmlname}.xml")
    
    # the doxy2swig input
    SET(WRAP_ITK_DOC_DOXY2SWIG_INPUT "${WRAP_ITK_DOC_DOXY2SWIG_INPUT}\n${CMAKE_CURRENT_BINARY_DIR}/Doc/xml/class${xmlname}.xml\t${class}")
    SET(WRAP_ITK_DOC_GENERATE_DOXY2SWIG_INPUT ON)
  ENDIF(doc_path AND "${class}" MATCHES "^itk::")
    
ENDMACRO(WRAP_NAMED_CLASS_DOC)

MACRO(ADD_ONE_TYPEDEF_DOC wrap_method wrap_class swig_name template_params)
  IF(WRAP_ITK_DOC_GENERATE_DOXY2SWIG_INPUT)
    SET(WRAP_ITK_DOC_DOXY2SWIG_INPUT "${WRAP_ITK_DOC_DOXY2SWIG_INPUT}\t${swig_name}")
  ENDIF(WRAP_ITK_DOC_GENERATE_DOXY2SWIG_INPUT)
ENDMACRO(ADD_ONE_TYPEDEF_DOC)

###############################################################################
MACRO(END_WRAP_LIBRARY_DOC)
    # create the target doc dir
    SET(library_doc_build_dir "${CMAKE_CURRENT_BINARY_DIR}/Doc") # Library documentation interface files building directory
                                                                 # TODO: direct name of the library dir?
    file(MAKE_DIRECTORY ${library_doc_build_dir})
    
    # configure doxygen input file.
    # be sure to not include a header several times
    UNIQUE(headers "${WRAP_ITK_DOC_DOXYGEN_HEADERS}")
    SET(library_doxygen_config_file ${library_doc_build_dir}/doxygen.config)
    SET(WRAP_ITK_DOC_DOXYGEN_HEADERS_FORMATED)
    FOREACH(header ${headers})
      SET(WRAP_ITK_DOC_DOXYGEN_HEADERS_FORMATED "${WRAP_ITK_DOC_DOXYGEN_HEADERS_FORMATED}           \"${header}\"\\\n")
    ENDFOREACH(header)
    SET(WRAP_ITK_DOC_GENERATE_MAN "NO")
    IF(WRAP_ITK_DOC_MAN)
      SET(WRAP_ITK_DOC_GENERATE_MAN "YES")
    ENDIF(WRAP_ITK_DOC_MAN)
    SET(WRAP_ITK_DOC_LIBRARY_DIR " ${library_doc_build_dir}")
    CONFIGURE_FILE("${WRAP_ITK_DOC_SOURCE_DIR}/doxygen.config.in" 
      "${library_doxygen_config_file}"
      @ONLY IMMEDIATE)

    # which files are produced?
    SET(outputs ${WRAP_ITK_DOC_DOXYGEN_XML_FILES} "${library_doc_build_dir}/xml/combine.xslt") # a file which seem to be produced every times with xml outputs
    IF(WRAP_ITK_DOC_MAN)
      SET(outputs ${outputs} ${WRAP_ITK_DOC_DOXYGEN_PAGES})
    ENDIF(WRAP_ITK_DOC_MAN)

    # run doxygen
    ADD_CUSTOM_COMMAND(
      OUTPUT ${outputs}
      COMMAND "${DOXYGEN_EXECUTABLE}" "${library_doxygen_config_file}"
#      WORKING_DIRECTORY ${WRAP_ITK_DOC_BINARY_DIR}
      DEPENDS ${WRAP_ITK_DOC_DOXYGEN_HEADERS} "${library_doxygen_config_file}"
      COMMENT "-- Wrapping library ${WRAPPER_LIBRARY_NAME}: Constructing documentation xml structure."
    )
    
    ADD_CUSTOM_TARGET(${WRAPPER_LIBRARY_NAME}Doxygen ALL DEPENDS ${outputs} ${WRAP_ITK_DOC_DOCSTRING_FILES})
    
ENDMACRO(END_WRAP_LIBRARY_DOC)

MACRO(WRAP_MODULE_DOC module)
  SET(WRAP_ITK_DOC_DOXY2SWIG_INPUT )  # the c++ name - swig names definitions
ENDMACRO(WRAP_MODULE_DOC)

###############################################################################
# This macro is called once per module
# Global variable WRAPPER_MODULE_NAME can be used
# in the macro to current module name
#
MACRO(END_WRAP_MODULE_DOC)
    SET(doxy2swig_config_file ${CMAKE_CURRENT_BINARY_DIR}/Doc/${WRAPPER_MODULE_NAME}.conf)
    CONFIGURE_FILE("${WRAP_ITK_DOC_SOURCE_DIR}/itk_doxy2swig.conf.in" 
      "${doxy2swig_config_file}"
      @ONLY IMMEDIATE)

    # run itk_doxy2swig
    SET(itk_doxy2swig_py "${WRAP_ITK_DOC_SOURCE_DIR}/itk_doxy2swig.py")
    SET(swig_doc_interface_file ${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/wrap_${WRAPPER_MODULE_NAME}_doc.i)
    ADD_CUSTOM_COMMAND(
    OUTPUT ${swig_doc_interface_file}
    COMMAND ${PYTHON_EXECUTABLE} ${itk_doxy2swig_py} ${doxy2swig_config_file} ${swig_doc_interface_file}
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/Doc/xml/combine.xslt" ${WRAP_ITK_DOC_DOXYGEN_XML_FILES} ${doxy2swig_config_file} ${itk_doxy2swig_py}
#    COMMENT "-- Wrapping library ${WRAPPER_MODULE_NAME}: Generating swig interface for inline documentation."
    )
    SET(WRAP_ITK_DOC_DOCSTRING_FILES ${WRAP_ITK_DOC_DOCSTRING_FILES} ${swig_doc_interface_file})
ENDMACRO(END_WRAP_MODULE_DOC)

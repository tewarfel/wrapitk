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


###############################################################################
# the var to store the file produced by doxygen
SET(WRAP_ITK_DOC_DOXYGEN_INSTALLED_PAGES "" CACHE INTERNAL "man pages produced by doxygen and already installed")


###############################################################################
MACRO(WRAP_LIBRARY_DOC library_name)
  SET(WRAP_ITK_DOC_DOXYGEN_HEADERS )  # doxygen headers to process in this lib
  SET(WRAP_ITK_DOC_DOXYGEN_PAGES )  # pages produced by doxygen in this lib
ENDMACRO(WRAP_LIBRARY_DOC)


###############################################################################
MACRO(WRAP_NAMED_CLASS_DOC class swig_name)
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
  ENDIF(doc_path AND "${class}" MATCHES "^itk::")
ENDMACRO(WRAP_NAMED_CLASS_DOC)

###############################################################################
MACRO(END_WRAP_LIBRARY_DOC)

  # check if we need to generate the doc at all
  STRING(COMPARE NOTEQUAL "${WRAP_ITK_DOC_DOXYGEN_PAGES}" "" headers_exist)
  
  IF(${headers_exist})
  
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
    SET(outputs "${library_doc_build_dir}/xml/combine.xslt") # a file which seem to be produced every times with xml outputs
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
    
    # run itk_doxy2swig
    SET(itk_doxy2swig_py "${WRAP_ITK_DOC_SOURCE_DIR}/itk_doxy2swig.py")
    SET(swig_doc_interface_file ${library_doc_build_dir}/doc.i)
    ADD_CUSTOM_COMMAND(
    OUTPUT ${swig_doc_interface_file}
    COMMAND ${PYTHON_EXECUTABLE} ${itk_doxy2swig_py} ${library_doc_build_dir}/xml ${swig_doc_interface_file}
    DEPENDS "${library_doc_build_dir}/xml/combine.xslt" # xml files
    COMMENT "-- Wrapping library ${WRAPPER_LIBRARY_NAME}: Generating swig interface for inline documentation."
    )
    
    ADD_CUSTOM_TARGET(${WRAPPER_LIBRARY_NAME}Doxygen ALL DEPENDS ${outputs} ${swig_doc_interface_file})

  ENDIF(${headers_exist})
    
ENDMACRO(END_WRAP_LIBRARY_DOC)

###############################################################################
# This macro is called once per module
# Global variable WRAPPER_MODULE_NAME can be used
# in the macro to current module name
#
MACRO(END_WRAP_MODULE_DOC)

ENDMACRO(END_WRAP_MODULE_DOC)

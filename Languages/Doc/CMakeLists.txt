FIND_PACKAGE(Doxygen REQUIRED)

OPTION(WRAP_ITK_DOC_MAN "Generate unix manual pages." OFF)

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
# the var to store the file to be processed by doxygen
SET(WRAP_ITK_DOC_DOXYGEN_HEADERS "" CACHE INTERNAL "doxygen headers to process")

###############################################################################
# the var to store the file produced by doxygen
SET(WRAP_ITK_DOC_DOXYGEN_PAGES "" CACHE INTERNAL "man pages produced by doxygen")

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
    SET(WRAP_ITK_DOC_DOXYGEN_HEADERS ${WRAP_ITK_DOC_DOXYGEN_HEADERS} "${doc_path}" CACHE INTERNAL "doxygen headers to process")
    # and the produced file
    STRING(REPLACE "::" "_" base_name "${class}")
    SET(page "${WRAP_ITK_DOC_BINARY_DIR}/man3/${base_name}.3")
    SET(WRAP_ITK_DOC_DOXYGEN_PAGES "${WRAP_ITK_DOC_DOXYGEN_PAGES};${page}" CACHE INTERNAL "man pages produced by doxygen")
  ENDIF(doc_path AND "${class}" MATCHES "^itk::")
ENDMACRO(WRAP_NAMED_CLASS_DOC)

###############################################################################
MACRO(END_WRAP_LIBRARIES_DOC)
  #
  # configure doxygen input file.
  #
  # be sure to not include a header several times
  UNIQUE(headers "${WRAP_ITK_DOC_DOXYGEN_HEADERS}")
  SET(WRAP_ITK_DOC_DOXYGEN_HEADERS_FORMATED)
  FOREACH(header ${headers})
    SET(WRAP_ITK_DOC_DOXYGEN_HEADERS_FORMATED "${WRAP_ITK_DOC_DOXYGEN_HEADERS_FORMATED}           \"${header}\"\\\n")
  ENDFOREACH(header)
  CONFIGURE_FILE("${WRAP_ITK_DOC_SOURCE_DIR}/doxygen.config.in" 
    "${WRAP_ITK_DOC_BINARY_DIR}/doxygen.config"
    @ONLY IMMEDIATE)

  #
  # install the produced files
  #
  FOREACH(page ${WRAP_ITK_DOC_DOXYGEN_PAGES})
    WRAP_ITK_INSTALL(/Doc/man3 "${page}")
  ENDFOREACH(page)
  
ENDMACRO(END_WRAP_LIBRARIES_DOC)

###############################################################################
MACRO(END_WRAP_LIBRARY_DOC)

  # check if we need to generate the doc at all
  STRING(COMPARE NOTEQUAL "${WRAP_ITK_DOC_DOXYGEN_PAGES}" "" headers_exist)
  
  IF(${headers_exist})
  
    # create the target doc dir
    SET(library_doc_build_dir "${WrapITK_BINARY_DIR}/Libraries/${WRAPPER_LIBRARY_NAME}/Doc"
    CACHE INTERNAL "Library documentation interface files building directory") # TODO: direct name of the library dir?
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
    IF(${WRAP_ITK_DOC_MAN})
      SET(WRAP_ITK_DOC_GENERATE_MAN "YES")
    ENDIF(${WRAP_ITK_DOC_MAN})
    CONFIGURE_FILE("${WRAP_ITK_DOC_SOURCE_DIR}/doxygen.config.in" 
      "${library_doxygen_config_file}"
      @ONLY IMMEDIATE)

    # run doxygen
    ADD_CUSTOM_COMMAND(
      OUTPUT ${WRAP_ITK_DOC_DOXYGEN_PAGES} #!TODO: man pages are not always generated
      COMMAND "${DOXYGEN_EXECUTABLE}" "${library_doxygen_config_file}"
#      WORKING_DIRECTORY ${WRAP_ITK_DOC_BINARY_DIR}
      DEPENDS ${WRAP_ITK_DOC_DOXYGEN_HEADERS}
      COMMENT "-- Wrapping library ${WRAPPER_LIBRARY_NAME}: Constructing documentation xml structure."
    )
    
    # run itk_doxy2swig
    SET(itk_doxy2swig_py "${WRAP_ITK_DOC_SOURCE_DIR}/itk_doxy2swig.py")
    SET(swig_doc_interface_file ${library_doc_build_dir}/doc.i)
    ADD_CUSTOM_COMMAND(
    OUTPUT ${swig_doc_interface_file}
    COMMAND ${PYTHON_EXECUTABLE} ${itk_doxy2swig_py} ${library_doc_build_dir}/xml ${swig_doc_interface_file}
#    DEPENDS # xml files
    COMMENT "-- Wrapping library ${WRAPPER_LIBRARY_NAME}: Generating swig interface for inline documentation."
    )
    
    ADD_CUSTOM_TARGET(Doxygen ALL DEPENDS ${WRAP_ITK_DOC_DOXYGEN_PAGES} ${swig_doc_interface_file})

    # install the produced manual pages if necessary
    IF(${WRAP_ITK_DOC_MAN})
      FOREACH(page ${WRAP_ITK_DOC_DOXYGEN_PAGES})
        WRAP_ITK_INSTALL(/Doc/man3 "${page}")
      ENDFOREACH(page)
    ENDIF(${WRAP_ITK_DOC_MAN})
    
    # reset for the next library iteration
    SET(WRAP_ITK_DOC_DOXYGEN_PAGES "" CACHE INTERNAL "man pages produced by doxygen")
  
  ENDIF(${headers_exist})
    
ENDMACRO(END_WRAP_LIBRARY_DOC)

###############################################################################
# This macro is called once per module
# Global variable WRAPPER_MODULE_NAME can be used
# in the macro to current module name
#
MACRO(END_WRAP_MODULE_DOC)

ENDMACRO(END_WRAP_MODULE_DOC)

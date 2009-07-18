WRAP_LIBRARY(VXLNumerics)

SET(WRAPPER_LIBRARY_GROUPS
  vcl_complex
  vnl_matrix
  vnl_vector
  vnl_unary_function
)

SET(WRAPPER_LIBRARY_LINK_LIBRARIES ITKCommon)

# still include the default include, to avoid a compilation error
# with exceptions on sun studio
## don't include all the default include for that module
# SET(include_save ${WRAPPER_DEFAULT_INCLUDE})
# SET(WRAPPER_DEFAULT_INCLUDE)

AUTO_INCLUDE_MODULES() 

# restore the default values for the other modules
# SET(WRAPPER_DEFAULT_INCLUDE ${include_save})



END_WRAP_LIBRARY()


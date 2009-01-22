WRAP_LIBRARY(FFT)

SET(WRAPPER_LIBRARY_DEPENDS VXLNumerics Base)

# find needed libs
SET(libs ITKCommon ITKAlgorithms)

IF(WRAP_complex_float AND WRAP_float)
  IF(FFTWF_LIB)
    SET(libs ${libs} ${FFTWF_LIB})
  ENDIF(FFTWF_LIB)
ENDIF(WRAP_complex_float AND WRAP_float)

IF(WRAP_complex_double AND WRAP_double)
  IF(FFTWD_LIB)
    SET(libs ${libs} ${FFTWD_LIB})
  ENDIF(FFTWD_LIB)
ENDIF(WRAP_complex_double AND WRAP_double)

SET(WRAPPER_LIBRARY_LINK_LIBRARIES ${libs})

AUTO_INCLUDE_MODULES()
END_WRAP_LIBRARY()



INCLUDE(FindFFTW.cmake)

WRAP_LIBRARY("FFTW")
SET(WRAPPER_LIBRARY_DEPENDS VXLNumerics Base FFT)

# find needed libs
SET(libs ITKCommon ITKAlgorithms)

IF(WRAP_complex_float AND WRAP_float)
  IF(FFTWF_LIB)
    SET(libs ${libs} ${FFTWF_LIB})
  ELSE(FFTWF_LIB)
    MESSAGE("Warning: FFTWF not found automatically, some or all FFTW filters will not be wrapped. Set FFTWF parameters manually and try again.")
  ENDIF(FFTWF_LIB)
ENDIF(WRAP_complex_float AND WRAP_float)

IF(WRAP_complex_double AND WRAP_double)
  IF(FFTWD_LIB)
    SET(libs ${libs} ${FFTWD_LIB})
  ELSE(FFTWD_LIB)
    MESSAGE("Warning: FFTWD not found automatically, some or all FFTW filters will not be wrapped. Set FFTWD parameters manually and try again.")
  ENDIF(FFTWD_LIB)
ENDIF(WRAP_complex_double AND WRAP_double)

SET(WRAPPER_LIBRARY_LINK_LIBRARIES ${libs})

AUTO_INCLUDE_MODULES()
END_WRAP_LIBRARY()



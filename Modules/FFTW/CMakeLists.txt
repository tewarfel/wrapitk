SET(CMAKE_MODULE_PATH ${ITK_SOURCE_DIR}/CMake) # Path to FINDFFTW

WRAP_LIBRARY("FFTW")
SET(WRAPPER_LIBRARY_DEPENDS Base FFT)

# find needed libs
SET(libs ITKCommon ITKAlgorithms)

IF(WRAP_complex_float AND WRAP_float)
  OPTION(USE_FFTWF "Use single precision FFTW if found" ON)
  FIND_PACKAGE(FFTW)
  IF(FFTWF_LIB)
    SET(libs ${libs} ${FFTWF_LIB})
  ELSE(FFTWF_LIB)
    MESSAGE("Warning: FFTWF not found automatically, some or all FFTW filters will not be wrapped. Set FFTWF parameters manually and try again.")
  ENDIF(FFTWF_LIB)
ENDIF(WRAP_complex_float AND WRAP_float)

IF(WRAP_complex_double AND WRAP_double)
  OPTION(USE_FFTWD "Use double precision FFTW if found" ON)
  FIND_PACKAGE(FFTW)
  IF(FFTWD_LIB)
    SET(libs ${libs} ${FFTWD_LIB})
  ELSE(FFTWD_LIB)
    MESSAGE("Warning: FFTWD not found automatically, some or all FFTW filters will not be wrapped. Set FFTWD parameters manually and try again.")
  ENDIF(FFTWD_LIB)
ENDIF(WRAP_complex_double AND WRAP_double)

SET(WRAPPER_LIBRARY_LINK_LIBRARIES ${libs})

WRAPPER_LIBRARY_CREATE_WRAP_FILES()
END_WRAP_LIBRARY()



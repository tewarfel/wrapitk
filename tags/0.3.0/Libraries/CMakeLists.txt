  
SET(WRAP_ITK_LIBRARIES
  VXLNumerics
  Base
)
  
# set Iterators library to OFF by default
# it is currently usable only when applying patches to ITK, and will only
# take place and time to build
# TODO: remove this line when iterators will be usable without patch
OPTION(WRAP_Iterators "Build Iterators library." OFF)
  
# Set FFTW to on if USE_FFTWF or USE_FFTWD are on.
IF(USE_FFTWD OR USE_FFTWF)
  OPTION(WRAP_FFTW "Build FFTW library." ON)
ELSE(USE_FFTWD OR USE_FFTWF)
  OPTION(WRAP_FFTW "Build FFTW library." OFF)
ENDIF(USE_FFTWD OR USE_FFTWF)

# ItkVtk is very optional, it MUST be OFF by default
# TODO: move it to external project
OPTION(WRAP_ItkVtk "Build ITK-VTK glue library." OFF)

SET(optional_libraries 
  Numerics
  Transforms
  SpatialObject
  BinaryMorphology
  Calculators
  Compose
  Interpolators  # is required by DeformableTransforms
  DeformableTransforms
  Denoising
  DistanceMap
  EdgesAndContours
  FFT
  FFTW
  Filtering
  IntensityFilters
  IO
  Iterators
  LevelSet
  Morphology
  #Patented
  PixelMath
  Resize  # required by Registration
  Registration
  Review
  SegmentationAndThreshold
  SegmentationValidation
  SimpleFilters
  UnaryPixelMath
  ItkVtk
)

FOREACH(library ${optional_libraries})
  OPTION(WRAP_${library} "Build ${library} library." ON)
  MARK_AS_ADVANCED(WRAP_${library})
  IF(WRAP_${library})
    SET(WRAP_ITK_LIBRARIES ${WRAP_ITK_LIBRARIES} ${library})
  ENDIF(WRAP_${library})
ENDFOREACH(library)
SET(WRAP_ITK_LIBRARIES ${WRAP_ITK_LIBRARIES} CACHE INTERNAL "Internal library list.")

FOREACH(library ${WRAP_ITK_LIBRARIES} ${WRAP_ITK_EXTRA_LIBRARIES})
  ADD_SUBDIRECTORY(${library})
ENDFOREACH(library)

WRAP_LIBRARY(Numerics)

SET(WRAPPER_LIBRARY_GROUPS
  ITKCostFunctions
  itkDenseFrequencyContainer
  itkSample
  itkHistogram
)

SET(WRAPPER_LIBRARY_DEPENDS VXLNumerics Base VXLNumerics)

SET(WRAPPER_LIBRARY_LINK_LIBRARIES ITKCommon ITKNumerics ITKStatistics)

AUTO_INCLUDE_MODULES()
END_WRAP_LIBRARY()


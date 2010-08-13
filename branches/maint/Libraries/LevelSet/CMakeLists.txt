WRAP_LIBRARY(LevelSet)

SET(WRAPPER_LIBRARY_GROUPS
  itkLevelSetFunction
  itkSparseFieldLevelSetImageFilter
  itkSparseFieldFourthOrderLevelSetImageFilter
  itkSegmentationLevelSetImageFilter
  itkShapePriorSegmentationLevelSetImageFilter
  itkFastMarchingImageFilter
)

SET(WRAPPER_LIBRARY_DEPENDS VXLNumerics Base Numerics)

SET(WRAPPER_LIBRARY_LINK_LIBRARIES ITKCommon)

AUTO_INCLUDE_MODULES()
END_WRAP_LIBRARY()


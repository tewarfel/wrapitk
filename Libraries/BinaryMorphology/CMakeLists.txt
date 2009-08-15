WRAP_LIBRARY(BinaryMorphology)

SET(WRAPPER_LIBRARY_DEPENDS VXLNumerics Base Transforms Numerics)

SET(WRAPPER_LIBRARY_GROUPS
  itkLabelObjectLine
  itkLabelObject
  itkShapeLabelObject
  itkStatisticsLabelObject
  itkLabelMap
  ITKLabelMapBase
  itkLabelMapFilter
  itkInPlaceLabelMapFilter
  itkChangeRegionLabelMapFilter
)
		  
SET(WRAPPER_LIBRARY_LINK_LIBRARIES ITKCommon ITKStatistics)

AUTO_INCLUDE_MODULES()
END_WRAP_LIBRARY()


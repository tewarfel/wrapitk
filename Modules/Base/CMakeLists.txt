WRAP_LIBRARY(Base)

SET(WRAPPER_LIBRARY_GROUPS
  itkEventObjects
  ITKCommonBase
  itkOffset
  itkArray
  itkFixedArray
  itkIndex
  itkSize
  itkVector
  itkPoint
  itkDefaultDynamicMeshTraits
  itkDefaultStaticMeshTraits
  itkLevelSet
  itkVectorContainer
  itkMapContainer
  itkBoundingBox
  itkPointSet
  itkContinuousIndex
  itkCovariantVector
  itkRGBPixel
  itkVariableLengthVector
  ITKRegions
  itkMatrix
  itkImage
  itkVectorImage
  itkNeighborhood
#  SwigExtras
  itkImageSource
  itkImageToImageFilterA
  itkImageToImageFilterB
  itkInPlaceImageFilterA
  itkInPlaceImageFilterB
  itkFiniteDifferenceFunction
  itkFiniteDifferenceImageFilter
)

SET(WRAPPER_LIBRARY_DEPENDS VXLNumerics)

SET(WRAPPER_LIBRARY_LINK_LIBRARIES ITKCommon ITKNumerics)

WRAPPER_LIBRARY_CREATE_WRAP_FILES()

END_WRAP_LIBRARY()




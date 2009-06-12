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
  itkContinuousIndex
  itkVectorContainer
  itkMapContainer
  itkBoundingBox
  itkPointSet
  itkCovariantVector
  itkRGBPixel
  itkRGBAPixel
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
  itkPath
  itkParametricPath
  itkPolyLineParametricPath
)

SET(WRAPPER_LIBRARY_DEPENDS VXLNumerics)

SET(WRAPPER_LIBRARY_LINK_LIBRARIES ITKCommon ITKNumerics)

AUTO_INCLUDE_MODULES()

END_WRAP_LIBRARY()




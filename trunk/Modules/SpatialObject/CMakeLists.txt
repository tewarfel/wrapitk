WRAP_LIBRARY(SpatialObject)

SET(WRAPPER_LIBRARY_GROUPS
  itkSpatialObjectProperty
  itkAffineGeometryFrame
  itkSpatialObjectBase
  itkSpatialObjectPoint
  itkContourSpatialObjectPoint
  itkPointBasedSpatialObject
  itkImageSpatialObject
  itkBlobSpatialObject
  itkPolygonSpatialObject
)

SET(WRAPPER_LIBRARY_DEPENDS VXLNumerics Base Transforms Calculators)

SET(WRAPPER_LIBRARY_LINK_LIBRARIES ITKCommon ITKSpatialObject)

WRAPPER_LIBRARY_CREATE_WRAP_FILES()
END_WRAP_LIBRARY()


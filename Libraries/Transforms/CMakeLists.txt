WRAP_LIBRARY(Transforms)

SET(WRAPPER_LIBRARY_GROUPS
  itkTransform
  itkMatrixOffsetTransformBase
  ITKRigidTransforms
  ITKEulerTransform
  itkVersorTransform
  ITKVersorRigidTransform
  ITKSimilarityTransforms
  itkAffineTransform
  itkScalableAffineTransform
  itkScaleTransform
)

SET(WRAPPER_LIBRARY_DEPENDS VXLNumerics Base)

SET(WRAPPER_LIBRARY_LINK_LIBRARIES ITKCommon)

AUTO_INCLUDE_MODULES()
END_WRAP_LIBRARY()

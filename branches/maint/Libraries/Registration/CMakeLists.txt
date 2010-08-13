WRAP_LIBRARY(Registration)

SET(WRAPPER_LIBRARY_GROUPS
  itkPDEDeformableRegistrationFilter
  itkPDEDeformableRegistrationFunction
  itkESMDemonsRegistrationFunction
  itkDisplacementFieldJacobianDeterminantFilter
)

SET(WRAPPER_LIBRARY_DEPENDS 
  VXLNumerics 
  Base 
  Numerics
  Transforms
  SpatialObject
  Resize
  Calculators
)

SET(WRAPPER_LIBRARY_LINK_LIBRARIES ITKCommon ITKNumerics)

AUTO_INCLUDE_MODULES()
END_WRAP_LIBRARY()


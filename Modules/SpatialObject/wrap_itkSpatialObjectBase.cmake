WRAP_NON_TEMPLATE_CLASS("itk::MetaEvent")

WRAP_CLASS("itk::SpatialObject" POINTER)
  FOREACH(d ${WRAP_ITK_DIMS})
    WRAP_TEMPLATE("${d}" "${d}")
  ENDFOREACH(d)
END_WRAP_CLASS()

# the file must have a different name than the class name for java
# that's why it is here

# only available for dim 3

FILTER_DIMS(d 3)
IF(d)
  WRAP_NON_TEMPLATE_CLASS("itk::CylinderSpatialObject" POINTER)
ENDIF(d)

#
# hack for ::MetaEvent, the super class of itk::MetaEvent
#
SET(WRAPPER_AUTO_INCLUDE_HEADERS OFF)
WRAP_NAMED_CLASS("MetaEvent" "itkMetaEvent_Superclass")
  WRAP_CLASS_ALL_LANGUAGES()
  # to avoid useless warning: no template can be defined in 
  SET(WRAPPER_WARN_ABOUT_NO_TEMPLATE OFF)
  ADD_ONE_TYPEDEF("FORCE_INSTANTIATE" "${WRAPPER_CLASS}" "${WRAPPER_SWIG_NAME}")
END_WRAP_CLASS()
# WRAP_NON_TEMPLATE_CLASS_ALL_LANGUAGES("MetaEvent")


# must be in the same file than SpatialObject for java wrappers

WRAP_CLASS("itk::TreeNode" POINTER)
  FOREACH(d ${WRAP_ITK_DIMS})
    WRAP_TEMPLATE("${ITKM_SO${d}}" "${ITKT_SO${d}}*")
  ENDFOREACH(d)
END_WRAP_CLASS()

WRAP_CLASS("itk::SpatialObjectTreeNode" POINTER)
  SET(WRAPPER_TEMPLATES "${itk_Wrap_SpatialObject}")
END_WRAP_CLASS()

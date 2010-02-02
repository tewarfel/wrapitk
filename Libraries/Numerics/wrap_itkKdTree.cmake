WRAP_INCLUDE("itkListSample.h")

WRAP_CLASS("itk::Statistics::KdTree" POINTER)
  FOREACH(d ${WRAP_ITK_DIMS})
    WRAP_TEMPLATE("LS${ITKM_VF${d}}"  "itk::Statistics::ListSample< ${ITKT_VF${d}} >")
  ENDFOREACH(d)
END_WRAP_CLASS()

SET(WRAPPER_AUTO_INCLUDE_HEADERS OFF)
WRAP_CLASS("itk::Statistics::KdTreeNode")
  FOREACH(d ${WRAP_ITK_DIMS})
    WRAP_TEMPLATE("LS${ITKM_VF${d}}"  "itk::Statistics::ListSample< ${ITKT_VF${d}} >")
  ENDFOREACH(d)
END_WRAP_CLASS()

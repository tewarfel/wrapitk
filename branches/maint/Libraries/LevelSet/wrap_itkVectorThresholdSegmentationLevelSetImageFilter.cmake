
WRAP_INCLUDE("itkShapeDetectionLevelSetImageFilter.h")

WRAP_CLASS("itk::VectorThresholdSegmentationLevelSetImageFilter" POINTER)

  FOREACH(d ${WRAP_ITK_DIMS})
    FOREACH(t ${WRAP_ITK_REAL})
      FOREACH(t2 ${WRAP_ITK_VECTOR_REAL})
        WRAP_TEMPLATE("${ITKM_I${t}${d}}${ITKM_I${t2}${d}${d}}${ITKM_${t}}" "${ITKT_I${t}${d}},${ITKT_I${t2}${d}${d}},${ITKT_${t}}")
      ENDFOREACH(t2)
    ENDFOREACH(t)
  ENDFOREACH(d)

END_WRAP_CLASS()

WRAP_CLASS("itk::MultiScaleHessianBasedMeasureImageFilter" POINTER)
  foreach(d ${WRAP_ITK_DIMS})
    foreach(t ${WRAP_ITK_SCALAR})
      WRAP_TEMPLATE("${ITKM_I${t}${d}}${ITKM_ISSRTD${d}${d}}${ITKM_I${t}${d}}" "${ITKT_I${t}${d}}, ${ITKT_ISSRTD${d}${d}}, ${ITKT_I${t}${d}}")
    endforeach(t)
  endforeach(d)
END_WRAP_CLASS()
WRAP_CLASS("itk::PDEDeformableRegistrationFunction" POINTER)
  FOREACH(s ${WRAP_ITK_SCALAR})
    WRAP_IMAGE_FILTER_COMBINATIONS("${s}" "${s}" "${WRAP_ITK_VECTOR_REAL}" 2+)
  ENDFOREACH(s)  
END_WRAP_CLASS()

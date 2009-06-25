# auto include feature must be disable because the class is not in the file
# with the same name
SET(WRAPPER_AUTO_INCLUDE_HEADERS OFF)
WRAP_INCLUDE("vnl/vnl_vector_ref.h")

WRAP_CLASS("vnl_vector_ref")
  UNIQUE(vector_types "D;F;SI;SL;LD;SC;UC;US;UI;UL;${WRAP_ITK_SCALAR}")
  FOREACH(t ${vector_types})
    WRAP_TEMPLATE("${ITKM_${t}}"  "${ITKT_${t}}")
  ENDFOREACH(t)

  # not sure why we didn't use the standard names - keep them anyway
  WRAP_TEMPLATE("_vcl_complex${ITKM_D}" "vcl_complex<${ITKT_D}>")
  WRAP_TEMPLATE("_vcl_complex${ITKM_F}" "vcl_complex<${ITKT_F}>")
  WRAP_TEMPLATE("_vcl_complex${ITKM_LD}" "vcl_complex<${ITKT_LD}>")
END_WRAP_CLASS()

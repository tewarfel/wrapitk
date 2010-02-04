WRAP_CLASS("itk::Statistics::Histogram" POINTER)
  SET(WRAPPER_TEMPLATES "${itk_Wrap_Histogram}")
END_WRAP_CLASS()

# also wrap the decorator in the same file to avoid file collision with the
# one in Base
WRAP_CLASS("itk::SimpleDataObjectDecorator" POINTER)
  IF(ITK_USE_REVIEW_STATISTICS)
    WRAP_TEMPLATE("${ITKM_HF}"  "${ITKT_HF} *")
    WRAP_TEMPLATE("${ITKM_HD}"  "${ITKT_HD} *")
  ELSE(ITK_USE_REVIEW_STATISTICS)
    UNIQUE(dims "1;2;${WRAP_ITK_DIMS}")                    
    FOREACH(d ${dims})
      WRAP_TEMPLATE("${ITKM_HF${d}}"  "${ITKT_HF${d}} *")
      WRAP_TEMPLATE("${ITKM_HD${d}}"  "${ITKT_HD${d}} *")
    ENDFOREACH(d)
  ENDIF(ITK_USE_REVIEW_STATISTICS)
END_WRAP_CLASS()
		  

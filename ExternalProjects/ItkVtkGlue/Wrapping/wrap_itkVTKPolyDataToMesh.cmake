WRAP_INCLUDE("itkDefaultStaticMeshTraits.h")

WRAP_CLASS("itk::VTKPolyDataToMesh" POINTER)
  FILTER_DIMS(d3 3)
  FOREACH(d ${d3})
    WRAP_TEMPLATE("M${ITKM_D}${d}S" "itk::Mesh< ${ITKT_D},${d},itk::DefaultStaticMeshTraits< ${ITKT_D},${d},${d},${ITKT_D} > >")
  ENDFOREACH(d)
END_WRAP_CLASS()

WRAP_INCLUDE("itkMesh.h")
WRAP_INCLUDE("itkQuadEdgeMeshTraits.h")

WRAP_CLASS("itk::ImageToMeshFilter" POINTER)
  FOREACH(d ${WRAP_ITK_DIMS})
    WRAP_TEMPLATE("${ITKM_IUC${d}}MD${d}Q" "${ITKT_IUC${d}}, itk::Mesh< ${ITKT_D},${d},itk::QuadEdgeMeshTraits< ${ITKT_D},${d},${ITKT_B},${ITKT_B},${ITKT_F},${ITKT_F} > >")
  ENDFOREACH(d)
END_WRAP_CLASS()                              

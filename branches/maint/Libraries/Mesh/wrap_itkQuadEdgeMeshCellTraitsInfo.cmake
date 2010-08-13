# this class is define in a header with a different name
WRAP_INCLUDE("itkQuadEdgeCellTraitsInfo.h")
SET(WRAPPER_AUTO_INCLUDE_HEADERS OFF)

WRAP_CLASS("itk::QuadEdgeMeshCellTraitsInfo")
  FOREACH(d ${WRAP_ITK_DIMS})
    WRAP_TEMPLATE("${d}" "${d}")
  ENDFOREACH(d)
END_WRAP_CLASS()


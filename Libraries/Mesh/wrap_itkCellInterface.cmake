WRAP_INCLUDE("itkQuadEdgeCellTraitsInfo.h")

WRAP_CLASS("itk::CellInterface" AUTOPOINTER)
  FOREACH(d ${WRAP_ITK_DIMS})
    WRAP_TEMPLATE("DQEMCTI${d}" "double, itk::QuadEdgeMeshCellTraitsInfo< ${d} >")
  ENDFOREACH(d)
END_WRAP_CLASS()

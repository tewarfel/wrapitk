WRAP_LIBRARY(Mesh)

SET(WRAPPER_LIBRARY_DEPENDS Base)

SET(WRAPPER_LIBRARY_GROUPS
  itkDefaultDynamicMeshTraits
  itkDefaultStaticMeshTraits
  itkQuadEdge
  itkGeometricalQuadEdge
  itkQuadEdgeMeshPoint
  itkQuadEdgeMeshTraits
  itkQuadEdgeMeshCellTraitsInfo
  itkCellInterface
  itkQuadEdgeMeshBase
  itkQuadEdgeMeshLineCell
  itkPointSet
  itkMesh
  itkQuadEdgeMesh
  itkMeshSource
  itkImageToMeshFilter
)
  
SET(WRAPPER_LIBRARY_LINK_LIBRARIES ITKCommon ITKQuadEdgeMesh)

AUTO_INCLUDE_MODULES()
END_WRAP_LIBRARY()



#ifndef _itkImageFunctionSpace
#define _itkImageFunctionSpace

#include <dolfin/Function.h>
#include <dolfin/FunctionSpace.h>
#include <dolfin/fem/FiniteElement.h>
#include <dolfin/mesh/UnitSquare.h>
#include <dolfin/mesh/UnitCube.h>

#using namespace dolfin;

namespace itk
{


class ImageFunctionSpace : public FunctionSpace
{
public:
  ImageFunctionSpace(const ImageType* & imageData, const FiniteElement& element,
      const DofMap& dofmap)
  {
    if(!imageData)
    {
      throw std::runtime_error("Input image is null.");
    }
    if ((ImageDimension < 2) || (ImageDimension > 3))
    {
      throw std::runtime_error("Input image dimension must be 2 or 3.");
    }

    PixelType * buffer = const_cast<PixelType *> (image->GetBufferPointer());
    m_ImageData = (double *) (buffer);

    m_ImageSize = image->GetBufferedRegion().GetSize();

    dolfin::Mesh mesh;
    if(ImageDimension == 2)
    {
      mesh = UnitSquare(m_ImageSize[0], m_ImageSize[1]);
    }
    else if(ImageDimension == 3)
    {
      mesh = UnitCube(m_ImageSize[0], m_ImageSize[1], m_ImageSize[2]);
    }
    FunctionSpace(mesh, element, dofmap);
  };

  void eval(double* values, const double* x, const Function& v) const;
  {
    if(ImageDimension == 2)
    {
      int i = int((m_ImageSize[0] - 1) * x[0]);
      int j = int((m_ImageSize[1] - 1) * x[1]);
      int index = i + (m_ImageSize[0] * j);
      values[0] = image[index];
    }
    else if(ImageDimension == 3)
    {
      int i = int((m_ImageSize[0] - 1) * x[0]);
      int j = int((m_ImageSize[1] - 1) * x[1]);
      int k = int((m_ImageSize[2] - 1) * x[2]);
      int index = i + (m_ImageSize[0] * j) + (m_ImageSize[0] * m_ImageSize[1] * k);
      values[0] = image[index];
    }
  }

protected:
  double *m_ImageData;
  int[ImageDimension] m_ImageSize;
};

} // namespace itk

#endif // _itkImageFunctionSpace

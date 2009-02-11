#ifndef _itkImageToDolfinFunction_txx
#define _itkImageToDolfinFunction_txx

#include "itkImageToDolfinFunction.h"
#include "itkImageFunctionSpace.h"

namespace itk
{

template<class TImage>
Function DolfinFunction<TImage>::GetOutput()
{
	FiniteElement element("CG");
	DofMap dofmap; // How to initialise this?

	ImageFunctionSpace ifs(m_Image, element, dofmap);
	dolphin::Function func(ifs);

	return func;
}

template<class TImage>
void DolfinFunction<TImage>::Update()
{
	m_Image->Update();
}

}

} // namespace itk

#endif


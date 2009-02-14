#ifndef _itkDolfinImageFunction_h
#define _itkDolfinImageFunction_h

#if !defined(CABLE_CONFIGURATION)
#include <dolfin/function/Function.h>
#include <dolfin/function/FunctionSpace.h>
#include <dolfin/mesh/Mesh.h>
#include <dolfin/fem/FiniteElement.h>
#include <dolfin/fem/DofMap.h>
#else
namespace dolfin
{
	class Function {};
	class FunctionSpace {};
	class Mesh {};
	class FiniteElement {};
	class DofMap {};
};
#endif //!defined(CABLE_CONFIGURATION)

namespace itk
{

	/** \Class DolfinImageFunction
	 *  \brief Helper class for itk::ImageToDolfinFunction
	 *
	 *	\author Ali Tonddast-Navaei
	 *	\author Gaetan Lehman
	 */

//	class ImageFunctionSpace : public dolfin::FunctionSpace
//	{
//	public:
//		typedef typename std::tr1::shared_ptr<const dolfin::Mesh> MeshPointerType;
//		typedef typename std::tr1::shared_ptr<const dolfin::FiniteElement> ElementPointerType;
//		typedef typename std::tr1::shared_ptr<const DofMap> DofMapPointerType;
//
//		ImageFunctionSpace(const dolfin::Mesh& mesh)
//		: dolfin::FunctionSpace(MeshPointerType(&mesh, dolfin::NoDeleter<const dolfin::Mesh>()),
//				ElementPointerType element, DofMapPointerType dofmap)
//		{
//			// Do nothing
//		}
//
//	};

	template <typename TImage>
	class DolfinImageFunction : public dolfin::Function
	{
	public:
		typedef TImage ImageType;
		typedef typename ImageType::PixelType PixelType;
		typedef typename ImageType::SizeType SizeType;

//		typedef typename std::tr1::shared_ptr<const dolfin::Mesh> MeshPointerType;
//		typedef typename std::tr1::shared_ptr<const dolfin::FiniteElement> ElementPointerType;
//		typedef typename std::tr1::shared_ptr<const DofMap> DofMapPointerType;

		/** Image dimension. */
		itkStaticConstMacro(ImageDimension, unsigned int, ImageType::ImageDimension);

	protected:
		double *m_ImageData;
		SizeType m_ImageSize;

	public:
		DolfinImageFunction(ImageType* imageData, dolfin::FunctionSpace& V) :
			dolfin::Function(V)
		{
			if(!imageData)
			{
				throw std::runtime_error("Input image is null.");
			}
			if ((ImageDimension < 2) || (ImageDimension > 3))
			{
				throw std::runtime_error("Input image dimension must be 2 or 3.");
			}

			PixelType *buffer = const_cast<PixelType *> (imageData->GetBufferPointer());
			m_ImageData = (double *) (buffer);
			m_ImageSize = imageData->GetBufferedRegion().GetSize();
		};

		DolfinImageFunction(const DolfinImageFunction &v)
		{
			std::cerr << "copy ctor\n";
			 *this = v;
		}

		void eval(double* values, const double* x) const
		{
			if(ImageDimension == 2)
			{
				int i = int((m_ImageSize[0] - 1) * x[0]);
				int j = int((m_ImageSize[1] - 1) * x[1]);
				int index = i + (m_ImageSize[0] * j);
				values[0] = m_ImageData[index];
			}
			else if(ImageDimension == 3)
			{
				int i = int((m_ImageSize[0] - 1) * x[0]);
				int j = int((m_ImageSize[1] - 1) * x[1]);
				int k = int((m_ImageSize[2] - 1) * x[2]);
				int index = i + (m_ImageSize[0] * j) + (m_ImageSize[0] * m_ImageSize[1] * k);
				values[0] = m_ImageData[index];
			}
		};
	};

} // namespace itk

#endif // _itkDolfinImageFunction_h

#ifndef _itkDolfinImageFunctionSpace_h
#define _itkDolfinImageFunctionSpace_h

#if !defined(CABLE_CONFIGURATION)
#include <dolfin/function/FunctionSpace.h>
#include <dolfin/mesh/Mesh.h>
#include <dolfin/fem/FiniteElement.h>
#include <dolfin/fem/DofMap.h>

#include <memory>
//#include <boost/shared_ptr.hpp>
#else
namespace std
{
namespace tr1
{
	template<typename T> class shared_ptr
	{
		 T& operator*() const;
	};
}
};
namespace dolfin
{
	class FunctionSpace {};
	class Function
	{
		Function(std::tr1::shared_ptr<const dolfin::FunctionSpace>);
	};
	class Mesh {};
	class FiniteElement
	{
		FiniteElement(std::string);
	};
	class DofMap
	{
		DofMap(std::string, Mesh);
	};
};
#endif //!defined(CABLE_CONFIGURATION)

namespace itk
{

	/** \Class DolfinImageFunctionSpace
	 *  \brief Helper class for itk::ImageToDolfinFunction
	 *
	 *	\author Ali Tonddast-Navaei
	 *	\author Gaetan Lehman
	 */

	template <typename TImage>
	class DolfinImageFunctionSpace : public dolfin::FunctionSpace
	{
	public:
		typedef TImage ImageType;
		typedef typename ImageType::PixelType PixelType;
		typedef typename ImageType::SizeType SizeType;

		typedef typename std::tr1::shared_ptr<const dolfin::Mesh> MeshConstPointerType;
		typedef typename std::tr1::shared_ptr<const dolfin::FiniteElement> ElementConstPointerType;
		typedef typename std::tr1::shared_ptr<const dolfin::DofMap> DofMapConstPointerType;

		/** Image dimension. */
		itkStaticConstMacro(ImageDimension, unsigned int, ImageType::ImageDimension);

	protected:
		double *m_ImageData;
		SizeType m_ImageSize;

	public:
				DolfinImageFunctionSpace(ImageType* imageData,
						MeshConstPointerType mesh,
						ElementConstPointerType element,
						DofMapConstPointerType dofmap) :
				dolfin::FunctionSpace(mesh, element, dofmap)
				{
					if(!imageData)
					{
						throw std::runtime_error("Input image is null.");
					}
					if ((ImageDimension < 2) || (ImageDimension> 3))
					{
						throw std::runtime_error("Input image dimension must be 2 or 3.");
					}

					PixelType *buffer = const_cast<PixelType *> (imageData->GetBufferPointer());
					m_ImageData = (double *) (buffer);
					m_ImageSize = imageData->GetBufferedRegion().GetSize();
				};

//		DolfinImageFunctionSpace(ImageType* imageData) :
//		dolfin::FunctionSpace(
//				typename IFSType::MeshType(new dolfin::Mesh()),
//				typename IFSType::ElementType(new dolfin::FiniteElement(elemSig)),
//				typename IFSType::DofMapType(new dolfin::DofMap(dofSig, mesh)) )
//		{
//			if(!imageData)
//			{
//				throw std::runtime_error("Input image is null.");
//			}
//			if ((ImageDimension < 2) || (ImageDimension> 3))
//			{
//				throw std::runtime_error("Input image dimension must be 2 or 3.");
//			}
//
//			PixelType *buffer = const_cast<PixelType *> (imageData->GetBufferPointer());
//			m_ImageData = (double *) (buffer);
//			m_ImageSize = imageData->GetBufferedRegion().GetSize();
//		};

		void eval(double* values, const double* x, const dolfin::Function& v) const
		{
			std::cerr << "evaluating image discrete function ...\n";
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

#endif // _itkDolfinImageFunctionSpace_h

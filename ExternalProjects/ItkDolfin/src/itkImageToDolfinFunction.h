#ifndef _itkImageToDolfinFunction_h
#define _itkImageToDolfinFunction_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkImportImageFilter.h"

#include "itkDolfinImageFunction.h"
#if !defined(CABLE_CONFIGURATION)
#include <dolfin/function/Function.h>
#include <dolfin/mesh/UnitSquare.h>
#include <dolfin/mesh/UnitCube.h>
#else
namespace dolfin
{
	class UnitSquare {};
	class UnitCube {};
};
namespace itk
{
	template<typename TImage> class DolfinImageFunctionSpace {};
};
#endif //!defined(CABLE_CONFIGURATION)

namespace itk
{

	/** \Class DolfinFunctionToImage
	 *  \brief itk::Image to dolfin::Function data structure conversion
	 *
	 *  ITK image can be converted to piecewise constant
	 *  discrete function on a simplicial triangulation mesh.
	 *
	 *  An itk::Image of any pixel type is casted to \code double \endcode type and
	 *  when dolfin::Function is converted to an itk::Image, the output is of \code double \endcode
	 *  type. Only 2 and 3 dimensional images are supported. An arbitrary
	 *  finite element can be associated, the default is 'CG'.
	 *
	 *	\author Ali Tonddast-Navaei
	 *	\author Gaetan Lehman
	 */

	template <typename TImage>
	class ImageToDolfinFunction
	{
	public:
		///! Standard "Self" typedef.
		typedef ImageToDolfinFunction Self;

		typedef TImage ImageType;
		typedef typename ImageType::SizeType SizeType;

		/** Image dimension. */
		itkStaticConstMacro(ImageDimension, unsigned int, ImageType::ImageDimension);

		static DolfinImageFunction< TImage > Convert(ImageType *imageData)
		{
			SizeType imageSize = imageData->GetBufferedRegion().GetSize();

			dolfin::Mesh mesh;
			std::string elemSig;
			std::string dofSig;
			if(ImageDimension == 2)
			{
				mesh = dolfin::UnitSquare(imageSize[0], imageSize[1]);
				elemSig = std::string("FiniteElement('Lagrange', 'triangle', 1)");
				dofSig = std::string("FFC dof map for FiniteElement('Lagrange', 'triangle', 1)");
			}
			else if(ImageDimension == 3)
			{
				mesh = dolfin::UnitCube(imageSize[0], imageSize[1], imageSize[2]);
				elemSig = std::string("FiniteElement('Lagrange', 'tetrahedron', 1)");
				dofSig = std::string("FFC dof map for FiniteElement('Lagrange', 'tetrahedron', 1)");
			}
			else
			{
				throw std::runtime_error("Input image dimension must be 2 or 3.");
			}

//			typedef DolfinImageFunctionSpace< ImageType > IFSType;
//			typedef std::tr1::shared_ptr<const IFSType> IFSPointerType;
//			IFSPointerType ifs = IFSPointerType(new IFSType(imageData,
//					typename IFSType::MeshType(&mesh, dolfin::NoDeleter<const dolfin::Mesh>()),
//					typename IFSType::ElementType(new dolfin::FiniteElement(elemSig)),
//					typename IFSType::DofMapType(new dolfin::DofMap(dofSig, mesh))));
//			dolfin::Function func(ifs);

			typedef DolfinImageFunction< ImageType > IFType;
			dolfin::FiniteElement element(elemSig);
			dolfin::DofMap dofmap(dofSig, mesh);
			dolfin::FunctionSpace fs(mesh, element, dofmap);
			IFType func(imageData, fs);

			return func;
		}

		/**
		 * Set the input itk::Image. If the input pixel is not
		 * of double type, it will be casted to double during convertion
		 * to dolfin::Function.
		 */
		void SetInput(ImageType *image)
		{
			m_Image = image;
		};

#if !defined(CABLE_CONFIGURATION)
		/**
		 * Get a dolfin::Function from itk::Image
		 */
//		dolfin::Function GetOutput();

		void Update();
#endif //!defined(CABLE_CONFIGURATION)

	protected:
		ImageToDolfinFunction(const Self&); // Not implemented.
		void operator=(const Self&); // Not implemented.

		ImageType *m_Image;
	};

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkImageToDolfinFunction.txx"
#endif

#endif // _itkImageToDolfinFunction_h

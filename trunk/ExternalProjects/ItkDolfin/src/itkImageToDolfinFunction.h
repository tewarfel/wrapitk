#ifndef _itkImageToDolfinFunction_h
#define _itkImageToDolfinFunction_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkImportImageFilter.h"

#include "itkDolfinImageFunctionSpace.h"
#if !defined(CABLE_CONFIGURATION)
#include <dolfin/function/Function.h>
#include <dolfin/mesh/UnitSquare.h>
#include <dolfin/mesh/UnitCube.h>
#else
namespace dolfin
{
	template<typename T> class NoDeleter {};

	class UnitSquare {};
	class UnitCube {};
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

		typedef std::tr1::shared_ptr<dolfin::Function> FPointerType;
		typedef DolfinImageFunctionSpace< ImageType > IFSType;
		typedef std::tr1::shared_ptr<IFSType> IFSPointerType;
		typedef std::tr1::shared_ptr<const IFSType> IFSConstPointerType;

		/** Image dimension. */
		itkStaticConstMacro(ImageDimension, unsigned int, ImageType::ImageDimension);

		static FPointerType Convert(ImageType *imageData)
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

//			IFSConstPointerType ifs = IFSConstPointerType( new IFSType(imageData,
//					typename IFSType::MeshConstPointerType(&mesh, dolfin::NoDeleter<const dolfin::Mesh>()),
//					typename IFSType::ElementConstPointerType(new dolfin::FiniteElement(elemSig)),
//					typename IFSType::DofMapConstPointerType(new dolfin::DofMap(dofSig, mesh))) );

			std::tr1::shared_ptr<const dolfin::FunctionSpace> ifs(new dolfin::FunctionSpace(
					typename IFSType::MeshConstPointerType(&mesh, dolfin::NoDeleter<const dolfin::Mesh>()),
					typename IFSType::ElementConstPointerType(new dolfin::FiniteElement(elemSig)),
					typename IFSType::DofMapConstPointerType(new dolfin::DofMap(dofSig, mesh))) );

			FPointerType funcp = FPointerType(new dolfin::Function(ifs));

//			dolfin::FiniteElement element(elemSig);
//			dolfin::DofMap dofmap(dofSig, mesh);
//			dolfin::FunctionSpace fs(mesh, element, dofmap);
//			IFType func(imageData, fs);
//			IFPointerType funcp = IFPointerType(new IFType(imageData, fs));

			return funcp;
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

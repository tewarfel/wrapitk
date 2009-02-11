#ifndef _itkImageToDolfinFunction_h
#define _itkImageToDolfinFunction_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkImportImageFilter.h"

#if !defined(CABLE_CONFIGURATION)
// let the compiler do its normal job
#include <dolfin/Function.h>
#include <dolfin/FunctionSpace.h>
#include <dolfin/fem/FiniteElement.h>
#include <dolfin/mesh/UnitSquare.h>
#include <dolfin/mesh/UnitCube.h>
#else
// prevent gccxml to parse dolphin stuff. We give him a fake dolphin::Function class, the only
// dolphin class exposed.
namespace dolphin
{
  class Function {};
};
#endif

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
	 */

	template <typename TImage>
	class ImageToDolfinFunction
	{
	public:
		///! Standard "Self" typedef.
		typedef ImageToDolfinFunction Self;

		/// Type of the image from where the buffer will be converted
		typedef TImage ImageType;
		typedef typename ImageType::PixelType PixelType;
		typedef typename ImageType::SizeType SizeType;
		typedef typename ImageType::IndexType IndexType;
		typedef typename ImageType::RegionType RegionType;
		typedef typename ImageType::PointType PointType;
		typedef typename ImageType::SpacingType SpacingType;
		typedef typename ImageType::Pointer ImagePointer;

		/** Image dimension. */
		itkStaticConstMacro(ImageDimension, unsigned int, ImageType::ImageDimension);

		/// Type of the import image filter
		//		typedef ImportImageFilter< PixelType, ImageDimension> ImporterType;
		//		typedef typename ImporterType::Pointer ImporterPointer;

		/**
		 * Set the input itk::Image. If the input pixel is not
		 * of double type, it will be casted to double during convertion
		 * to dolfin::Function.
		 */
		void SetInput(ImageType *image)
		{	m_Image = image;};

		/**
		 * Get a dolfin::Function from itk::Image
		 */
		typename dolphin::Function GetOutput();

		void Update();

		/**
		 * Get an itk::Image from dolfin::Function
		 */
		//		static const ImagePointer GetImageFromFunction( Function func );

	protected:
    ImageToDolfinFunction() {}
    virtual ~ImageToDolfinFunction() {}
	
	private:
		ImageToDolfinFunction(const Self&); // Not implemented.
		void operator=(const Self&); // Not implemented.

		ImagePointer m_Image;

	};

} // namespace itk


// again, prevent gccxml to parse dolphin code
#if !defined(CABLE_CONFIGURATION)
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkImageToDolfinFunction.txx"
#endif
#endif

#endif // _itkImageToDolfinFunction_h

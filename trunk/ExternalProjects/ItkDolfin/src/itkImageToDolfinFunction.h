#ifndef _itkImageToDolfinFunction_h
#define _itkImageToDolfinFunction_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkImportImageFilter.h"

#include <dolfin/Function.h>
#include <dolfin/FunctionSpace.h>
#include <dolfin/fem/FiniteElement.h>
#include <dolfin/mesh/UnitSquare.h>
#include <dolfin/mesh/UnitCube.h>

#using namespace dolfin;

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
	class DolfinFunction
	{
	public:
		///! Standard "Self" typedef.
		typedef DolfinFunction Self;

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
		Function SetInput(ImageType *image)
		{	m_Image = image;};

		/**
		 * Get a dolfin::Function from itk::Image
		 */
		Function GetOutput();

		void Update();

		/**
		 * Get an itk::Image from dolfin::Function
		 */
		//		static const ImagePointer GetImageFromFunction( Function func );

	protected:
		DolfinFunction(const Self&); // Not implemented.
		void operator=(const Self&); // Not implemented.

		ImageType *m_Image = null;

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
	};

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkImageToDolfinFunction.txx"
#endif

#endif // _itkImageToDolfinFunction_h

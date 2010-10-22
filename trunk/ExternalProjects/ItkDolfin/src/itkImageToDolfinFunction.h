#ifndef _itkImageToDolfinFunction_h
#define _itkImageToDolfinFunction_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkImportImageFilter.h"

#include "itkDolfinImageFunction.h"
//#include <boost/shared_ptr.hpp>
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
         *        \author Ali Tonddast-Navaei
         *        \author Gaetan Lehman
         */

        template <typename TImage>
        class ImageToDolfinFunction
        {
        public:
                ///! Standard "Self" typedef.
                typedef ImageToDolfinFunction Self;

                typedef TImage ImageType;
                typedef typename ImageType::SizeType SizeType;

                typedef DolfinImageFunction< ImageType > IFType;
                typedef boost::shared_ptr< IFType > IFPointerType;
                typedef boost::shared_ptr< const dolfin::FunctionSpace > FSConstPointerType;
                typedef dolfin::Mesh MeshType;
                typedef typename boost::shared_ptr< const dolfin::Mesh > MeshConstPointerType;
                typedef typename boost::shared_ptr< const dolfin::FiniteElement > ElementConstPointerType;
                typedef typename boost::shared_ptr< const dolfin::DofMap > DofMapConstPointerType;

                /** Image dimension. */
                itkStaticConstMacro(ImageDimension, unsigned int, ImageType::ImageDimension);

                static IFPointerType Convert(ImageType *imageData)
                {
                        SizeType imageSize = imageData->GetBufferedRegion().GetSize();

                        dolfin::Mesh mesh;
                        std::string elemSig;
                        std::string dofSig;
                        if(ImageDimension == 2)
                        {
                                mesh = dolfin::UnitSquare(imageSize[0] - 1, imageSize[1] - 1);
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
                        std::cerr << "[debug] 00\n";
//                        FSConstPointerType V(new dolfin::FunctionSpace(
//                                                        MeshConstPointerType(&mesh, dolfin::NoDeleter<const dolfin::Mesh>()),
//                                                        ElementConstPointerType(new dolfin::FiniteElement(elemSig)),
//                                                        DofMapConstPointerType(new dolfin::DofMap(dofSig, mesh))) );
                        dolfin::FunctionSpace V(
                                        MeshConstPointerType(&mesh, dolfin::NoDeleter<const dolfin::Mesh>()),
                                        ElementConstPointerType(new dolfin::FiniteElement(elemSig)),
                                        DofMapConstPointerType(new dolfin::DofMap(dofSig, mesh)));
                        std::cerr << "[debug] 01\n";
                        IFPointerType v(new IFType(imageData, V));
                        std::cerr << "[debug] 02\n";
                        return v;
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

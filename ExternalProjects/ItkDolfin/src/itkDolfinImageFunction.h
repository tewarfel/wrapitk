#ifndef _itkDolfinImageFunction_h
#define _itkDolfinImageFunction_h

#include <boost/shared_ptr.hpp>
#if !defined(CABLE_CONFIGURATION)
#include <dolfin/function/Function.h>
#include <dolfin/function/FunctionSpace.h>
#include <dolfin/mesh/Mesh.h>
#include <dolfin/fem/FiniteElement.h>
#include <dolfin/fem/DofMap.h>
#include <dolfin/mesh/UnitSquare.h>
#include <dolfin/mesh/UnitCube.h>
#else //!defined(CABLE_CONFIGURATION)
namespace dolfin
{
	class Function {};
	class Mesh {};
	class FiniteElement
	{
		FiniteElement(std::string);
	};
	class DofMap
	{
		DofMap(const std::string, const Mesh&);
	};
	class FunctionSpace
	{
		FunctionSpace(boost::shared_ptr<const Mesh>,
				boost::shared_ptr<const FiniteElement>,
				boost::shared_ptr<const DofMap>);
	};
	template <typename T> class NoDeleter {};
	class UnitSquare {};
	class UnitCube {};
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

	template <typename TImage>
	class DolfinImageFunction : public dolfin::Function
	{
	public:
		typedef TImage ImageType;
		typedef typename ImageType::PixelType PixelType;
		typedef typename ImageType::SizeType SizeType;

		typedef boost::shared_ptr<const dolfin::FunctionSpace> FSConstPointerType;
		typedef dolfin::Mesh MeshType;
		typedef typename boost::shared_ptr<const dolfin::Mesh> MeshConstPointerType;
		typedef typename boost::shared_ptr<const dolfin::FiniteElement> ElementConstPointerType;
		typedef typename boost::shared_ptr<const dolfin::DofMap> DofMapConstPointerType;

		/** Image dimension. */
		itkStaticConstMacro(ImageDimension, unsigned int, ImageType::ImageDimension);

	protected:
		double *m_ImageData;
		SizeType m_ImageSize;

	public:
		DolfinImageFunction(boost::shared_ptr<const dolfin::FunctionSpace> V):
			dolfin::Function(V)
		{
		};

		DolfinImageFunction(ImageType* imageData) :
			dolfin::Function()
		{
			// Input verifications
			if(!imageData)
			{
				throw std::runtime_error("Input image is null.");
			}
			if ((ImageDimension < 2) || (ImageDimension> 3))
			{
				throw std::runtime_error("Input image dimension must be 2 or 3.");
			}

			// Variable initialisations
			PixelType *buffer = const_cast<PixelType *> (imageData->GetBufferPointer());
			m_ImageData = (double *) (buffer);
			m_ImageSize = imageData->GetBufferedRegion().GetSize();

			// Create a Function instance
			FSConstPointerType V = CreateFunctionSpace();
			DolfinImageFunction v(V);
			*this = v;
		};

		FSConstPointerType CreateFunctionSpace()
		{
			MeshType mesh;
			std::string elemSig;
			std::string dofSig;
			if(ImageDimension == 2)
			{
				mesh = dolfin::UnitSquare(m_ImageSize[0] - 1, m_ImageSize[1] - 1);
				elemSig = std::string("FiniteElement('Lagrange', 'triangle', 1)");
				dofSig = std::string("FFC dof map for FiniteElement('Lagrange', 'triangle', 1)");
			}
			else if(ImageDimension == 3)
			{
				mesh = dolfin::UnitCube(m_ImageSize[0], m_ImageSize[1], m_ImageSize[2]);
				elemSig = std::string("FiniteElement('Lagrange', 'tetrahedron', 1)");
				dofSig = std::string("FFC dof map for FiniteElement('Lagrange', 'tetrahedron', 1)");
			}
			else
			{
				throw std::runtime_error("Input image dimension must be 2 or 3.");
			}

			FSConstPointerType V(new dolfin::FunctionSpace(
							MeshConstPointerType(&mesh, dolfin::NoDeleter<const dolfin::Mesh>()),
							ElementConstPointerType(new dolfin::FiniteElement(elemSig)),
							DofMapConstPointerType(new dolfin::DofMap(dofSig, mesh))) );

			return V;
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

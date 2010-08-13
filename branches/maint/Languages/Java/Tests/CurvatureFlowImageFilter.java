/**
 *  Example on the use of the CurvatureFlowImageFilter
 *
 */

import org.itk.io.*;
import org.itk.intensityfilters.*;
import org.itk.denoising.*;
import org.itk.simplefilters.*;


public class CurvatureFlowImageFilter
{
  public static void main( String argv[] )
  {
    itkImageFileReaderIUS2 reader = new itkImageFileReaderIUS2();
    itkImageFileWriterIUS2 writer = new itkImageFileWriterIUS2();

    itkCastImageFilterIUS2IF2 inputCast = new itkCastImageFilterIUS2IF2();
      
    itkCurvatureFlowImageFilterIF2IF2 filter = new itkCurvatureFlowImageFilterIF2IF2();

    itkRescaleIntensityImageFilterIF2IUS2 outputCast = new itkRescaleIntensityImageFilterIF2IUS2();

    inputCast.SetInput( reader.GetOutput() );
    filter.SetInput( inputCast.GetOutput() );
    outputCast.SetInput( filter.GetOutput() );
    writer.SetInput( outputCast.GetOutput() );

    outputCast.SetOutputMinimum(  0  );
    outputCast.SetOutputMaximum( 255 );

    filter.SetNumberOfIterations( Integer.parseInt( argv[2] ) );
    filter.SetTimeStep(           Float.parseFloat( argv[3] ) );

    reader.SetFileName( argv[0] );
    writer.SetFileName( argv[1] );

    writer.Update();
  }

}



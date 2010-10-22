/**
 *  Example on the use of the SigmoidImageFilter
 *
 */

import org.itk.io.*;
import org.itk.intensityfilters.*;

public class SigmoidImageFilter
{
  public static void main( String argv[] )
  {
    System.out.println("SigmoidImageFilter Example");

    itkImageFileReaderIUS2 reader = new itkImageFileReaderIUS2();
    itkImageFileWriterIUS2 writer = new itkImageFileWriterIUS2();

    itkSigmoidImageFilterIUS2IUS2 filter = new itkSigmoidImageFilterIUS2IUS2();

    filter.SetInput( reader.GetOutput() );
    writer.SetInput( filter.GetOutput() );

    reader.SetFileName( argv[0] );
    writer.SetFileName( argv[1] );

    filter.SetOutputMinimum( Short.parseShort( argv[2] ) );
    filter.SetOutputMaximum( Short.parseShort( argv[3] ) );

    filter.SetAlpha( Float.parseFloat( argv[4] ) );
    filter.SetBeta(  Float.parseFloat( argv[5] ) );

    writer.Update();
  }

}

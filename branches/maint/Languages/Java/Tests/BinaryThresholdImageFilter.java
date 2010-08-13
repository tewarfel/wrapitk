/**
 *  Example on the use of the BinaryThresholdImageFilter
 *
 */

import org.itk.io.*;
import org.itk.binarymorphology.*;

public class BinaryThresholdImageFilter
{
  public static void main( String argv[] )
  {
    System.out.println("BinaryThresholdImageFilter Example");

    itkImageFileReaderIUS2 reader = new itkImageFileReaderIUS2();
    itkImageFileWriterIUS2 writer = new itkImageFileWriterIUS2();

    itkBinaryThresholdImageFilterIUS2IUS2 filter = new itkBinaryThresholdImageFilterIUS2IUS2();

    filter.SetInput( reader.GetOutput() );
    writer.SetInput( filter.GetOutput() );

    reader.SetFileName( argv[0] );
    writer.SetFileName( argv[1] );

    filter.SetLowerThreshold(  Integer.parseInt( argv[2] )  );
    filter.SetUpperThreshold(  Integer.parseInt( argv[3] )  );

    filter.SetOutsideValue( Integer.parseInt( argv[4] )  );
    filter.SetInsideValue(  Integer.parseInt( argv[5] )  );

    writer.Update();
  }

}



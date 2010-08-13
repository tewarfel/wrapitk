/**
 *  Example on the use of the BinaryDilateImageFilter
 *
 */

import org.itk.base.*;
import org.itk.io.*;
import org.itk.binarymorphology.*;
import org.itk.simplefilters.*;

public class BinaryDilateImageFilter
{
  public static void main( String argv[] )
  {
    System.out.println("BinaryDilateImageFilter Example");

    itkImageFileReaderIUS2 reader = new itkImageFileReaderIUS2();
    itkImageFileWriterIUC2 writer = new itkImageFileWriterIUC2();
    itkCastImageFilterIUS2IUC2 caster = new itkCastImageFilterIUS2IUC2();

    itkBinaryDilateImageFilterIUS2IUS2SE2 filter = new itkBinaryDilateImageFilterIUS2IUS2SE2();

    filter.SetInput( reader.GetOutput() );
    caster.SetInput( filter.GetOutput() );
    writer.SetInput( caster.GetOutput() );

    reader.SetFileName( argv[0] );
    writer.SetFileName( argv[1] );

    itkSize2 radius = new itkSize2();
    radius.Fill( 5 );
    itkFlatStructuringElement2 element = itkFlatStructuringElement2.Ball( radius );

    filter.SetKernel( element );

    short value = 200;

    filter.SetDilateValue( value );

    writer.Update();
  }

}



#
#  Program:   Insight Segmentation & Registration Toolkit
#  Module:    $RCSfile: itkutils.tcl,v $
#  Language:  C++
#  Date:      $Date: 2006/09/06 20:58:42 $
#  Version:   $Revision: 1.1 $
#
#  Copyright (c) Insight Software Consortium. All rights reserved.
#  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.
#
#     This software is distributed WITHOUT ANY WARRANTY; without even
#     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
#     PURPOSE.  See the above copyright notices for more information.
#

# Define ITK Tcl utilities.
namespace eval itk {

  # Allow code like "$obj Print [itk::result]
  proc result {} {
    return [itk::TclStringStream [cable::Interpreter]]
  }

  # Create an object of the given type.  Return a pointer to it.  A
  # smart pointer to the object is kept in a list that is destroyed at
  # program exit.
  proc create {type} {
    global itk::_ObjectList_
    set ptr [itk::$type New]
    set p [$ptr ->]
    lappend _ObjectList_ $ptr
    return $p
  }

  # Start with an empty object list.
  set _ObjectList_ {}

  # Create an image viewer in a given frame.
  proc createImageViewer2D {frame image args} {
    # Create the canvas.
    eval canvas $frame.canvas {-scrollregion "1 1 32 32"} \
                              {-xscrollcommand "$frame.scrollx set"} \
                              {-yscrollcommand "$frame.scrolly set"} $args
    scrollbar $frame.scrollx -orient horizontal \
                             -command "$frame.canvas xview"
    scrollbar $frame.scrolly -orient vertical \
                             -command "$frame.canvas yview"
    pack $frame.scrollx -side bottom -fill x
    pack $frame.scrolly -side right -fill y
    pack $frame.canvas -expand 1 -fill both

    # Create a Tk image on the canvas.
    set i [image create photo]
    $frame.canvas create image 0 0 -image $i -anchor nw

    # Setup the TkImageViewer2D instance.
    set viewer [itk::create TkImageViewer2D]
    $viewer SetInput $image
    $viewer SetInterpreter [cable::Interpreter]
    $viewer SetImageName $i
    $viewer SetCanvasName $frame.canvas
    return $viewer
  }

  # Create a Tcl callback event.
  proc createTclCommand { cmd } {
    set command [itk::create TclCommand]
    $command SetInterpreter [cable::Interpreter]
    $command SetCommandString $cmd
    return $command
  }

  # Tcl procedure to list the wrapped classes.
  proc listClasses {} {
    set cmds {}
    foreach c [info commands ::itk::*] {
      if { ! [regexp {(<)|(_Pointer$)|(_Superclass$)|(^::itk::[^A-Z])} $c] } {
        lappend cmds $c
      }
    }
    set cmds [lsort $cmds]
    foreach c $cmds {
      puts $c
    }
  }

  proc listMethods {obj} {
    cable::ListMethods $obj
  }

  namespace export create result createImageViewer2D
}

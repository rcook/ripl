# RIPLMAIN.TCL
# Version 1.1
# By Richard Cook, Copyright (C) 1998
#
# This is the main script which draws together the JASHE stuff and my own add-ons.

# The following sets up directory variables to point to the various Tcl/Tk script files
# lying around in various places.
#
# First the RIPL_TCLDIR environment variable is read and used to set the riplAddOnDir variable.
# If this variable is undefined then the full pathname of this file is used as the stem
# and assigned to the riplAddOnDir variable. RIPL_DLLDIR points to location of DLL.
#
# The JASHE_LIBDIR and JASHE_DIR environment variables can be used to define the location
# of the JASHE files. If they are not defined, then the pointers are set by parsing the
# riplAddOnDir variable instead (the JASHE files are assumed to be in ***/WISH and ***/WATTK directories
# respectively.

if {[array get env RIPL_TCLDIR]==""} {
    set riplAddOnDir [file dirname [file join [pwd] [info script]]]
} else {
    set riplAddOnDir $env(RIPL_TCLDIR)
}
if {[array get env RIPL_DLLDIR]==""} {
    set riplDLLDir [file dirname $riplAddOnDir]/ripldll
} else {
    set riplDLLDir $env(RIPL_DLLDIR)
}
if {[array get env JASHE_DIR]==""} {
    set jashe_dir [file dirname $riplAddOnDir]/wish
} else {
    set jashe_dir $env(JASHE_DIR)
}
if {[array get env JASHE_LIBDIR]==""} {
    set jashe_libdir [file dirname $jashe_dir]/wattk
} else {
    set jashe_libdir $env(JASHE_LIBDIR)
}

lappend auto_path $jashe_libdir $riplDLLDir
package require tkjashe
package require ripldll

# Variable definitions
source $jashe_dir/jashed.tcl

after idle {puts -nonewline $usageString}

# Jashe library access: configuration and execution
source $jashe_dir/jashel.tcl

# Generic (ish) widget control procedures
source $jashe_dir/jasheg.tcl

# Variable and widget control procedures
source $jashe_dir/jashec.tcl

# Widget creation
source $jashe_dir/jashew.tcl

# Load in my add-on stuff
source $riplAddOnDir/ripl.tcl

jashecreate $jasheBase
riplcreate $riplBase $jasheBase

newImageWind $inputWind $inputImg
newOutput
toggleAdv
toggleCF


# Second time of asking
after idle selectTempl "Default"
after idle {puts -nonewline $usageString}
#after idle {puts $jashe_dir}

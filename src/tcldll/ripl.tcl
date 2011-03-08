##################################################
# RIPL.TCL
# Main driver script
# Version 1.0
# By Richard Cook, October 1997
##################################################

##################################################
# Some window-specific procedures
##################################################

# Shows or hides the 'Ripl' window
proc riplToggleWindow {} {
    global riplWin
    if {[wm state $riplWin]=="normal"} {
		wm withdraw $riplWin
    } else {
		wm deiconify $riplWin
    }
	focus $riplWin
}

# Displays a dialogue box containing information about this programme
proc riplAboutBox {} {
	global riplAppName
	tk_messageBox -icon info -type ok -title "About $riplAppName" -message \
		"Ripl Tcl/Tk Interface\n\n\
		Add-On: Copyright (c) 1998, Richard A. Cook\n\
		Incorporates Ripl Version 1.1\n\
		(Richard's Image-Processing Library)\n\n\
		JASHE: Copyright (c) 1997, J. Alex Stark\n"
}

# Adds widgets corresponding to the currently selected transform to the window
proc riplUpdateOptions {{a ""} {b ""} {c ""}} {
	global riplWin riplOp riplOpProcList riplWidgetPady
	if {$riplOp>=0} {$riplWin.buttons.apply configure -state normal}
	# Must hide window while adding new widgets to it (I think)
 	wm withdraw $riplWin
	# Destroy anything that was there before
	catch {destroy $riplWin.options.op}
	frame $riplWin.options.op
	pack $riplWin.options.op -fill x
	frame $riplWin.options.op.spacer1 -height [expr 5*$riplWidgetPady]
	pack $riplWin.options.op.spacer1
	set str [lindex $riplOpProcList $riplOp]
	eval ripl${str}Options $riplWin.options.op
	frame $riplWin.options.op.spacer2 -height [expr 5*$riplWidgetPady]
	pack $riplWin.options.op.spacer2
	# Show the updated window
	wm deiconify $riplWin
}

# Interrupts or terminates the currently executing transform (restores controls in window etc.)
proc riplStopApply {} {
	global riplWin
	# Jashe command
	stopApply
	# Reset states of appropriate controls
	$riplWin.buttons.cancel configure -state disabled
	$riplWin.buttons.apply configure -state normal
}

# Execute the currently select transform passing the appropriate arguments to it
proc riplApplyTransform {} {
	global riplWin riplBase riplAppName riplCancel riplOp riplOpProcList

    global jasheBase numTerms redispRate
    global currOutWind
    global numJashes cancelJashes inputImg currOutImg

	incr numJashes
	if {$numJashes>1} {
		# Another process is running
		incr numJashes -1
		set reply [tk_messageBox -icon question -type yesno \
			-title $riplAppName -message "Another process is already running!\n\nInterrupt it?"]
		case $reply {
			yes {
				if {"[.outputF.apply cget -text]"=="Cancel"} {set cancelJashes 1} else {bell}
				tk_messageBox -icon info -type ok \
					-title $riplAppName -message "Try 'Apply' again to apply this transform."
			}
			no {return}
		}
	} else {
	
		# Set states of appropriate controls
		$riplWin.buttons.apply configure -state disabled
		$riplWin.buttons.cancel configure -state normal
		.outputF.apply config -text "Cancel"
		set cancelJashes 0
		set numJashes 1
		update

		# Tell Jashe which images to use
		$riplBase setup $inputImg $currOutImg
		update
		if {$cancelJashes || $riplCancel} {riplStopApply; return}

		# Execute appropriate ripldll command
		set str [lindex $riplOpProcList $riplOp]
		set result [eval [join [list ripl $str Apply] ""]]
#		if {$result==1} {
			update
			$riplBase output
			update
#		}
		riplStopApply
	}
}

##################################################
# Global variables
##################################################

set riplBase ripl1
set riplAppName "Ripl"
set riplWin .riplWindow
set riplLabelWidth 20
set riplTextWidth 30
set riplWidgetPadx 10
set riplWidgetPady 5
set riplButtonIPadx 10
set riplOpList []
set riplOpProcList []
set riplOp -1
set riplCancel 0

##################################################
# INCLUDE THE TRANSFORM-SPECIFIC STUFF
##################################################
source $riplAddOnDir/ripltran.tcl
riplSetupTransformLists riplOpList riplOpProcList

##################################################
# SET UP THE WINDOW, ITS WIDGETS AND OTHER STUFF
##################################################

# Include listbox library
source $riplAddOnDir/ripllb.tcl

# Append new menu option to Jashe "Windows" menu
.menuBar.wind.m insert [expr [.menuBar.wind.m index last]-1] \
	command -label $riplAppName -command "riplToggleWindow" -underline 1

toplevel $riplWin
wm title $riplWin $riplAppName

frame $riplWin.menubar -relief raised -bd 2
menubutton $riplWin.menubar.actions -text Actions -underline 0 -menu $riplWin.menubar.actions.menu
menubutton $riplWin.menubar.window -text Window -underline 0 -menu $riplWin.menubar.window.menu
pack $riplWin.menubar.actions $riplWin.menubar.window -side left
#pack $riplWin.menubar.window -side left
pack $riplWin.menubar -side top -fill x
tk_menuBar $riplWin.menubar $riplWin.menubar.window

menu $riplWin.menubar.actions.menu -tearoff 0
$riplWin.menubar.actions.menu add command -label "Exit JASHE" -underline 1 -command exit

menu $riplWin.menubar.window.menu -tearoff 0
$riplWin.menubar.window.menu add command -label "Hide Window" -underline 0 -command riplToggleWindow
$riplWin.menubar.window.menu add separator
$riplWin.menubar.window.menu add command -label "About $riplAppName..." -underline 0 -command riplAboutBox

frame $riplWin.inout
pack $riplWin.inout -fill x

frame $riplWin.inout.in
pack $riplWin.inout.in -fill x
label $riplWin.inout.in.label -text "Input" -padx $riplWidgetPadx -width $riplLabelWidth
label $riplWin.inout.in.filename -relief ridge -textvariable infile -padx $riplWidgetPadx -width $riplTextWidth
pack $riplWin.inout.in.label $riplWin.inout.in.filename -side left -padx $riplWidgetPadx -pady $riplWidgetPady

frame $riplWin.inout.out
pack $riplWin.inout.out -fill x
label $riplWin.inout.out.label -text "Output Window" -padx $riplWidgetPadx -width $riplLabelWidth
label $riplWin.inout.out.name -relief ridge -textvariable currOutput -padx $riplWidgetPadx -width $riplTextWidth
pack $riplWin.inout.out.label $riplWin.inout.out.name -side left -padx $riplWidgetPadx -pady $riplWidgetPady

frame $riplWin.op
pack $riplWin.op -fill x
riplLBCreate "Transform" $riplWin.op riplOp riplOpList [list]

frame $riplWin.options
pack $riplWin.options -fill x

frame $riplWin.buttons
pack $riplWin.buttons -fill x
button $riplWin.buttons.apply -text Apply -command riplApplyTransform
button $riplWin.buttons.cancel -text Cancel -command "set riplCancel 1"
button $riplWin.buttons.hide -text Hide -command riplToggleWindow
button $riplWin.buttons.about -text About... -command riplAboutBox
pack $riplWin.buttons.apply $riplWin.buttons.cancel \
	$riplWin.buttons.hide $riplWin.buttons.about \
	-side left -ipadx $riplButtonIPadx -padx $riplWidgetPadx \
	-pady $riplWidgetPady -expand 1

$riplWin.buttons.apply configure -state disabled
$riplWin.buttons.cancel configure -state disabled

trace variable riplOp w riplUpdateOptions

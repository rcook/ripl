# RIPLLB.TCL
# Generic Listbox-Handling Routines
# Version 1.0
# By Richard A Cook
# October 1997

# Some global listbox instance counter---used to generate unique names for listboxes
set riplLBCount -1

proc riplLBRefresh {menu selection outputtext itemlist commandlist} {
	# the variable name passed in 'itemlist' must refer to a GLOBAL variable
	global $itemlist
	$menu delete 0 last
	set i 0
	foreach item [expr $$itemlist] {
		$menu add command -label $item -command "set $selection $i; set $outputtext [list $item]"
		incr i
	}
	if {[llength  $commandlist]>0} {
		$menu add separator
		foreach item $commandlist {
			eval $menu add command $item
		}
	}
}

proc riplLBCreate {labeltext parent selection itemlist commandlist} {
	# The variable names passed in 'selection' and 'itemlist' must refer to GLOBAL variables
	global $selection $itemlist riplLBCount riplLBText riplLabelWidth riplTextWidth riplWidgetPadx riplWidgetPady
	# Increment the global counter
	incr riplLBCount
	if {$selection>=0} {
		set riplLBText(${riplLBCount}) [lindex $$itemlist [expr $$selection]]
	} else {
		set riplLBText(${riplLBCount}) ""
	}
	frame $parent.lb${riplLBCount}
	label $parent.lb${riplLBCount}.label -text $labeltext -padx $riplWidgetPadx -width $riplLabelWidth
	menubutton $parent.lb${riplLBCount}.menu -textvariable riplLBText(${riplLBCount}) \
		-menu $parent.lb${riplLBCount}.menu.m -indicatoron yes -relief ridge -padx $riplWidgetPadx -width $riplTextWidth
	menu $parent.lb${riplLBCount}.menu.m -postcommand \
		"riplLBRefresh $parent.lb${riplLBCount}.menu.m $selection riplLBText(${riplLBCount}) $itemlist [list $commandlist]" \
		-tearoff 0
	pack $parent.lb${riplLBCount}.label $parent.lb${riplLBCount}.menu -side left -padx $riplWidgetPadx -pady  $riplWidgetPady
	pack $parent.lb${riplLBCount} -fill x
	return $parent.lb${riplLBCount}.menu.m
}


##################################################
# RIPLTRAN.TCL
# Transforms and their widget-related stuff
# Version 1.0
# By Richard Cook, October 1997
##################################################

##################################################
# Gaussian Smoothing Filter procedures
#
# Arguments required by function:
#		1.		The variance of the kernel:		riplGaussVar
##################################################

# Install widgets onto window
proc riplGaussOptions {frame} {
	global riplGaussVar riplGaussSize riplWidgetPadx riplWidgetPady riplLabelWidth riplTextWidth
	label $frame.label -text "Variance of kernel"
	pack $frame.label -side left

	frame $frame.value
	pack $frame.value -side top -fill x
	scaleentry $frame.value.ent riplGaussVar

	scaleBar $frame.scale riplGaussVar 0 10 1 2 0.1
	
	frame $frame.approx
	pack $frame.approx -fill x
	label $frame.approx.lbl -text "Approximate size of kernel" -padx $riplWidgetPadx -width $riplLabelWidth
	label $frame.approx.val -textvariable riplGaussSize -relief ridge -padx $riplWidgetPadx -width $riplTextWidth
	pack $frame.approx.lbl $frame.approx.val -side left -padx $riplWidgetPadx -pady $riplWidgetPady

	set riplGaussVar 1.0
	riplGaussVarProcess
}

# scaleBar event-handler
proc riplGaussVarProcess {} {
	global riplGaussVar riplGaussSize
	
	set riplGaussSize [expr 2*round(3.7*$riplGaussVar-0.5)+1]
	if {$riplGaussSize<1} {set riplGaussSize 1}
}

# scaleentry Enter event-handler
proc riplGaussVarEnter {} {
	global riplWin
	riplGaussVarProcess
	focus $riplWin
}

# Apply the filter by calling the C DLL function
proc riplGaussApply {} {
	global riplBase riplGaussVar
	$riplBase ripl gaussian $riplGaussVar
	return 1
}

##################################################
# Convolution with 2D Kernel
#
# Arguments required by function:
#		1.		The number of columns in the kernel:	riplConvCols
#		2.		The number of rows in the kernel:		riplConvRows
#		3.		The kernel matrix elements:				riplConvVal
##################################################

# Install widgets onto window
proc riplConvOptions {frame} {
	global riplConvCols riplConvRows riplConvVal riplConvFrame riplWidgetPadx
	global riplWidgetPady riplLabelWidth riplTextWidth
	global riplConvMin riplConvMax riplConvScale riplConvNeg riplConvIntFP
	global riplConvSum

	set riplConvMin 1
	set riplConvMax 10
	set riplConvIntFP 0
	set riplConvScale 1
	set riplConvNeg 0
	set riplConvCols 3
	set riplConvRows 3
	set riplConvSum 0

	# Integer vs floating-point arithmetic.
	frame $frame.arith -relief sunken
	pack $frame.arith -side top -fill x
	label $frame.arith.label -text "Arithmetic Type" -width $riplLabelWidth
	radiobutton $frame.arith.b1 -variable riplConvIntFP -value 0 -indicatoron 1 \
		-selectcolor "white" -text "Integer/fixed-point" -underline 0
	radiobutton $frame.arith.b2 -variable riplConvIntFP -value 1 -indicatoron 1 \
		-selectcolor "white" -text "Floating-point" -underline 0
	pack $frame.arith.label $frame.arith.b1 $frame.arith.b2 -side left -padx $riplWidgetPadx -pady $riplWidgetPady

	# Kernel size parameters.
	frame $frame.kernelsize
	pack $frame.kernelsize -side top -fill x -padx $riplWidgetPadx -pady $riplWidgetPady
	label $frame.kernelsize.collabel -text Columns -width $riplLabelWidth
	wheel $frame.kernelsize.cols riplConvCols riplConvMin riplConvMax
	label $frame.kernelsize.rowlabel -text Rows -width $riplLabelWidth
	wheel $frame.kernelsize.rows riplConvRows riplConvMin riplConvMax
	pack $frame.kernelsize.collabel $frame.kernelsize.cols \
		$frame.kernelsize.rowlabel $frame.kernelsize.rows -side left -padx $riplWidgetPadx -pady $riplWidgetPady

	# Kernel element entry stuff.
	frame $frame.kernel
	pack $frame.kernel -side top -fill x -padx $riplWidgetPadx -pady $riplWidgetPady
	label $frame.kernel.label -text "Kernel Matrix" -width $riplLabelWidth
	pack $frame.kernel.label -side left -fill x
	set riplConvFrame $frame.kernel.sub
	frame $riplConvFrame
	pack $riplConvFrame

	# The sum of the kernel elements.
	frame $frame.sum
	pack $frame.sum -fill x
	label $frame.sum.lbl -text "Sum of Kernel Elements" -padx $riplWidgetPadx -width $riplLabelWidth
	label $frame.sum.val -textvariable riplConvSum -relief ridge -padx $riplWidgetPadx -width $riplTextWidth
	pack $frame.sum.lbl $frame.sum.val -side left -padx $riplWidgetPadx -pady $riplWidgetPady

	# Normalizing factor.
	frame $frame.divisor
	pack $frame.divisor -side top -fill x
	label $frame.divisor.lbl -text "Normalizing Divisor" -padx $riplWidgetPadx -width $riplLabelWidth
	entry $frame.divisor.val -textvariable riplConvScale
	pack $frame.divisor.lbl $frame.divisor.val -side left -padx $riplWidgetPadx -pady $riplWidgetPady

	# What to do with negative convolution sums.
	frame $frame.abs -relief sunken
	pack $frame.abs -side top -fill x
	label $frame.abs.label -text "Negative Sums" -width $riplLabelWidth
	radiobutton $frame.abs.b1 -variable riplConvNeg -value 0 -indicatoron 1 \
		-selectcolor "white" -text "Set to zero" -underline 0
	radiobutton $frame.abs.b2 -variable riplConvNeg -value 1 -indicatoron 1 \
		-selectcolor "white" -text "Store absolute values" -underline 0
	pack $frame.abs.label $frame.abs.b1 $frame.abs.b2 -side left -padx $riplWidgetPadx -pady $riplWidgetPady

	riplConvCreateTable
	riplConvUpdateSum
}

# Calculate the sum of the kernel elements.
proc riplConvUpdateSum {} {
	global riplConvVal riplConvSum

	set riplConvSum 0
	foreach i [array names riplConvVal] {set riplConvSum [expr $riplConvSum+$riplConvVal($i)]}
}


# Update the number of columns in the kernel matrix table
proc riplConvColsProcess {} {
	global riplWin riplConvFrame riplConvVal

 	wm withdraw $riplWin
	destroy $riplConvFrame
	unset riplConvVal

	frame $riplConvFrame
	pack $riplConvFrame

	riplConvCreateTable
	wm deiconify $riplWin
	riplConvUpdateSum
}

# Update the number of rows in the kernel matrix table
proc riplConvRowsProcess {} {
	global riplWin riplConvFrame riplConvVal

 	wm withdraw $riplWin
	destroy $riplConvFrame
	unset riplConvVal

	frame $riplConvFrame
	pack $riplConvFrame

	riplConvCreateTable
	wm deiconify $riplWin
	riplConvUpdateSum
}

# Create the convolution coefficient table
proc riplConvCreateTable {} {
	global riplWin riplConvFrame riplConvRows riplConvCols riplConvVal riplWidgetPadx

	for {set i 0} {$i<$riplConvRows} {incr i} {
		frame $riplConvFrame.r${i}
		pack $riplConvFrame.r${i} -side top -fill x
		for {set j 0} {$j<$riplConvCols} {incr j} {
			entry $riplConvFrame.r${i}.c${j} -textvariable riplConvVal(r${i}c${j}) -width 5
			pack $riplConvFrame.r${i}.c${j} -side left -padx $riplWidgetPadx
			set riplConvVal(r${i}c${j}) 1
			bind $riplConvFrame.r${i}.c${j} <Return> riplConvUpdateSum
			bind $riplConvFrame.r${i}.c${j} <Any-FocusOut> riplConvUpdateSum
		}
	}
}

# Perform the convolution by calling C DLL function
proc riplConvApply {} {
	global riplBase riplConvCols riplConvRows riplConvVal riplConvScale riplConvNeg riplConvIntFP

	set cmdline "$riplBase ripl dllconv "
	if {$riplConvIntFP==0} {
		append cmdline int
	} else {
		append cmdline fp
	}
	append cmdline " $riplConvScale $riplConvNeg $riplConvCols $riplConvRows"
	for {set i 0} {$i<$riplConvRows} {incr i} {
		for {set j 0} {$j<$riplConvCols} {incr j} {
			append cmdline " $riplConvVal(r${i}c${j})"
		}
	}
	puts $cmdline
	eval $cmdline
	return 1
}

##################################################
# Sobel Edge Detection procedures
#
# Arguments required by function:
#		1.		The neighbourhood size:		riplSobelNeighbour
#		2.		The threshold:				riplSobelThresh
#		3.		The overlay:				riplSobelOverlay
##################################################

# Install widgets onto window
proc riplSobelOptions {frame} {
	global riplSobelNeighbour riplSobelWinSize riplSobelThresh riplSobelOverlay
	global riplWidgetPadx riplWidgetPady riplLabelWidth riplTextWidth
	
	frame $frame.neighbour
	pack $frame.neighbour -side top -fill x
	
	label $frame.neighbour.label -text "Neighbourhood" -width $riplLabelWidth
	pack $frame.neighbour.label -side left

	frame $frame.neighbour.value
	pack $frame.neighbour.value -side top -fill x
	scaleentry $frame.neighbour.value.ent riplSobelNeighbour

	scaleBar $frame.neighbour.scale riplSobelNeighbour 1 20 2 5 1
	$frame.neighbour.scale.scale configure -digits 0
	
	frame $frame.size
	pack $frame.size -fill x
	label $frame.size.lbl -text "Window Size (Square)" -padx $riplWidgetPadx -width $riplLabelWidth
	label $frame.size.val -textvariable riplSobelWinSize -relief ridge -padx $riplWidgetPadx -width $riplTextWidth
	pack $frame.size.lbl $frame.size.val -side left -padx $riplWidgetPadx -pady $riplWidgetPady

	frame $frame.thresh
	pack $frame.thresh -side top -fill x
	label $frame.thresh.label -text "Threshold" -width $riplLabelWidth
	pack $frame.thresh.label -side left
	frame $frame.thresh.value
	pack $frame.thresh.value -side top -fill x
	scaleentry $frame.thresh.value.ent riplSobelThresh
	scaleBar $frame.thresh.scale riplSobelThresh 0 255 32 32 1
	$frame.thresh.scale.scale configure -digits 0

	frame $frame.over -relief sunken
	pack $frame.over -side top -fill x

	label $frame.over.label -text "Overlay Mode" -width $riplLabelWidth
	radiobutton $frame.over.b1 -variable riplSobelOverlay -value 0 -indicatoron 1 \
		-selectcolor "white" -text "Don't overlay input image" -underline 0
	radiobutton $frame.over.b2 -variable riplSobelOverlay -value 1 -indicatoron 1 \
		-selectcolor "white" -text "Overlay input image" -underline 0
	pack $frame.over.label $frame.over.b1 $frame.over.b2 -side left -padx $riplWidgetPadx -pady $riplWidgetPady

	set riplSobelNeighbour 0
	set riplSobelThresh 0
	set riplSobelOverlay 0
	riplSobelNeighbourProcess
}

# scaleBar event-handlers
proc riplSobelNeighbourProcess {} {
	global riplSobelNeighbour riplSobelWinSize
	set riplSobelWinSize [expr $riplSobelNeighbour*2+1]
	set riplSobelWinSize "$riplSobelWinSize x $riplSobelWinSize"
}

proc riplSobelThreshProcess {} {
	# Do nothing
}

# scaleentry Enter event-handler
proc riplSobelNeighbourEnter {} {
	global riplWin
	riplSobelNeighbourProcess
	focus $riplWin
}

# Apply the filter by calling the C DLL function
proc riplSobelApply {} {
	global riplBase riplSobelNeighbour riplSobelThresh riplSobelOverlay
	$riplBase ripl sobel $riplSobelNeighbour $riplSobelThresh $riplSobelOverlay
	return 1
}

##################################################
# Median Filter procedures
#
# Arguments required by function:
#		1.		The window size:		riplMedianWinSize
##################################################

# Install widgets onto window
proc riplMedianOptions {frame} {
	global riplMedianWinD riplMedianWinSize riplWidgetPadx riplWidgetPady riplLabelWidth riplTextWidth
	label $frame.label -text "Neighbourhood"
	pack $frame.label -side left

	frame $frame.value
	pack $frame.value -side top -fill x
	scaleentry $frame.value.ent riplMedianWinD

	scaleBar $frame.scale riplMedianWinD 0 20 2 5 1
	$frame.scale.scale configure -digits 0
	
	frame $frame.size
	pack $frame.size -fill x
	label $frame.size.lbl -text "Window Size (Square)" -padx $riplWidgetPadx -width $riplLabelWidth
	label $frame.size.val -textvariable riplMedianWinSize -relief ridge -padx $riplWidgetPadx -width $riplTextWidth
	pack $frame.size.lbl $frame.size.val -side left -padx $riplWidgetPadx -pady $riplWidgetPady

	set riplMedianWinD 0
	riplMedianWinDProcess
}

# scaleBar event-handler
proc riplMedianWinDProcess {} {
	global riplMedianWinD riplMedianWinSize
	set riplMedianWinSize [expr $riplMedianWinD*2+1]
	set riplMedianWinSize "$riplMedianWinSize x $riplMedianWinSize"
}

# scaleentry Enter event-handler
proc riplMedianWinDEnter {} {
	global riplWin
	riplMedianWinDProcess
	focus $riplWin
}

# Apply the filter by calling the C DLL function
proc riplMedianApply {} {
	global riplBase riplMedianWinD
	$riplBase ripl median [expr $riplMedianWinD*2+1]
	return 1
}

##################################################
# Marr-Hildreth Edge Detection
#
# Arguments required by function:
#		1.		The variance of the kernel:		riplMarrHildrethVar
##################################################

# Install widgets onto window
proc riplMarrHildrethOptions {frame} {
	global riplMarrHildrethVar riplMarrHildrethSize riplWidgetPadx riplWidgetPady riplLabelWidth riplTextWidth
	global riplMarrHildrethZeroes
	label $frame.label -text "Variance of Gaussian"
	pack $frame.label -side left

	frame $frame.value
	pack $frame.value -side top -fill x
	scaleentry $frame.value.ent riplMarrHildrethVar

	scaleBar $frame.scale riplMarrHildrethVar 0 10 1 2 0.1
	
	frame $frame.approx
	pack $frame.approx -fill x
	label $frame.approx.lbl -text "Approximate size of kernel" -padx $riplWidgetPadx -width $riplLabelWidth
	label $frame.approx.val -textvariable riplMarrHildrethSize -relief ridge -padx $riplWidgetPadx -width $riplTextWidth
	pack $frame.approx.lbl $frame.approx.val -side left -padx $riplWidgetPadx -pady $riplWidgetPady

	set riplMarrHildrethVar 1.0
	riplMarrHildrethVarProcess
}

# scaleBar event-handler
proc riplMarrHildrethVarProcess {} {
	global riplMarrHildrethVar riplMarrHildrethSize
	
	set riplMarrHildrethSize [expr 2*round(3.7*$riplMarrHildrethVar-0.5)-1]
	if {$riplMarrHildrethSize<1} {set riplMarrHildrethSize 1}
}

# scaleentry Enter event-handler
proc riplMarrHildrethVarEnter {} {
	global riplWin
	riplMarrHildrethVarProcess
	focus $riplWin
}

# Apply the filter by calling the C DLL function
proc riplMarrHildrethApply {} {
	global riplBase riplMarrHildrethVar
	$riplBase ripl marrhildreth $riplMarrHildrethVar
	return 1
}

##################################################
# Global Histogram Equalization
#
# Arguments required by function: none
##################################################

# Install widgets onto window
proc riplGlobalHEOptions {frame} {
}

proc riplGlobalHEApply {} {
	global riplBase
	$riplBase ripl globalhe
	return 1
}

##################################################
# SUBIMAGE-BASED ADAPTIVE HISTOGRAM EQUALIZATION
#
# Arguments required by function:
#		1.		The window size:		riplSiAHEWinSize
##################################################

# Install widgets onto window
proc riplSiAHEOptions {frame} {
	global riplWidgetPadx riplWidgetPady riplLabelWidth riplTextWidth
	global riplSiAHEWindow riplSiAHEWinType riplSiAHEPost
	
	frame $frame.win
	pack $frame.win -side top -fill x
	
	label $frame.win.label -text "Window Size"
	pack $frame.win.label -side left

	frame $frame.win.value
	pack $frame.win.value -side top -fill x
	scaleentry $frame.win.value.ent riplSiAHEWindow

	scaleBar $frame.win.scale riplSiAHEWindow 2 101 2 5 1
	$frame.win.scale.scale configure -digits 0
	
	frame $frame.wintype -relief sunken
	pack $frame.wintype -side top -fill x

	label $frame.wintype.label -text "Window Type" -width $riplLabelWidth
	radiobutton $frame.wintype.b0 -variable riplSiAHEWinType -value 0 -indicatoron 1 \
		-selectcolor "white" -text "Rectangular" -underline 0
	radiobutton $frame.wintype.b1 -variable riplSiAHEWinType -value 1 -indicatoron 1 \
		-selectcolor "white" -text "Hanning" -underline 0
	radiobutton $frame.wintype.b2 -variable riplSiAHEWinType -value 2 -indicatoron 1 \
		-selectcolor "white" -text "Hamming" -underline 1
	radiobutton $frame.wintype.b3 -variable riplSiAHEWinType -value 3 -indicatoron 1 \
		-selectcolor "white" -text "Triangular" -underline 0
	pack $frame.wintype.label \
		$frame.wintype.b0 \
		$frame.wintype.b1 \
		$frame.wintype.b2 \
		$frame.wintype.b3 \
		-side left -padx $riplWidgetPadx -pady $riplWidgetPady

	frame $frame.post -relief sunken
	pack $frame.post -side top -fill x

	label $frame.post.label -text "Post-equalization" -width $riplLabelWidth
	radiobutton $frame.post.b0 -variable riplSiAHEPost -value 1 -indicatoron 1 \
		-selectcolor "white" -text "Apply" -underline 0
	radiobutton $frame.post.b1 -variable riplSiAHEPost -value 0 -indicatoron 1 \
		-selectcolor "white" -text "Don't Apply" -underline 0
	pack $frame.post.label \
		$frame.post.b0 \
		$frame.post.b1 \
		-side left -padx $riplWidgetPadx -pady $riplWidgetPady

	set riplSiAHEWindow 1
	set riplSiAHEWinType 2
	set riplSiAHEPost 1
}

# scaleBar event-handler
proc riplSiAHEWindowProcess {} {
}

# scaleentry Enter event-handler
proc riplSiAHEWindowEnter {} {
	global riplWin
	focus $riplWin
}

# Apply the filter by calling the C DLL function
proc riplSiAHEApply {} {
	global riplBase riplSiAHEWindow riplSiAHEWinType riplSiAHEPost
	set cmdline "$riplBase ripl siahe "
	switch $riplSiAHEWinType {
		0	{
			# Rectangular
			append cmdline rect
		}
		1	{
			# Hanning
			append cmdline hanning
			}
		2	{
			# Hamming
			append cmdline hamming
			}
		3	{
			# Triangular
			append cmdline triang
			}
	}
	append cmdline " $riplSiAHEWindow"
	if {$riplSiAHEPost==1} {append cmdline " globalhe"}
	eval $cmdline
	return 1
}

##################################################
# Threshold Image
#
# Arguments required by function:
#		1.		Threshold value.
##################################################

# Install widgets onto window
proc riplThreshOptions {frame} {
	global riplThreshVal riplWidgetPadx
	global riplWidgetPady riplLabelWidth riplTextWidth
	global riplThreshMin riplThreshMax

	set riplThreshMin 0
	set riplThreshMax 255
	set riplThreshVal 128

	frame $frame.value
	pack $frame.value -side top -fill x -padx $riplWidgetPadx -pady $riplWidgetPady
	label $frame.value.vallabel -text "Grey Level" -width $riplLabelWidth
	wheel $frame.value.val riplThreshVal riplThreshMin riplThreshMax
	pack $frame.value.vallabel $frame.value.val \
		-side left -padx $riplWidgetPadx -pady $riplWidgetPady

}

proc riplThreshValProcess {} {
}

# Perform the convolution by calling C DLL function
proc riplThreshApply {} {
	global riplBase riplThreshVal

	$riplBase ripl threshold $riplThreshVal
	return 1
}

##################################################
# Zero Image
#
# Arguments required by function:
#		1.		Threshold value.
##################################################

# Install widgets onto window
proc riplZeroOptions {frame} {
	global riplZeroVal riplWidgetPadx
	global riplWidgetPady riplLabelWidth riplTextWidth
	global riplZeroMin riplZeroMax

	set riplZeroMin 0
	set riplZeroMax 255
	set riplZeroVal 128

	frame $frame.value
	pack $frame.value -side top -fill x -padx $riplWidgetPadx -pady $riplWidgetPady
	label $frame.value.vallabel -text "Grey Level" -width $riplLabelWidth
	wheel $frame.value.val riplZeroVal riplZeroMin riplZeroMax
	pack $frame.value.vallabel $frame.value.val \
		-side left -padx $riplWidgetPadx -pady $riplWidgetPady

}

proc riplZeroValProcess {} {
}

# Perform the convolution by calling C DLL function
proc riplZeroApply {} {
	global riplBase riplZeroVal

	$riplBase ripl zero $riplZeroVal
	return 1
}

##################################################
# Binary Morphological Operations
#
# Arguments required by function:
##################################################

# Install widgets onto window
proc riplBMorphOptions {frame} {
	global riplBMorphOp riplBMorphOps riplBMorphStruct riplBMorphStructs riplBMorphCols riplBMorphRows
	global riplBMorphMin riplBMorphMax riplBMorphStructFr riplBMorphVal
	global riplWidgetPadx riplWidgetPady riplLabelWidth riplTextWidth
	
	set riplBMorphOp -1
	set riplBMorphOps [list "Erode" "Dilate" "Open" "Close"]
	set riplBMorphStruct 1
	set riplBMorphStructs [list "Ones" "Cross" "Kite" "Diagonal"]
	set riplBMorphCols 5
	set riplBMorphRows 5
	set riplBMorphMin 1
	set riplBMorphMax 10

	frame $frame.op
	pack $frame.op -side top -fill x
	riplLBCreate "OPERATION" $frame.op riplBMorphOp riplBMorphOps [list]

	frame $frame.lbl
	pack $frame.lbl -side top -fill x -padx $riplWidgetPadx -pady $riplWidgetPady
	label $frame.lbl.lbl -text "STRUCTURING ELEMENT"
	pack $frame.lbl.lbl -side left -padx $riplWidgetPadx -pady $riplWidgetPady

	frame $frame.predef
	pack $frame.predef -fill x
	riplLBCreate "Predefined Type" $frame.predef riplBMorphStruct riplBMorphStructs [list]
	
	# Structuring element size parameters.
	frame $frame.size
	pack $frame.size -side top -fill x -padx $riplWidgetPadx -pady $riplWidgetPady
	label $frame.size.collabel -text Columns -width $riplLabelWidth
	wheel $frame.size.cols riplBMorphCols riplBMorphMin riplBMorphMax
	label $frame.size.rowlabel -text Rows -width $riplLabelWidth
	wheel $frame.size.rows riplBMorphRows riplBMorphMin riplBMorphMax
	pack $frame.size.collabel $frame.size.cols \
		$frame.size.rowlabel $frame.size.rows -side left -padx $riplWidgetPadx -pady $riplWidgetPady

	# Structuring element display.
	frame $frame.struct
	pack $frame.struct -side top -fill x -padx $riplWidgetPadx -pady $riplWidgetPady
	label $frame.struct.label -text "Structure" -width $riplLabelWidth
	pack $frame.struct.label -side left -fill x
	set riplBMorphStructFr $frame.struct.sub
	frame $riplBMorphStructFr
	pack $riplBMorphStructFr

	triple variable riplBMorphStruct w riplBMorphGenStruct

	riplBMorphTable
}

# Update the relevant structuring element to one of the built-in structures.
proc riplBMorphGenStruct {{a ""} {b ""} {c ""}} {
	global riplBMorphStruct riplBMorphRows riplBMorphCols riplBMorphVal

	switch $riplBMorphStruct {
		0	{
			# 'Ones'
			for {set i 0} {$i<$riplBMorphRows} {incr i} {
				for {set j 0} {$j<$riplBMorphCols} {incr j} {set riplBMorphVal(r${i}c${j}) 1}
			}
		}
		1	{
			# 'Cross'
			set halfrow [expr ceil($riplBMorphRows/2)]
			set halfcol [expr ceil($riplBMorphCols/2)]
			for {set i 0} {$i<$riplBMorphRows} {incr i} {
				for {set j 0} {$j<$riplBMorphCols} {incr j} {
					if {$i==$halfrow || $j==$halfcol} {
						set riplBMorphVal(r${i}c${j}) 1
					} else {
						set riplBMorphVal(r${i}c${j}) 0
					}
				}
			}
		}
		2	{
			# 'Kite'
			for {set i 0} {$i<$riplBMorphRows} {incr i} {
				for {set j 0} {$j<$riplBMorphCols} {incr j} {set riplBMorphVal(r${i}c${j}) 0}
			}
			set halfrow [expr round($riplBMorphRows/2)]
			set halfcol [expr ceil($riplBMorphCols/2)]
			set range [expr int(($halfrow<$halfcol) ? $halfrow:$halfcol)]
			for {set i 0; set k [expr $riplBMorphRows-1]} {$i<$range} {incr i; incr k -1} {
				for {set j [expr $range-$i]} {$j<=[expr $range+$i]} {incr j} {
					set riplBMorphVal(r${i}c${j}) 1
					set riplBMorphVal(r${k}c${j}) 1
				}
			}
			if {[expr $range*2]<$riplBMorphRows} {
				# Odd number of rows.
				for {set j [expr $range-$i]} {$j<=[expr $range+$i]} {incr j} {set riplBMorphVal(r${i}c${j}) 1}
			}
		}
		3	{
			# 'Diagonal'
			for {set i 0} {$i<$riplBMorphRows} {incr i} {
				for {set j 0} {$j<$riplBMorphCols} {incr j} {
					if {$i==$j} {
						set riplBMorphVal(r${i}c${j}) 1
					} else {
						set riplBMorphVal(r${i}c${j}) 0
					}
				}
			}
		}
	}
}

# Update the number of columns in the erosion structuring element table.
proc riplBMorphColsProcess {} {
	global riplWin riplBMorphStructFr

 	wm withdraw $riplWin
	destroy $riplBMorphStructFr

	frame $riplBMorphStructFr
	pack $riplBMorphStructFr

	riplBMorphTable
	wm deiconify $riplWin
}

# Update the number of rows in the erosion structuring element table.
proc riplBMorphRowsProcess {} {
	global riplWin riplBMorphStructFr

 	wm withdraw $riplWin
	destroy $riplBMorphStructFr

	frame $riplBMorphStructFr
	pack $riplBMorphStructFr

	riplBMorphTable
	wm deiconify $riplWin
}

# Create the erosion/dilation structuring element table.
proc riplBMorphTable {} {
	global riplWin riplBMorphStructFr riplBMorphRows riplBMorphCols riplBMorphVal riplWidgetPadx

	for {set i 0} {$i<$riplBMorphRows} {incr i} {
		frame $riplBMorphStructFr.r${i}
		pack $riplBMorphStructFr.r${i} -side top -fill x
		for {set j 0} {$j<$riplBMorphCols} {incr j} {
			entry $riplBMorphStructFr.r${i}.c${j} -textvariable riplBMorphVal(r${i}c${j}) -width 5
			pack $riplBMorphStructFr.r${i}.c${j} -side left -padx $riplWidgetPadx
			set riplBMorphVal(r${i}c${j}) 1
		}
	}
}

# Perform the convolution by calling C DLL function
proc riplBMorphApply {} {
	global riplAppName riplBase riplBMorphOp
	global riplBMorphCols riplBMorphRows riplBMorphVal

	if {$riplBMorphOp<0} {
		bell
		tk_messageBox -icon warning -type ok -title $riplAppName -message \
			"You must select a valid operation first!"
		return 0
	} else {
		set cmdline "$riplBase ripl binmorph "
		switch $riplBMorphOp {
		0	{append cmdline "erosion"}
		1	{append cmdline "dilation"}
		2	{append cmdline "opening"}
		3	{append cmdline "closing"}
		}
		# Add on erosion structure information.
		append cmdline " $riplBMorphCols $riplBMorphRows"
		for {set i 0} {$i<$riplBMorphRows} {incr i} {
			for {set j 0} {$j<$riplBMorphCols} {incr j} {
				if {$riplBMorphVal(r${i}c${j})==0} {append cmdline " 0"} else {append cmdline " 1"}
			}
		}
		eval $cmdline
		return 1
	}
}

##################################################
# Grey-Level Morphological Operations
#
# Arguments required by function:
##################################################

# Install widgets onto window
proc riplGLMorphOptions {frame} {
	global riplGLMorphOp riplGLMorphOps riplGLMorphStruct riplGLMorphStructs riplGLMorphCols riplGLMorphRows
	global riplGLMorphMin riplGLMorphMax riplGLMorphStructFr riplGLMorphVal
	global riplWidgetPadx riplWidgetPady riplLabelWidth riplTextWidth
	
	set riplGLMorphOp -1
	set riplGLMorphOps [list "Erode" "Dilate" "Open" "Close"]
	set riplGLMorphStruct 1
	set riplGLMorphStructs [list "Ones" "Cross" "Kite" "Diagonal"]
	set riplGLMorphCols 5
	set riplGLMorphRows 5
	set riplGLMorphMin 1
	set riplGLMorphMax 10

	frame $frame.op
	pack $frame.op -side top -fill x
	riplLBCreate "OPERATION" $frame.op riplGLMorphOp riplGLMorphOps [list]

	frame $frame.lbl
	pack $frame.lbl -side top -fill x -padx $riplWidgetPadx -pady $riplWidgetPady
	label $frame.lbl.lbl -text "STRUCTURING ELEMENT"
	pack $frame.lbl.lbl -side left -padx $riplWidgetPadx -pady $riplWidgetPady

	frame $frame.predef
	pack $frame.predef -fill x
	riplLBCreate "Predefined Type" $frame.predef riplGLMorphStruct riplGLMorphStructs [list]
	
	# Structuring element size parameters.
	frame $frame.size
	pack $frame.size -side top -fill x -padx $riplWidgetPadx -pady $riplWidgetPady
	label $frame.size.collabel -text Columns -width $riplLabelWidth
	wheel $frame.size.cols riplGLMorphCols riplGLMorphMin riplGLMorphMax
	label $frame.size.rowlabel -text Rows -width $riplLabelWidth
	wheel $frame.size.rows riplGLMorphRows riplGLMorphMin riplGLMorphMax
	pack $frame.size.collabel $frame.size.cols \
		$frame.size.rowlabel $frame.size.rows -side left -padx $riplWidgetPadx -pady $riplWidgetPady

	# Structuring element display.
	frame $frame.struct
	pack $frame.struct -side top -fill x -padx $riplWidgetPadx -pady $riplWidgetPady
	label $frame.struct.label -text "Structure" -width $riplLabelWidth
	pack $frame.struct.label -side left -fill x
	set riplGLMorphStructFr $frame.struct.sub
	frame $riplGLMorphStructFr
	pack $riplGLMorphStructFr

	triple variable riplGLMorphStruct w riplGLMorphGenStruct

	riplGLMorphTable
}

# Update the relevant structuring element to one of the built-in structures.
proc riplGLMorphGenStruct {{a ""} {b ""} {c ""}} {
	global riplGLMorphStruct riplGLMorphRows riplGLMorphCols riplGLMorphVal

	switch $riplGLMorphStruct {
		0	{
			# 'Ones'
			for {set i 0} {$i<$riplGLMorphRows} {incr i} {
				for {set j 0} {$j<$riplGLMorphCols} {incr j} {set riplGLMorphVal(r${i}c${j}) 1}
			}
		}
		1	{
			# 'Cross'
			set halfrow [expr ceil($riplGLMorphRows/2)]
			set halfcol [expr ceil($riplGLMorphCols/2)]
			for {set i 0} {$i<$riplGLMorphRows} {incr i} {
				for {set j 0} {$j<$riplGLMorphCols} {incr j} {
					if {$i==$halfrow || $j==$halfcol} {
						set riplGLMorphVal(r${i}c${j}) 1
					} else {
						set riplGLMorphVal(r${i}c${j}) 0
					}
				}
			}
		}
		2	{
			# 'Kite'
			for {set i 0} {$i<$riplGLMorphRows} {incr i} {
				for {set j 0} {$j<$riplGLMorphCols} {incr j} {set riplGLMorphVal(r${i}c${j}) 0}
			}
			set halfrow [expr round($riplGLMorphRows/2)]
			set halfcol [expr ceil($riplGLMorphCols/2)]
			set range [expr int(($halfrow<$halfcol) ? $halfrow:$halfcol)]
			for {set i 0; set k [expr $riplGLMorphRows-1]} {$i<$range} {incr i; incr k -1} {
				for {set j [expr $range-$i]} {$j<=[expr $range+$i]} {incr j} {
					set riplGLMorphVal(r${i}c${j}) 1
					set riplGLMorphVal(r${k}c${j}) 1
				}
			}
			if {[expr $range*2]<$riplGLMorphRows} {
				# Odd number of rows.
				for {set j [expr $range-$i]} {$j<=[expr $range+$i]} {incr j} {set riplGLMorphVal(r${i}c${j}) 1}
			}
		}
		3	{
			# 'Diagonal'
			for {set i 0} {$i<$riplGLMorphRows} {incr i} {
				for {set j 0} {$j<$riplGLMorphCols} {incr j} {
					if {$i==$j} {
						set riplGLMorphVal(r${i}c${j}) 1
					} else {
						set riplGLMorphVal(r${i}c${j}) 0
					}
				}
			}
		}
	}
}

# Update the number of columns in the erosion structuring element table.
proc riplGLMorphColsProcess {} {
	global riplWin riplGLMorphStructFr

 	wm withdraw $riplWin
	destroy $riplGLMorphStructFr

	frame $riplGLMorphStructFr
	pack $riplGLMorphStructFr

	riplGLMorphTable
	wm deiconify $riplWin
}

# Update the number of rows in the erosion structuring element table.
proc riplGLMorphRowsProcess {} {
	global riplWin riplGLMorphStructFr

 	wm withdraw $riplWin
	destroy $riplGLMorphStructFr

	frame $riplGLMorphStructFr
	pack $riplGLMorphStructFr

	riplGLMorphTable
	wm deiconify $riplWin
}

# Create the erosion/dilation structuring element table.
proc riplGLMorphTable {} {
	global riplWin riplGLMorphStructFr riplGLMorphRows riplGLMorphCols riplGLMorphVal riplWidgetPadx

	for {set i 0} {$i<$riplGLMorphRows} {incr i} {
		frame $riplGLMorphStructFr.r${i}
		pack $riplGLMorphStructFr.r${i} -side top -fill x
		for {set j 0} {$j<$riplGLMorphCols} {incr j} {
			entry $riplGLMorphStructFr.r${i}.c${j} -textvariable riplGLMorphVal(r${i}c${j}) -width 5
			pack $riplGLMorphStructFr.r${i}.c${j} -side left -padx $riplWidgetPadx
			set riplGLMorphVal(r${i}c${j}) 1
		}
	}
}

# Perform the convolution by calling C DLL function
proc riplGLMorphApply {} {
	global riplAppName riplBase riplGLMorphOp
	global riplGLMorphCols riplGLMorphRows riplGLMorphVal

	if {$riplGLMorphOp<0} {
		bell
		tk_messageBox -icon warning -type ok -title $riplAppName -message \
			"You must select a valid operation first!"
		return 0
	} else {
		set cmdline "$riplBase ripl glmorph "
		switch $riplGLMorphOp {
		0	{append cmdline "erosion"}
		1	{append cmdline "dilation"}
		2	{append cmdline "opening"}
		3	{append cmdline "closing"}
		}
		# Add on erosion structure information.
		append cmdline " $riplGLMorphCols $riplGLMorphRows"
		for {set i 0} {$i<$riplGLMorphRows} {incr i} {
			for {set j 0} {$j<$riplGLMorphCols} {incr j} {
				if {$riplGLMorphVal(r${i}c${j})==0} {append cmdline " 0"} else {append cmdline " 1"}
			}
		}
		eval $cmdline
		return 1
	}
}

##################################################
# Extract Bit Plane from Image
#
# Arguments required by function:
#		1.		Bit plane number.
##################################################

# Install widgets onto window
proc riplPlaneOptions {frame} {
	global riplPlaneVal riplWidgetPadx
	global riplWidgetPady riplLabelWidth riplTextWidth
	global riplPlaneMin riplPlaneMax

	set riplPlaneMin 0
	set riplPlaneMax 7
	set riplPlaneVal 0

	frame $frame.value
	pack $frame.value -side top -fill x -padx $riplWidgetPadx -pady $riplWidgetPady
	label $frame.value.vallabel -text "Bit Plane" -width $riplLabelWidth
	wheel $frame.value.val riplPlaneVal riplPlaneMin riplPlaneMax
	pack $frame.value.vallabel $frame.value.val \
		-side left -padx $riplWidgetPadx -pady $riplWidgetPady

}

proc riplPlaneValProcess {} {
}

# Perform the convolution by calling C DLL function
proc riplPlaneApply {} {
	global riplBase riplPlaneVal

	$riplBase ripl plane $riplPlaneVal
	return 1
}

##################################################
# Mask Image
#
# Arguments required by function:
#		1.		Threshold value.
##################################################

# Install widgets onto window
proc riplMaskOptions {frame} {
	global riplMaskVal riplWidgetPadx
	global riplWidgetPady riplLabelWidth riplTextWidth
	global riplMaskMin riplMaskMax

	set riplMaskMin 0
	set riplMaskMax 255
	set riplMaskVal 0

	frame $frame.value
	pack $frame.value -side top -fill x -padx $riplWidgetPadx -pady $riplWidgetPady
	label $frame.value.vallabel -text "Mask Value" -width $riplLabelWidth
	wheel $frame.value.val riplMaskVal riplMaskMin riplMaskMax
	pack $frame.value.vallabel $frame.value.val \
		-side left -padx $riplWidgetPadx -pady $riplWidgetPady

}

proc riplMaskValProcess {} {
}

# Perform the convolution by calling C DLL function
proc riplMaskApply {} {
	global riplBase riplMaskVal

	$riplBase ripl mask $riplMaskVal
	return 1
}

##################################################
# Execute RIPL Command
# Arguments required by function:
#		1.		The command line.
##################################################

# Install widgets onto window.
proc riplRiplOptions {frame} {
	global riplRiplCommand riplTextWidth
	label $frame.label -text "Ripl Command"
	pack $frame.label -side left

	frame $frame.entry
	pack $frame.entry -side top -fill x
	entry $frame.entry.ent -textvariable riplRiplCommand -width 40
	pack $frame.entry.ent -side left -fill x
}

# Run the command line.
proc riplRiplApply {} {
	global riplBase riplRiplCommand
	eval $riplBase ripl $riplRiplCommand
	return 1
}

##################################################
# Listbox entry initializer
##################################################

proc riplSetupTransformLists {oplist opproclist} {
	global $oplist $opproclist
	set $oplist [list "Gaussian Smoothing Filter" \
		"Convolution with 2D Kernel" \
		"Sobel Edge Detection" \
		"Median Filter" \
		"Marr-Hildreth Edge Detection" \
		"Global Histogram Equalization" \
		"Subimage-Based Adaptive HE" \
		"Threshold" \
		"Zero" \
		"Binary Morphological Operations" \
		"Grey-Level Morphological Operations" \
		"Extract Bit Plane" \
		"Apply Mask" \
		"Run Ripl Command"]
	set $opproclist [list Gauss Conv Sobel Median \
		MarrHildreth GlobalHE SiAHE Thresh \
		Zero BMorph GLMorph Plane \
		Mask Ripl]
}

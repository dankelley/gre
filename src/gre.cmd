# -*- mode: gre -*-
# version 0.2.2


#* Convert z=z(x,y) data to grid for contouring.
#* By default, the method is Barnes, a form of gaussian smoothing, with 
#* parameters determined from data.  The neighbor method may be useful 
#* if the data are nearly on a grid already.
#* @param $xr smoothing radius in x-direction
#* @param $yr smoothing radius in y-direction
#* @param $gamma square of radius-reduction factor per iteration
#* @param $iterations number of iterations
cmd convert columns to grid (;@) { # [neighbor|{barnes [$xr $yr $gamma $iterations]}]
    execute "C" bool convert_columns_to_gridCmd();
}


#* Convert grid to image
#* @param $w width of image in pixels
#* @param $h height of image in pixels
cmd convert grid to image (;$$$) { # [size $w $h]
    execute "C" bool convert_grid_to_imageCmd();
}


#* Delete something
cmd delete ($;$$) { # {columns [{randomly $fraction}|{where missing}] }|grid|{[x|y] scale}
    execute "C" bool deleteCmd();
}

#* Draw an arc (fraction of a circle)
#* @param filled means to fill the arc with current color
#* @param $xc x-coordinate of center of circle, in cm on page
#* @param $yc y-coordinate of center of circle, in cm on page
#* @param $r  radius of circle, in cm
#* @param $angle1 start of arc, in degrees counterclockwise from East
#* @param $angle2 end of arc, in degrees counterclockwise from East
cmd draw arc ($$$$$;$) { # [filled] $xc $yc $r $angle1 $angle2
    execute "C" bool draw_arcCmd();
}

#* Draw arrow from ($x0,$y0) pointing to ($x1,$y1) (each in user-coords)
cmd draw arrow ($$$$$$;$) { # from $x0 $y0 to $x1 $y1 [cm]
    execute "C" bool draw_arrowCmd();
}
#* Draw arrows emanating from (x,y) and extending as indicated by (u,v)
cmd draw arrows (;) {
    execute "C" bool draw_arrowsCmd();
}
#* Draw axes, in four-sided box with x-axis at bottom, y-axis at left
#* @param $type "none": don't draw axes; 0: label left and bottom; 1: no tics at top and right; 2: frame, no tics or labels; "frame": frame, no tics or labels
cmd draw axes (;$) {
    execute "C" bool draw_axesCmd();
}

#* Draw rectangular box connecting given lower-left and upper-right points
#* @param $ll_x x-coordinate of lower-left corner
#* @param $ll_y y-coordinate of lower-left corner
#* @param $ur_x x-coordinate of upper-right corner
#* @param $ur_y y-coordinate of upper-right corner
cmd draw box ($$$$;$$) { # [filled] $ll_x $ll_y $ur_x $ur_y [cm]
    execute "C" bool draw_boxCmd();
}

#cmd draw circle with radius ($$$$) { # $r_cm at $x $y
#    execute "C" bool draw_circleCmd();
#}

#* Draw contour(s) based on data in grid
cmd draw contour (;$$$$$) { # [{$c [unlabelled|{labelled $lab}]}|{$min $max $inc [$inc_unlabelled] [unlabelled]}]
    execute "C" bool draw_contourCmd();
}

#* Draw contour(s) based on data in grid
cmd draw filled contour (;$$$$$) { # [{$c [unlabelled|{labelled $lab}]}|{$min $max $inc [$inc_unlabelled] [unlabelled]}]
    execute "C" bool draw_filled_contourCmd();
}


#* Draw curve by connecting points in the x and y columns.
#* With no options a line is simply drawn (after drawing axes if they
#* are required), but one may also draw an 'overlying' curve (done by
#* first drawing a white thick curve under the curve) or draw a 
#* 'filled' curve, either by connecting first and last points or by
#* filling to indicated x or y value.
cmd draw curve (;$$$$) { # [overlying|{filled [{to x $x}]|[{to y $y}]}
    execute "C" bool draw_curveCmd();
}

#* Draw plus-signs at locations where grid data are non-missing.
cmd draw grid (;) {
    execute "C" bool draw_gridCmd();
}

#* Draw color or black/white image.
cmd draw image (;) { #
    execute "C" bool draw_imageCmd();
}

#* Draw curve of constant density for temperature-salinity diagram,
#* labelled with the density value unless told otherwise.
#* @param $density = density of curve
#* @param $P_sigma = reference pressure for density @default 0dbar
#* @param $P_theta = reference pressure for temperature @default 0dbar
cmd draw isopycnal ($;$$$) { # [unlabelled] $density [$P_sigma $P_theta]
    execute "C" bool draw_isopycnalCmd();
}

#* Draw a label at indicated location, possibly rotated
cmd draw label ($$$;$$$$) { # $label [centered|rightjustified|whiteunder] at $x $y [cm] [rotated $deg]
    execute "C" bool draw_labelCmd();
}

#* Draw line connecting two points specified in user coordinates
cmd draw line from ($$$$$;$) { # $x0 $y0 to $x1 $y1 [cm]
    execute "C" bool draw_line_fromCmd();
}

#* Draw filled polygon, possibly filled
cmd draw polygon (@) { # [filled] <x-y pairs> [cm]
    execute "C" bool draw_polygonCmd();
}

#* Draw symbol at indicated location
#* @param $type is type @default bullet
cmd draw symbol (;$$$$$) { # [$type [at $x $y [cm]]]
    execute "C" bool draw_symbolCmd();
}

#* Draw a title above plot
cmd draw title ($) { # $title
    execute "C" bool draw_titleCmd();
}

#* Draw x-axis at specified location
cmd draw x axis (;$$$$) { # [at bottom|top|{$y [cm]} [lower|upper]]
    execute "C" bool draw_x_axisCmd();
}

#* Draw Tukey boxplot horizontally
cmd draw x box plot at ($;$$) { # $y [size $cm]
    execute "C" bool draw_x_box_plotCmd();
}

#* Draw y-axis at specified location
cmd draw y axis (;$$$$) { # [at left|right|{$x [cm]} [left|right]]
    execute "C" bool draw_y_axisCmd();
}

#* Draw Tukey boxplot horizontally
cmd draw y box plot at ($;$$) { # $x [size $cm]
    execute "C" bool draw_y_box_plotCmd();
}

#* End this page of PostScript output, and begin another page
cmd new page (;) {
    execute "C" bool new_pageCmd();
}

#* Insert comment in PostScript file
cmd pscomment ($;) {
    execute "C" bool pscommentCmd();
}

#* Perform linear regression on (x,y) data, y=$_coeff0+$_coeff1*x
#* Intercept and its 5 percent confidence interval stored in $_coeff0 and $_coeff0_sig
#* Slope and its 95 percent confidence interval stored in $_coeff1 and $_coeff1_sig
#* Deviation of regression (unexplained variance) stored in $_regression_deviation
cmd regress ($$$) { # {y vs x}|{x vs y}
    execute "C" bool regressCmd();
}

#* Reorder the column data randomly
cmd reorder columns randomly (;) {
    execute "C" bool reorder_columns_randomlyCmd();
}


#* Set size of arrowheads
#* @param $size arrowhead half-width @unit cm @default 0.2
#* @param $p percentage
cmd set arrow size ($;$$$$) { # $size|{as $p percent of length}|default
    execute "C" bool set_arrow_sizeCmd();
}

#* Set type of arrowhead
#* @param $type 0 for 3-strokes, 1 for outline, 2 for filled swept-back @default 0
cmd set arrow type ($) { # $type
    execute "C" bool set_arrow_typeCmd();
}
#* Control clipping
cmd set clip ($;@) { # off|on|{rectangle $xl $yb $xr $yt}|{to curve}
    execute "C" bool set_clipCmd();
}

#* Set pen color to named color, or a color in RGB or HSV space.
#* In RGB and HSV spaces, the parameters must be in the range 0 to 1.
#* @param $name
#* @param $r red if in rgb mode
#* @param $g green if in rgb mode
#* @param $b blue if in rgb mode
#* @param $h hue if in hsb mode
#* @param $s saturation if in hsb mode
#* @param $v value if in hsb mode
cmd set color ($;$$$) { # $name|{rgb $r $g $b}|{hsv $h $s $v}
    execute "C" bool set_colorCmd();
}

#* Set positions for labels on contours and isopycnals
#* @param $start_cm distance of label from start of line
#* @param $between_cm distance between subsequent labels
cmd set contour label position ($$) { # $start_cm $between_cm
    execute "C" bool set_contour_label_positionCmd();
}
#* Set to dashed line for drawing curves, contours, etc.
#* @param $type style of dash to use (range 0 to 15) @default 2
#* @param LIST space-separated list of dashlen_cm, blanklen_cm, dashlen_cm, ...
cmd set dash (;@) { # on|off|$type|LIST
    execute "C" bool set_dashCmd();
}
#* Set flag
#* @param $flag
cmd set flag ($;$) { # $name [on|off]
    execute "C" bool set_flagCmd();
}
#* Set text font size in points
#* @param $size of font @unit point @default 12 in $_font_size
cmd set font size ($) { # {$size [cm]}|default
    execute "C" bool set_font_sizeCmd();
}
#* Set text font
#* @param $name of font @default Helvetica
cmd set font to ($) { # $name
    execute "C" bool set_font_toCmd();
}

#* Set graylevel of pen
#* @param $brightness of ink with 0 for black and 1 for white @default 0
cmd set graylevel ($) { # $brightness
    execute "C" bool set_graylevelCmd();
}

#* Specify colorscale (or grayscale) for image
cmd set image colorscale ($$$$$$$$$$;$$) { # hsv|rgb $h|$r $s|$g $v|$b $im_low hsv|rgb $h|$r $s|$g $v|$b $im_high [increment $inc]
    execute "C" bool set_image_colorscaleCmd();
}

#* Specify min and max values image can hold (in user, z, units)
#* @param $min minimum value
#* @param $max maximum value
cmd set image range ($$) { # $min $max
    execute "C" bool set_image_rangeCmd();
}

#* Set caps on ends of lines (default square butt)
cmd set line cap ($$) { # {square butt}|{round overhang}|{square overhang}
    execute "C" bool set_line_capCmd();
}
#* Set style of joining line-segment intersections
cmd set line join ($) { # mitered|round|bevelled
    execute "C" bool set_line_joinCmd();
}

#* Set width of line
#* @param $width @unit point @default 0.709 in $_linewidth
#* @param $name name of rapidograph pen, in list 6x0, 4x0, 3x0, 00, 0, 1, 2, 2.5, 3, 3.5, 4, 6, 7
cmd set line width ($;$$) { # [axis|symbol|all] $width|default|{rapidograph $name}
    execute "C" bool set_line_widthCmd();
}
#* Set map projection
#* @param $specifics is what would be given to 'proj' unix command
cmd set map projection ($) { # $specifics
    execute "C" bool set_map_projectionCmd();
}
#* Set missing-value code for data
#* @param $value to be considered missing @default 1.0e22 in $_missing_value
cmd set missing value ($) { # $value
    execute "C" bool set_missing_valueCmd();
}

#* Set page style to landscape or portrait
cmd set page ($) { # "landscape" | "portrait"
    execute "C" bool set_pageCmd();
}

#* Set radius of symbols
#* @param $radius radius @unit cm @default 0.1
cmd set symbol size ($) { # $radius|default
    execute "C" bool set_symbol_sizeCmd();
}
#* Set length of major tics on axes
#* @param $length @unit cm @default 0.2
cmd set tic size ($) { # $length
    execute "C" bool set_tic_sizeCmd();
}
#* Control whether axis tics point into plot region (default out)
#* @param $direction ("in" or "out") @default 0 in $_tic_direction
cmd set tics ($;) {             # $direction
    execute "C" bool set_ticsCmd();
}

#* Set scale for x-component of arrows.
#* @param $cm_per_unit distance on page per unit of u
cmd set u scale ($;$) { # $cm_per_unit|{as x}
    execute "C" bool set_u_scaleCmd();
}
#* Set scale for y-component of arrows.
#* @param $cm_per_unit distance on page per unit of v
cmd set v scale ($;$) { # $cm_per_unit|{as y}
    execute "C" bool set_v_scaleCmd();
}

#* Set span of x-axis, and (optionally) give info on tics
#* @param $left is number at left end of axis
#* @param $right is number at right end of axis
#* @param $inc is increment between numbered tics
#* @param $incinc  is increment between unnumbered tics
cmd set x axis ($;$$$) { # increasing|decreasing"|{$left $right [$inc [$incinc]]}
    execute "C" bool set_x_axisCmd();
}
#* Set format for numbers on x-axis.
#* @param $fmt for numbers in C notation @default %g
cmd set x format ($) { #  $fmt
    execute "C" bool set_x_formatCmd();
}
#* Set x component of grid used for contouring
#* @param $left number at left of grid
#* @param $right number at right of grid
#* @param $inc increment from point to point in grid
cmd set x grid ($$$) { # $left $right $inc
    execute "C" bool set_x_gridCmd();
}
#* Set margin to left of x-axis
#* @param $space in centimeters @default 6
cmd set x margin ($) { #  $space|default
    execute "C" bool set_x_marginCmd();
}
#* Set name of x-axis.
#* @param $name of axis @default x
cmd set x name ($) { #  $name
    execute "C" bool set_x_nameCmd();
}

#* Set width of x-axis
#* @param $width of axis @unit cm @default 10
cmd set x size ($) { # $width
    execute "C" bool set_x_sizeCmd();
}

# NOTE FOR GRI USERS: 'map' not coded yet
#* Specify that x-axis will be linear (the default)
cmd set x type linear (;) {
    execute "C" bool set_x_type_linearCmd();
}
#* Specify that x-axis will be logarithmic
cmd set x type log (;) {
    execute "C" bool set_x_type_logCmd();
}

#* Set span of y-axis, and (optionally) give info on tics
#* @param $bottom is number at bottom of axis
#* @param $top is number at top of axis
#* @param $inc is increment between numbered tics
#* @param $incinc  is increment between unnumbered tics
cmd set y axis ($;$$$) { # "increasing|decreasing"|{$bottom $top [$inc [$incinc]]}
    execute "C" bool set_y_axisCmd();
}
#* Set format for numbers on y-axis.
#* @param $fmt for numbers in C notation @default %g
cmd set y format ($) { #  $fmt
    execute "C" bool set_y_formatCmd();
}
#* Set y component of grid used for contouring
#* @param $bottom number at bottom of grid
#* @param $top number at top of grid
#* @param $inc increment from point to point in grid
cmd set y grid ($$$) { # $bottom $top $inc
    execute "C" bool set_y_gridCmd();
}
#* Set margin below y-axis
#* @param $space in centimeters @default 6
cmd set y margin ($) { #  $space|default
    execute "C" bool set_y_marginCmd();
}
#* Set name of y-axis.
#* @param $name of axis @default y
cmd set y name ($) { # $name
    execute "C" bool set_y_nameCmd();
}
#* Set height of y-axis
#* @param $height of axis @unit cm @default 10
cmd set y size ($) { # $height
    execute "C" bool set_y_sizeCmd();
}

#* Specify that y-axis will be linear (the default)
cmd set y type linear (;) {
    execute "C" bool set_y_type_linearCmd();
}
#* Specify that y-axis will be logarithmic
cmd set y type log (;) {
    execute "C" bool set_y_type_logCmd();
}


#* Adjust x size, given axes ranges, to yield neutral map geometry.
cmd resize x as map (;) {
    set x size ($_ysize * ($_xright - $_xleft) / ($_ytop - $_ybottom)
		* abs(cos($_pi / 180 * 0.5 * ($_ybottom + $_ytop))));
}

#* Adjust y size, given axes ranges, to yield neutral map geometry.
cmd resize y as map (;) {
    set y size ($_xsize * ($_ytop - $_ybottom) / ($_xright - $_xleft)
		/ abs(cos($_pi / 180 * 0.5 * ($_ybottom + $_ytop))));
}


# The following are gre commnds that are not defined in as valid declarations
# in this file.  They are listed here for the benefit of Emacs' gre-mode:

#* Print information about current variables
#* @cmd print variables (;) { #

#* Print information about the current grid
#* @cmd print grid (;) { #

#* Print information about the current image
#* @cmd print image (;) { #

#* Print a message and stop program execution (exiting with code 255)
#* @param $message message to be printed
#* @cmd die ($;) { # $message

#* Open a file
#* @param $HANDLE handle to be used for file
#* @param $filename name of file to open
#* @param $type type of file to open, if not ascii (e.g. 'netCDF')
#* @cmd open (@) { # [$HANDLE] $filename [$type]

#* Read column data (perhaps to be plotted with `draw curve')
#* @cmd read columns (;@) { # [HANDLE] LIST [ROWS]
#* @param LIST indicates ordering and column-numbers for (x,y,...)

#* Rewind file with named handle, or with ANONYMOUS handle
#* @param $HANDLE handle of file to rewind
#* @cmd rewind (;$) { # [$HANDLE]

#* Loop while condition is true
#* @param $condition continue looping when this is nonzero
#* @cmd while ($;) { # $condition

#* Read grid x-component
#* @param $columns
#* @cmd read grid x (;@) { # [HANDLE] [$columns]

#* Read grid y-component
#* @param $rows
#* @cmd read grid y (;@) { # [HANDLE] [$rows]

#* Read grid data
#* @param $rows
#* @param $columns
#* @cmd read grid data (;@) { # [HANDLE] [$columns $rows]

#* Loop until condition is true
#* @param $condition stop looping when this is nonzero
#* @cmd until ($;) { # $condition

#* @variable $_ as in perl @default ""
#* @variable $_[] as in perl @default ""
#* @variable $_x[] stores x-column
#* @variable $_y[] stores y-column
#* @variable $_z[] stores z-column
#* @variable $_u[] stores u-column
#* @variable $_v[] stores v-column
#* @variable $_arrow_size width of arrow heads @unit cm @default 0.2
#* @variable $_exit_status relic from gri, may not exist
#* @variable $_font_size font size @unit point @default 12
#* @variable $_graylevel pen brightness, 0 = black @default 0
#* @variable $_missing_value missing value @default 
#* @variable $_symbol_size radius of symbols @default 0.1
#* @variable $_linewidth width of curves @unit point @default 0.709
#* @variable $_linewidth_axis width of axis lines @unit point @default 0.369
#* @variable $_linewidth_symbol width of symbol lines @default 0.369
#* @variable $_xmargin space to left of axes @unit cm @default 6
#* @variable $_xsize width of x axis @unit cm @default 10
#* @variable $_xleft x-value at left limit of x-axis
#* @variable $_xright x-value at right limit of x-axes
#* @variable $_ymargin space below axes @unit cm @default 6
#* @variable $_ytop y-value at top of y-axis
#* @variable $_ybottom y-value at bottom of y-axis
#* @variable $_ysize height of y axes @unit cm @default 10
#* @variable $_tic_direction direction of tics (0 means out) @default 0
#* @variable $_tic_size size of tics on axes @unit cm @default 0.2
#* @variable $_red red-ness of ink in range 0 to 1
#* @variable $_green green-ness of ink in range 0 to
#* @variable $_blue blue-ness of ink in range 0 to 1
#* @variable $_eof end of file indicator @default 0
#* @variable $_version version of gre
#* @variable $_command_file name of command file

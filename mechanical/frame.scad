include <scad-common/scad-common.scad>
include <BOSL2/std.scad>
include <BOSL2/shapes2d.scad>

$fn = $preview ? 32 : 128;

led_spacing = 1000/30;
row_count = 16;
paper_slot_thickness = 0.4*6;  // 65lb cardstock is ~10pt=254um
// text_height_width_ratio = 2; // letters are twice as tall, on average, as they are wide. Ish. By font
chore_label_length = 8.5*25.4; // cut from 8.5x11 paper
bezel_thickness = 15; // this is TOTAL bezel thickness
bezel_structural_thickness = 2.4;
bezel_depth = 4+paper_slot_thickness;
bezel_triangle_height = 2;
bezel_lock_length = 8;
bezel_lock_thickness = 2;
bezel_lock_bar_length = 5;
bezel_lock_offset = 0; // distance from bottom
bezel_lock_tolerance = 0.2;

button_diameter = 12;
pixel_hole_diameter = 10;
center_bar_width = 40;
center_bar_wall_thickness = m2dot5_heatset_mount_min_diameter + 1;

// derived parameters
total_bezel = 2*bezel_thickness + bezel_structural_thickness;
chore_label_height = led_spacing-bezel_thickness;
echo("chore label height: ", chore_label_height);
paper_slot_depth = (bezel_thickness-bezel_structural_thickness)/2;
echo("paper slot depth: ", paper_slot_depth);
center_bar_height = bezel_depth + bezel_triangle_height;

// geometry
module bezel()
{          
    translate([0, 0, bezel_lock_bar_length/2+bezel_lock_offset])
        rotate([-90, 0, 0])
            bezel_locking_tab();
    translate([0, -chore_label_length, bezel_lock_bar_length/2+bezel_lock_offset])
        rotate([90, 0, 0])
            bezel_locking_tab();
    rotate([90, 0, 0])
        mirror_copy([1, 0, 0])
            translate([-bezel_thickness/2, 0, 0])
                difference()
                {
                    linear_extrude(chore_label_length)
                    {
                        square([bezel_thickness/2, bezel_depth]);
                            translate([bezel_thickness/2, bezel_depth, 0])
                                    mirror([1, 0, 0])
                                        right_triangle([bezel_thickness/2, bezel_triangle_height]);
                    }
                        translate([0, (bezel_depth-paper_slot_thickness)/2, 0])
                            #cube([paper_slot_depth, paper_slot_thickness, chore_label_length]);
                }
}

module bezel_locking_tab(negative=false)
{
    offset_ = negative ? bezel_lock_tolerance : 0;
    linear_extrude(bezel_lock_length)
    {
        square([bezel_lock_thickness+2*offset_, bezel_lock_bar_length+2*offset_], center=true);
        rotate([0, 0, 90])
            square([bezel_lock_thickness+2*offset_, bezel_lock_bar_length+2*offset_], center=true);
    }
}

module center_bar(num_rows, right=false, left=false)
{
    for (i=[0:1:num_rows])
    {
        translate([i*led_spacing, 0, 0])
            center_bar_segment(right, left);
    }
}

module center_bar_segment(right=false, left=false)
{
    // todo try offsetting it so the holes are at the top, then the bezel slot, then the bottom
    // that way it will be more modular. And use similar bezel things for panel
    center_bar_lock_distance = 15;
    translate([led_spacing, center_bar_width/2-center_bar_lock_distance/2, center_bar_height/2])
        rotate([0, 90, 0])
            bezel_locking_tab();
    translate([led_spacing, center_bar_width/2+center_bar_lock_distance/2, center_bar_height/2])
        rotate([0, 90, 0])
            bezel_locking_tab();
    difference()
    {
        cube([led_spacing, center_bar_width, center_bar_height]);
        // led
        translate([led_spacing/2, center_bar_width/4, 0])
            cylinder(h=center_bar_height, d=pixel_hole_diameter);
        // button
        translate([led_spacing/2, 3*center_bar_width/4, 0])
            cylinder(h=center_bar_height, d=button_diameter);
        translate([0, center_bar_width/2-center_bar_lock_distance/2, center_bar_height/2])
            rotate([0, 90, 0])
                #bezel_locking_tab(negative=true);
        translate([0, center_bar_width/2+center_bar_lock_distance/2, center_bar_height/2])
            rotate([0, 90, 0])
                #bezel_locking_tab(negative=true);
        // locking tabs for bezels
        if (right)
        {
            translate([0, center_bar_width, bezel_lock_bar_length/2+bezel_lock_offset])
                rotate([90, 0, 0])
                    #bezel_locking_tab();
            translate([led_spacing, center_bar_width, bezel_lock_bar_length/2+bezel_lock_offset])
                rotate([90, 0, 0])
                    #bezel_locking_tab();
        }
        if (left)
        {
            translate([0, 0, bezel_lock_bar_length/2+bezel_lock_offset])
                rotate([-90, 0, 0])
                    #bezel_locking_tab(negative=true);
            translate([led_spacing, 0, bezel_lock_bar_length/2+bezel_lock_offset])
                rotate([-90, 0, 0])
                    #bezel_locking_tab(negative=true);
        }
    }
}

module mirror_copy(vector)
{
    children();
    mirror(vector)
        children();
}

// center_bar_segment(right=true, left=true);
center_bar(6, right=true, left=true);
//bezel_locking_tab();
//bezel();
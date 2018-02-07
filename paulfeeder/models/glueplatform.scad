shaftRadius = 2.5;
cutoutWidth = 3.5;
perfboardThickness = 1.38;
perfboardShort = 40;

braceWidth = 9.5;
braceDepth = 10;
braceHeight = 7;

wrapperInnerHeight = 3;
wrapperThickness = 1;

totalDepth = 30;

module makeShaftHolder() {
    translate([0,braceDepth,braceHeight/2])
    rotate([90,0,0])
    linear_extrude(height=braceDepth)
    difference() {
        square([braceWidth, braceHeight], center=true);
        intersection() {
            circle(r=shaftRadius,center=true,$fn=32);
            square([100,cutoutWidth],center=true);
        }
    };   
}

module makeWrapperSection(depthParam) {
    rotate([90,0,0])
    translate([0,-(wrapperInnerHeight+wrapperThickness),-depthParam/2])
    difference() {
        linear_extrude(height=depthParam, center=true)
        translate([0,(wrapperInnerHeight + 2*wrapperThickness)/2,0])
        difference() {
            square([perfboardShort,
                    wrapperInnerHeight],
                    center=true);
        };
        
    }
}

translate([0, 0, -1.5])
makeShaftHolder();

makeWrapperSection(braceDepth);
translate([0,braceDepth,0])
makeWrapperSection(30);
stepperDepth = 19 + 2; // has a couple mm extra
holderBacking = 5;
screwPanelHeight = 1;

stepperRadiusFudge = 0.3; // millimeter increase

// create a rounded horizontal bar sitting on z=0 and centered in x,y
module roundedHorizontalBar(width, height, depth) {
    radius = height/2;
    
    linear_extrude(height=depth)
    union() {
        square([width - 2*radius, height], center=true);
        
        for (direction = [-1, 1]) {
            position = direction * (width/2 - radius);
            
            translate([position, 0])
            circle(r=radius, center=true);
        }
    }
}

module trapezoid(bottomWidth, topWidth, height) {
    polygon(points=[
        [-bottomWidth/2, 0],
        [-topWidth/2, height],
        [topWidth/2, height],
        [bottomWidth/2, 0]
    ]);
}

// create a simplified stepper on its back centered in x, y.
module stepper() {
    radius = 14;
    translate([0,0,stepperDepth/2])
    cylinder(r=radius + stepperRadiusFudge, h=stepperDepth, center=true, $fn=36);
    
    rectHeight = 2.75;
    topRectWidth = 14.6;
    translate([-topRectWidth/2, 0, 0])
    cube([topRectWidth, radius + rectHeight, stepperDepth]);
    
    bottomRectWidth = 17.3;
    translate([-bottomRectWidth/2, 0, 0])
    cube([bottomRectWidth, radius, stepperDepth]);
    
    cordWidth = 7;
    cordHeight = 6;
    translate([-cordWidth/2, 0, stepperDepth - cordHeight])
    cube([cordWidth, radius + 50, cordHeight]);
    
    screwPanelSize = 7;
    translate([0, 0, stepperDepth - screwPanelHeight])
    roundedHorizontalBar(2*radius + 2*screwPanelSize, screwPanelSize, screwPanelHeight);
}

module stepperPegs() {
    stepperRadius = 14;
    pegHeight = 2;
    pegOffset = 3.5;
    pegRadius = 2 - 0.3; // fudge factor
    
    for (direction = [-1, 1]) {
        position = direction * (stepperRadius + pegOffset);
        
        translate([position, 0, stepperDepth + holderBacking + pegHeight/2 - screwPanelHeight])
        cylinder(r=pegRadius, h=pegHeight, center=true, $fn=16);
    }
}

module horizontalBar(depth) {
    width = 200;
    height = 15;
    holeOffsetX = 10;
    holeOffsetY = 6;
    
    difference() {
        translate([0, height/2, 0])
        roundedHorizontalBar(width, height, depth);
        
        translate([-width/2 + holeOffsetX, holeOffsetY, 0])
        mountHole();
        
        translate([width/2 - holeOffsetX, holeOffsetY, 0])
        mountHole();
    }
}

module verticalBar(depth) {
    width = 15;
    height = 80;
    holeOffsetY = 10;
    
    radius = width / 2;
    
    difference() {
        union() {
            translate([-width/2, -(height - radius), 0])
            cube([width, height - radius, depth]);
            
            translate([0, - (height - radius), depth/2])
            cylinder(r=radius, h=depth, center=true);
        }
            
        translate([0, -height + holeOffsetY, 0])
        mountHole();
    }
}

module mountBars() {
    mountDepth = 5;
    horizontalBar(mountDepth);
    verticalBar(mountDepth);
}


// create a mount hole centered at the origin
module mountHole() {
    headRadius = 3;
    shaftRadius = 1.5;
    height = 9;
    
    squareHeight = height - headRadius- shaftRadius;
    
    linear_extrude(height=50)
    union() {
        circle(r=headRadius, center=true, $fn=16);
        
        translate([-shaftRadius,0, 0])
        square([2*shaftRadius, squareHeight]);
        
        translate([0, squareHeight, 0])
        circle(r=shaftRadius, center=true, $fn=16);
    }
}

// the wall mounted chassis that holds the stepper.
// has holes for mounting screws.
// and a high surface area for velcro.
module chassis() {
    holderSize = 45;
    
    translate([0, 0, (stepperDepth + holderBacking)/2])
    cylinder(r=holderSize/2, h=stepperDepth+holderBacking, center=true, $fn=64);
    
    yOffset = 20;
    translate([0, yOffset, 0])
    mountBars();
    
    linear_extrude(5)
    trapezoid(holderSize, 50, yOffset);
}

module mainAssembly() {
    difference() {
        chassis();
        
        translate([0,0,holderBacking])
        stepper();
    }
    
    stepperPegs();
}

mainAssembly();
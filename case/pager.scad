// PagerSpass Pager - Gehaeuse
// part = "front" | "back" | "buttons" | "clip" | "all"
part = "all";

$fn = 48;

body_w = 76;
body_h = 64;
depth = 26;
split = 12;
wall = 2;
r_edge = 3;

roll_r = 13;
roll_x = body_w;

disp_pcb = [60.4, 35.4];
disp_pos = [6, 16];
disp_win = [44, 34];
disp_win_off = [3, 0];

top_btns = [[16, 7], [38, 14], [57, 14]];
btn_z = 9;
btn_depth = 5;
side_btn_y = 40;
side_btn_d = 8;

led_pos = [76, 56];
led_d = 3.1;

spk_pos = [76, 20];
spk_d = 20.4;

usb_esp_x = 19;
usb_tp_x = 42.5;
usb_z = 20.6;

screws = [[7, 7], [7, 57], [67, 7], [67, 57]];
clip_holes = [[36, 22], [36, 44]];

module rbox(size, r) {
  hull() for (x = [r, size[0] - r], y = [r, size[1] - r], z = [r, size[2] - r])
    translate([x, y, z]) sphere(r);
}

module roller(r, extra = 0) {
  translate([roll_x, -extra, split + 1]) rotate([-90, 0, 0])
    hull() {
      translate([0, 0, 1.5]) cylinder(r = r, h = body_h + 2 * extra - 3);
      cylinder(r = r - 1.5, h = body_h + 2 * extra);
    }
}

module outer() {
  rbox([body_w, body_h, depth], r_edge);
  roller(roll_r);
}

module inner() {
  translate([wall, wall, wall]) rbox([body_w - 2 * wall, body_h - 2 * wall, depth - 2 * wall], 1);
  intersection() {
    roller(roll_r - wall);
    translate([0, wall, 0]) cube([200, body_h - 2 * wall, 100]);
  }
}

module cutouts() {
  // Display-Fenster mit Fase
  wx = disp_pos[0] + disp_pcb[0] / 2 - disp_win[0] / 2 + disp_win_off[0];
  wy = disp_pos[1] + disp_pcb[1] / 2 - disp_win[1] / 2 + disp_win_off[1];
  translate([wx, wy, -1]) cube([disp_win[0], disp_win[1], 5]);
  hull() {
    translate([wx, wy, 1.2]) cube([disp_win[0], disp_win[1], 0.01]);
    translate([wx - 1.5, wy - 1.5, -0.01]) cube([disp_win[0] + 3, disp_win[1] + 3, 0.01]);
  }

  // Rahmen-Vertiefung wie beim Original
  translate([disp_pos[0] - 2, disp_pos[1] - 3, -0.01]) linear_extrude(0.6)
    offset(r = 3) offset(delta = -3) square([disp_pcb[0] + 4, disp_pcb[1] + 6]);

  translate([36, 6.5, -0.01]) linear_extrude(0.6)
    text("PagerSpass", size = 4.2, font = "Liberation Sans:style=Bold", halign = "center");
  translate([66, 6.5, -0.01]) linear_extrude(0.6)
    text("PS-1", size = 4.2, font = "Liberation Sans", halign = "center");

  for (b = top_btns)
    translate([b[0] - (b[1] + 0.6) / 2, body_h - wall - 1, btn_z - (btn_depth + 0.6) / 2])
      cube([b[1] + 0.6, wall + 2, btn_depth + 0.6]);

  translate([roll_x + roll_r - wall - 1, side_btn_y, split + 1]) rotate([0, 90, 0])
    cylinder(d = side_btn_d + 0.4, h = wall + 3);

  translate([led_pos[0], led_pos[1], -1]) cylinder(d = led_d, h = 6);

  for (a = [0 : 60 : 359]) for (r = [0, 3.5, 7])
    if (r > 0 || a == 0)
      translate([spk_pos[0] + r * cos(a + r * 10), spk_pos[1] + r * sin(a + r * 10), depth - 6])
        cylinder(d = 1.8, h = 8, $fn = 12);

  for (x = [usb_esp_x, usb_tp_x])
    translate([x - 5, -1, usb_z - 2.25]) rbox([10, wall + 2, 4.5], 1);

  translate([-1, 29, 17]) cube([wall + 2, 7, 3]);

  for (s = screws) {
    translate([s[0], s[1], 16]) cylinder(d = 4.6, h = 20);
    translate([s[0], s[1], 5]) cylinder(d = 2.4, h = 20);
  }

  for (c = clip_holes) {
    translate([c[0], c[1], depth - 7]) cylinder(d = 1.8, h = 8);
  }
}

module inner_parts() {
  for (s = screws) translate([s[0], s[1], 1]) cylinder(d = 7, h = depth - 2);

  // Displayrahmen
  translate([0, 0, wall - 0.01]) difference() {
    translate(disp_pos - [1.2, 1.2]) cube([disp_pcb[0] + 2.4, disp_pcb[1] + 2.4, 3]);
    translate(disp_pos) cube([disp_pcb[0], disp_pcb[1], 4]);
    translate(disp_pos + [-2, 8]) cube([4, disp_pcb[1] - 16, 4]);
  }

  // Halter fuer Tastenplatine
  for (x = [5, 67]) translate([x, body_h - 12.5, wall - 0.01]) difference() {
    cube([3, 4, split - wall + 0.01]);
    translate([-1, 1.2, 4]) cube([5, 1.8, 20]);
  }

  translate([spk_pos[0], spk_pos[1], depth - wall - 2.5]) difference() {
    cylinder(d = spk_d + 2.4, h = 2.51);
    translate([0, 0, -1]) cylinder(d = spk_d, h = 5);
  }

  for (c = clip_holes) translate([c[0], c[1], depth - wall - 4]) cylinder(d = 5, h = 4.01);

  // Platinenhalter hinten (ESP32-C3 + TP4056)
  for (b = [[usb_esp_x, 18.4, 23], [usb_tp_x, 17.4, 28.5]])
    for (sx = [-1, 1])
      translate([b[0] + sx * (b[1] / 2 + 0.6) - 0.6, wall + 3, depth - wall - 2.5])
        cube([1.2, b[2] - 6, 2.51]);
}

module shell() {
  difference() {
    union() {
      difference() {
        outer();
        inner();
      }
      intersection() {
        inner_parts();
        outer();
      }
    }
    cutouts();
  }
}

module front() {
  intersection() {
    shell();
    translate([-10, -10, -1]) cube([150, 100, split + 1]);
  }
  // Lippe zum Zentrieren
  translate([0, 0, split - 0.01]) intersection() {
    difference() {
      translate([wall, wall, 0]) cube([body_w - 2 * wall, body_h - 2 * wall, 1.5]);
      translate([wall + 1.2, wall + 1.2, -1]) cube([body_w - 2 * wall - 2.4, body_h - 2 * wall - 2.4, 4]);
      translate([body_w - 10, 0, -1]) cube([20, body_h, 4]);
    }
    translate([-1, -1, 0]) cube([body_w - 11, body_h + 2, 2]);
  }
}

module back() {
  difference() {
    intersection() {
      shell();
      translate([-10, -10, split]) cube([150, 100, 50]);
    }
    translate([wall - 0.25, wall - 0.25, split - 1]) difference() {
      cube([body_w - 2 * wall + 0.5, body_h - 2 * wall + 0.5, 2.8]);
      translate([1.7, 1.7, -1]) cube([body_w - 2 * wall - 2.9, body_h - 2 * wall - 2.9, 5]);
      translate([body_w - 10.5, -1, -1]) cube([20, body_h, 6]);
    }
  }
}

module btn_cap(w) {
  translate([-w / 2, -btn_depth / 2, 0]) rbox([w, btn_depth, 3.5], 1);
  translate([-(w + 2) / 2, -(btn_depth + 2) / 2, 2.5]) cube([w + 2, btn_depth + 2, 1]);
  translate([0, 0, 3.4]) cylinder(d = 3, h = 1.6);
}

module side_cap() {
  cylinder(d = side_btn_d, h = 3.5);
  translate([0, 0, 2.5]) cylinder(d = side_btn_d + 3, h = 1);
  translate([0, 0, 3.4]) cylinder(d = 3, h = 1.6);
}

module buttons() {
  translate([6, 0, 0]) btn_cap(7);
  translate([24, 0, 0]) btn_cap(14);
  translate([44, 0, 0]) btn_cap(14);
  translate([62, 0, 0]) side_cap();
}

module clip_profile() {
  square([3, 50]);
  translate([6, 45]) difference() {
    circle(r = 6);
    circle(r = 3);
    translate([-7, -7]) square([14, 7]);
  }
  polygon([[9, 45], [12, 45], [8, 7], [5.5, 6]]);
  polygon([[5.5, 6], [8, 7], [10.5, 1.5], [9, 0.5]]);
}

module clip_part() {
  w = 26;
  hole_dy = clip_holes[1][1] - clip_holes[0][1];
  difference() {
    linear_extrude(w) clip_profile();
    for (y = [15, 15 + hole_dy]) translate([-1, y, w / 2]) rotate([0, 90, 0]) {
      cylinder(d = 2.4, h = 5);
      translate([0, 0, 2.6]) cylinder(d1 = 2.4, d2 = 5, h = 1.41);
      translate([0, 0, 5]) cylinder(d = 5.5, h = 15);
    }
  }
}

// Modell ist von hinten gesehen aufgebaut, darum am Ende spiegeln
if (part == "front") mirror([1, 0, 0]) front();
else if (part == "back") mirror([1, 0, 0]) translate([0, 0, depth]) mirror([0, 0, 1]) back();
else if (part == "buttons") buttons();
else if (part == "clip") clip_part();
else mirror([1, 0, 0]) {
  color("#333") front();
  color("#444") translate([0, 0, 15]) back();
}

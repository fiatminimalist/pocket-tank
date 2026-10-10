/* Blackwater (2026-10-10): the wreck slot is a sunken WWII FIGHTER. A single-engine,
   radial-cowled fighter lying on the sand nose to the right and a little nose-down: the near
   wing toward the viewer, resting on the sand, with a worn roundel; the far wing behind,
   above the fuselage; the fin and the near tailplane at the left; the cockpit's canopy frame
   slid open over the cockpit (a HOLE the fish pass behind), a torn gap in the rear fuselage
   (the second hole); a bent propeller blade over the cowl and one dug into the sand; panel
   lines, rivets, rust streaks, encrusting growth low down; a small anchor on the sand off
   the nose. Dark olive-grey paint lit from the upper left. Same layers as the other hulls:
   -1 = all of it, 0 = only the dark inside (baked, never in the front mask), 1 = the solid
   pieces (the mask). Local coordinates: x from the centre, h up from WK_FY. */
static inline float wkb_axis(int x) { return 22 - (x + 58) * 0.07f; }           /* the fuselage's centre line, tail to nose */
static inline float wkb_r(int x) {                                               /* its half-height: a thin tail, a fat cowl, the spinner */
    if (x < -10) return 4 + 6 * (x + 58) / 48.0f;
    if (x < 30) return 10;
    if (x < 46) { float u = (x - 30) / 16.0f; return 10 - u * u * 3; }
    return 7 * (50 - x) / 4.0f;
}
static inline bool wkb_fuselage(int x, int h) {
    if (x < -58 || x >= 50) return false;
    float d = h - wkb_axis(x), r = wkb_r(x);
    return d > -r && d < r;
}
/* the cockpit, cut into the top of the fuselage, and the torn gap in the rear fuselage */
static inline bool wkb_cockpit(int x, int h) { float top = wkb_axis(x) + wkb_r(x); return x >= 6 && x < 22 && h >= top - 7 && h <= top; }
static inline bool wkb_gap(int x, int h) {
    float a = wkb_axis(x), r = wkb_r(x);
    int jag = (int)(chash(x, 9) % 4u), jag2 = (int)(chash(h, 11) % 3u);        /* ragged: the edge wanders per column and row */
    float dx = (x + 29 + (h > a ? jag2 : -jag2)) / 8.0f, dy = (h - a + 1) / (r * 0.6f + jag * 0.7f);
    return dx * dx + dy * dy < 1;
}
/* the near wing: from the root under the fuselage down and to the left onto the sand */
static inline bool wkb_near_wing(int x, int h) {
    float a0 = wkb_axis(8);
    if (h < 0 || h >= a0 + 1) return false;
    float u = (a0 - h) / a0;                                                       /* 0 at the root, 1 on the sand */
    float x0 = -8 - 14 * u, x1 = 26 - 24 * u;
    return x >= x0 && x <= x1;
}
/* the far wing: behind the fuselage, up and to the left (the plane lies a little on its side) */
static inline bool wkb_far_wing(int x, int h) {
    float a0 = wkb_axis(8) + 9;
    if (h < a0 || h >= a0 + 22) return false;
    float u = (h - a0) / 22.0f;
    float x0 = -4 - 24 * u, x1 = 22 - 30 * u;
    return x >= x0 && x <= x1;
}
static void draw_wreck_blackwater(ctx_t *c, int cx, bool final, int layer) {
    for (int h = -2; h <= WK_H; h++) {
        int y = WK_FY - h;
        if (y < c->oy || y >= c->oy + c->h) continue;
        for (int x = -WK_W / 2; x < WK_W / 2; x++) {
            int X = cx + x;
            if (X < c->ox || X >= c->ox + c->w) continue;
            unsigned noise = ((unsigned)(x + 100) * 1237u + (unsigned)(h + 40) * 719u) ^ ((unsigned)(x + 130) * (unsigned)(h + 17));
            float axis = wkb_axis(x), r = wkb_r(x);
            bool fus = wkb_fuselage(x, h);
            if (layer == 0) { if (fus) wk_put(c, X, y, mix(water_rgb(y), 0x070c10, 0.8f), final); continue; }
            bool hole = fus && (wkb_cockpit(x, h) || wkb_gap(x, h));
            if (hole) { if (layer < 0) wk_put(c, X, y, mix(water_rgb(y), 0x070c10, 0.8f), final); continue; }
            uint32_t rgb = 0; bool on = false;
            float d = h - axis;                                                    /* above (+) or below the centre line */
            /* the far wing and the fin, behind the fuselage */
            bool farw = !fus && wkb_far_wing(x, h);
            float fin_le = -44 - (h - (axis + r)) * 0.33f;                         /* the fin's leading edge sweeps back */
            bool fin = !fus && x >= -58 && x <= fin_le && h > wkb_axis(-50) + wkb_r(-50) - 1 && h < 46 && x >= -58 + (h > 42 ? (h - 42) * 2 : 0);
            if (farw) { on = true; rgb = (noise % 7 == 0) ? 0x3d4234 : 0x363b2f; if (((x + h) % 9) == 0) rgb = 0x272b23; }
            if (fin) { on = true; rgb = (noise % 7 == 0) ? 0x4a4f3f : 0x42473a;
                if (x <= -55) rgb = 0x3a3e33;                                       /* the rudder */
                if ((int)(h - 22) % 8 == 0) rgb = 0x2c2f27;
                if (x == -55 || h == 45) rgb = 0x5c6152; }
            /* the fuselage: olive-grey topsides, a pale belly, panel lines and rivets */
            if (fus) {
                on = true;
                float t = d / r;                                                    /* -1 belly .. 1 spine */
                rgb = t > 0.15f ? 0x4d5242 : t > -0.35f ? 0x565a4e : 0x7d8078;
                if (t > 0.72f) rgb = mix(rgb, 0xb9bcae, 0.32f);                     /* the sun on the spine */
                if (t < -0.75f) rgb = mix(rgb, 0x1c1e1a, 0.4f);
                if (noise % 9 == 0) rgb = mix(rgb, 0x6a6e60, 0.5f);
                bool panel = ((x + 58) % 12) == 0 || (x == 30) || (x == 46);
                if (panel) rgb = mix(rgb, 0x23261f, 0.6f);
                if (((x + 58) % 12) == 6 && ((h + x) & 3) == 0) rgb = mix(rgb, 0x8d9085, 0.5f);   /* rivets */
                if (x >= 30 && x < 46 && t > -0.4f && t < 0.4f && x >= 36) rgb = mix(rgb, 0x2a2d28, 0.4f);   /* the cowl's dark mouth */
                if (x >= 46) rgb = (x == 46) ? 0x2a2d28 : mix(0x8a8c84, 0x3a3d38, (float)(x - 46) / 4);      /* the spinner */
                if (x >= -5 && x < 4 && t > 0.3f) rgb = mix(rgb, 0x8a8c84, 0.3f);   /* the windscreen frame's root */
                if (x < -40 && t > 0.6f) rgb = mix(rgb, 0x2c2f27, 0.3f);
                /* rust streaks down the side, growth along the belly */
                if ((noise % 100) < 9 && t < 0.5f) rgb = mix(rgb, 0x6b3c1e, 0.55f);
                if (((x + 58) % 12 == 1) && t < 0.2f && (noise % 3)) rgb = mix(rgb, 0x5c3418, 0.4f);
                if (t < -0.5f && (noise % 100) < 30) rgb = (noise & 1) ? 0x6f7a60 : 0x55634c;
                /* torn edges round the gap, a lip round the cockpit */
                if (wkb_gap(x - 1, h) || wkb_gap(x + 1, h) || wkb_gap(x, h - 1) || wkb_gap(x, h + 1) || (wkb_gap(x - 2, h) && (noise & 1)) || (wkb_gap(x + 2, h) && (noise & 2))) rgb = (noise % 3) ? 0x1f211c : 0x9a9c92;
                if (wkb_cockpit(x - 1, h) || wkb_cockpit(x + 1, h) || wkb_cockpit(x, h - 1)) rgb = 0x8a8c84;
            }
            /* the canopy frame, slid open over the cockpit: an arch of struts, the water through it */
            { float top = wkb_axis(14) + wkb_r(14);
              bool strut = (x >= 6 && x < 24 && h > top && h < top + 7 && ((x == 6 || x == 7 || x == 14 || x == 23) || h >= top + 6))
                        || (x >= 24 && x < 32 && h > top && h < top + 7 - (x - 24) && (h == (int)(top + 7 - (x - 24)) - 1 || x == 24 || x == 25));
              if (strut) { on = true; rgb = ((x + h) & 1) ? 0x9a9c92 : 0x7a7d74; } }
            /* the near wing over the fuselage, resting on the sand: its roundel, panel lines, growth at the tip */
            if (wkb_near_wing(x, h)) {
                on = true;
                float a0 = wkb_axis(8), u = (a0 - h) / a0;
                rgb = (noise % 7 == 0) ? 0x6c7160 : 0x5f6455;
                if (u < 0.12f) rgb = mix(rgb, 0x8d9085, 0.5f);
                if (u < 0.05f || x > 26 - 24 * u - 1.2f) rgb = 0x272b23;             /* the root seam and the trailing edge */
                if (u > 0.3f && x < -8 - 14 * u + 2) rgb = mix(rgb, 0x9a9c92, 0.45f);   /* the lit leading edge */                      /* the lit root fillet */
                if (((int)(h * 1.0f) % 6) == 0 || ((x + 8 + (int)(14 * u)) % 11) == 0) rgb = mix(rgb, 0x272b23, 0.55f);
                { int rx = x - 2, rh = h - 9; int d2 = rx * rx + rh * rh;              /* the worn roundel */
                  if (d2 < 30) rgb = (noise % 4) ? 0xb3b2a2 : 0x8e8d80;
                  if (d2 < 11) rgb = (noise % 4) ? 0x7a2e26 : 0x5a2420;
                  if (d2 >= 30 && d2 < 46) rgb = (noise % 3) ? 0x2f3f6a : 0x3a4a78; }
                if (h < 4 && (noise % 100) < 45) rgb = (noise & 1) ? 0x6f7a60 : 0x55634c;
                if (h == 0 || (x < -15 && (noise % 5) == 0)) rgb = mix(rgb, 0x8a8c84, 0.3f);
            }
            /* the near tailplane: a foreshortened slab down from the tail */
            { float at = wkb_axis(-50);
              bool tp = x >= -57 && x <= -41 && h < at - 1 && h >= at - 7 + (x + 41) * 0.25f;
              if (tp) { on = true; rgb = (h >= at - 2.5f) ? 0x7d8078 : 0x454a3d; if (x == -41 || x == -57) rgb = 0x2c2f27; } }
            /* the propeller: the hub, a blade bent back over the cowl, one dug straight into the sand */
            { float hub = wkb_axis(48);
              int up = (int)(h - hub);
              bool blade_up = up >= 0 && up < 22 && x >= 48 + (up > 12 ? -(up - 12) : 0) - 1 && x <= 50 + (up > 12 ? -(up - 12) : 0);
              bool blade_dn = up < 0 && h >= 1 && x >= 48 && x <= 50;
              if (blade_up || blade_dn) { on = true; rgb = ((h + x) % 5 == 0) ? 0x6a6c64 : 0x33352f; if (up >= 19 && up < 22) rgb = 0xb9a63a; } }
            /* the anchor on the sand off the nose, its chain lying back toward the spinner */
            { int axl = x - 58;
              bool shank = axl >= -1 && axl <= 1 && h >= 2 && h < 20;
              bool stock = h >= 16 && h < 19 && axl >= -5 && axl <= 5;
              bool arms = h >= 1 && h < 6 && axl >= -5 && axl <= 5 && !(h >= 4 && axl >= -2 && axl <= 2);
              bool fluke = (axl <= -4 || axl >= 4) && axl >= -5 && axl <= 5 && h >= 5 && h < 10;
              bool ring = h >= 20 && h < 25 && axl >= -2 && axl <= 2 && !(h >= 21 && h < 24 && axl >= -1 && axl <= 1);
              if (shank || stock || arms || fluke || ring) { on = true; rgb = (noise % 5 == 0) ? 0x6b4a32 : (axl < 0 ? 0x5a5d5f : 0x3a3d40); }
              if (h >= 2 && h <= 6 && x >= 51 && x <= 56 && h == 2 + (56 - x) * 4 / 5 && (x & 1)) { on = true; rgb = 0x5a5d5f; } }
            if (!on) continue;
            if (noise % 29 == 0) rgb = mix(rgb, 0xffffff, 0.10f);
            wk_put(c, X, y, rgb, final);
        }
    }
    draw_floor_mound(c, cx, WRECK_HALF_W - 8, 5, final);
}

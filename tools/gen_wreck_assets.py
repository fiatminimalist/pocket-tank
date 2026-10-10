#!/usr/bin/env python3
"""Bake the shipwreck's shop thumbnails (2026-10-10), one per theme, the jellyfish's way: the
Original's tilted boat, Quiet Lagoon's capsized mossy hull, Tidepool Club's little tug (the
three hulls of render.c draw_wreck); --check verifies the tracked outputs are what this script makes."""
from pathlib import Path
import sys,io
from gen_theme_assets import raster,array
root=Path(__file__).resolve().parent.parent
# wood, dark wood, hole, sand, metal, sail, water tint - per theme
PAL=[('original',  '#8a6440','#4a3524','#0b1a22','#2e3b2c','#6f7577','#d8cfb0','#0a3c46'),
     ('quiet-lagoon','#7a7d64','#3e4a3c','#0e1816','#6f7a66','#6d7872','#c9c7a8','#386a63'),
     ('tidepool-club','#b38a5c','#6a4b30','#1c1e2a','#c5ad84','#8a8578','#efe3c3','#b7b488')]
def svg(wood,dark,hole,sand,metal,sail,water):
    return f'''<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 32 32">
<ellipse cx="16" cy="29" rx="14" ry="2.2" fill="{sand}"/>
<path d="M4 17 Q6 27 16 27 Q26 27 28 15 L26 14 Q24 25 16 25 Q8 25 6 16 Z" fill="{dark}"/>
<path d="M5 16 Q7 25 16 25 Q25 25 27 14 L4 14 Z" fill="{wood}"/>
<path d="M4 14 L27 14 L28 15 L4.5 15.5 Z" fill="{dark}"/>
<path d="M5 18 L26 18 M6.5 21.5 L24 21.5" stroke="{dark}" stroke-width=".7"/>
<ellipse cx="11" cy="19.5" rx="2.6" ry="2.2" fill="{hole}"/><ellipse cx="20" cy="19" rx="2.1" ry="1.9" fill="{hole}"/>
<rect x="15" y="3" width="1.6" height="11.5" fill="{dark}"/><rect x="11" y="6" width="9" height="1" fill="{dark}"/>
<path d="M12 7 L15 7 L15 12 Z" fill="{sail}"/>
<path d="M26 13 L27.5 7" stroke="{metal}" stroke-width=".8" stroke-dasharray="1 .8"/>
<path d="M28.5 4.5 v6 M26.5 7.5 h4 M26 10.5 q2.5 2.5 5 0" stroke="{metal}" stroke-width="1.1" fill="none" stroke-linecap="round"/>
<circle cx="28.5" cy="3.8" r="1" fill="none" stroke="{metal}" stroke-width=".8"/>
<circle cx="7" cy="9" r=".8" fill="{water}" opacity=".7"/><circle cx="9" cy="5.5" r="1.1" fill="{water}" opacity=".6"/>
</svg>'''
def svg_lagoon(wood,dark,hole,sand,metal,sail,water):
    moss='#4f7a4a'
    return f'''<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 32 32">
<ellipse cx="16" cy="29" rx="14" ry="2.2" fill="{sand}"/>
<path d="M3 28 Q3 12 16 11 Q29 12 29 28 Z" fill="{wood}"/>
<path d="M5 28 Q5 15 16 14 Q27 15 27 28" fill="none" stroke="{dark}" stroke-width=".8"/>
<path d="M8 28 Q8 19 16 18 Q24 19 24 28" fill="none" stroke="{dark}" stroke-width=".8"/>
<path d="M3.5 28 Q3 11 16 10.4 Q29 11 28.5 28" fill="none" stroke="{dark}" stroke-width="1.4"/>
<ellipse cx="11" cy="22" rx="3" ry="2.4" fill="{hole}"/><ellipse cx="20" cy="21" rx="2.6" ry="2.1" fill="{hole}"/>
<path d="M4 27 q3 -2 6 0 M22 27 q3 -2 5 0 M13 11.5 q3 -1.5 6 0" stroke="{moss}" stroke-width="1.6" fill="none" stroke-linecap="round"/>
<path d="M2.5 28 L7 15" stroke="{metal}" stroke-width="1.1" stroke-linecap="round"/><path d="M6 16 l2.2 -5 l2.2 1 l-1.4 5.2 Z" fill="{metal}"/>
<circle cx="29" cy="26.5" r="2.4" fill="{metal}"/><circle cx="29" cy="26" r=".7" fill="{hole}"/>
<path d="M27 25.5 Q25 23.5 23.5 24" stroke="{sail}" stroke-width=".7" fill="none"/>
<circle cx="8" cy="7" r=".8" fill="{water}" opacity=".7"/><circle cx="24" cy="5.5" r="1.1" fill="{water}" opacity=".6"/>
</svg>'''
def svg_club(wood,dark,hole,sand,metal,sail,water):
    red,teal,cream,orange,cyan='#d8402c','#2e8f85','#efe3c3','#e8a845','#33c7e0'
    return f'''<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 32 32">
<ellipse cx="16" cy="29" rx="14" ry="2.2" fill="{sand}"/>
<path d="M3 17 L29 17 L28 24 Q27 28 23 28 L9 28 Q5 28 4 24 Z" fill="{cream}"/>
<path d="M3.6 21 L28.4 21 L28 24 Q27 28 23 28 L9 28 Q5 28 4 24 Z" fill="{teal}"/>
<rect x="3.4" y="19.5" width="25.2" height="1.8" fill="{red}"/><rect x="3" y="16.4" width="26" height="1.4" fill="{dark}"/>
<circle cx="11" cy="22.5" r="2.4" fill="{hole}" stroke="{metal}" stroke-width=".8"/><circle cx="20" cy="22.5" r="2.4" fill="{hole}" stroke="{metal}" stroke-width=".8"/>
<rect x="11" y="9.5" width="9" height="7" rx="1" fill="{cream}"/><rect x="10.3" y="8.5" width="10.4" height="1.6" rx=".6" fill="{red}"/>
<rect x="12.5" y="11.5" width="2.4" height="2.6" fill="#ffd889"/><rect x="16.3" y="11.5" width="2.4" height="2.6" fill="#ffd889"/>
<rect x="21.5" y="7.5" width="3" height="9" fill="{orange}"/><rect x="21.5" y="7.5" width="3" height="1.6" fill="#2b2b2b"/><rect x="21.5" y="11" width="3" height="1.2" fill="#fff6d9"/>
<rect x="6.2" y="4" width="1" height="12.5" fill="{wood}"/><path d="M7.2 4.5 L12 6 L7.2 7.5 Z" fill="{cyan}"/>
<circle cx="26" cy="23.5" r="1.9" fill="none" stroke="#fff6d9" stroke-width="1.2"/><path d="M24.6 22.2 l2.8 2.6 M27.4 22.2 l-2.8 2.6" stroke="#e8702e" stroke-width=".8"/>
<path d="M29.5 18 v6 M27.8 21 h3.4 M27.3 24 q2.2 2.2 4.4 0" stroke="{metal}" stroke-width="1" fill="none" stroke-linecap="round"/>
<circle cx="29.5" cy="17.2" r=".9" fill="none" stroke="{metal}" stroke-width=".7"/>
<circle cx="4" cy="6" r=".8" fill="{water}" opacity=".7"/><circle cx="27" cy="4" r="1.1" fill="{water}" opacity=".6"/>
</svg>'''
outputs={};code=['/* Generated by tools/gen_wreck_assets.py (2026-10-10). Do not edit. */\n#include "icons.h"\n']
for i,(name,*cols) in enumerate(PAL):
    base=root/'assets/themes'/name; s=(svg,svg_lagoon,svg_club)[i](*cols)
    outputs[base/'shop_wreck.svg']=(s+'\n').encode()
    im=raster(s.encode(),32,32);buf=io.BytesIO();im.save(buf,format='PNG');outputs[base/'shop_wreck.png']=buf.getvalue()
    pixels=list(im.getdata());rgb=[((r>>3)<<11)|((g>>2)<<5)|(b>>3) for r,g,b,a in pixels];alpha=[a for r,g,b,a in pixels]
    symbol='icon_shop_wreck' if not i else f'wreck_icon_{i}'
    code.extend([array(symbol+'_rgb','uint16_t',rgb),array(symbol+'_a','uint8_t',alpha),f'{"static " if i else ""}const icon_t {symbol}={{32,32,{symbol}_rgb,{symbol}_a}};\n'])
code.append('const icon_t *wreck_theme_icon(int theme){return theme==1?&wreck_icon_1:theme==2?&wreck_icon_2:&icon_shop_wreck;}\n')
outputs[root/'common/wreck_assets.c']='\n'.join(code).encode()
for p,data in outputs.items():
    if '--check' in sys.argv: assert p.read_bytes()==data,f'Stale asset: {p}'
    else: p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(data)
print('Verified wreck icons' if '--check' in sys.argv else 'Generated 3 wreck SVG/PNG icons and common/wreck_assets.c')

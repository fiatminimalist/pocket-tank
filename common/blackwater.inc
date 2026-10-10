/* Blackwater (2026-10-10): the realistic creatures. Placeholder: the lagoon's shaded art until the
 * blackwater renderer lands. */
static void draw_jellyfish(sk_t *source);   /* jellyfish.inc, included later */
static void draw_blackwater_species(sk_t *k,int species) {
    if(species==SP_JELLYFISH){ int save=theme_active(); theme_activate(THEME_QUIET_LAGOON); draw_jellyfish(k); theme_activate(save); return; }
    draw_lagoon_species(k,species);
}
static void draw_blackwater_fish(ctx_t *c,const fish_t *f,float clock,bool asleep) { draw_lagoon_fish(c,f,clock,asleep); }

# Species (2026-10-05)

Nine new creatures join the classic fish: **seahorse, octopus, pufferfish,
anglerfish, electric eel, hammerhead shark, squid, crab, lobster**. Each one breeds, comes
in several designs, rolls its own personality inside its species' range, and
moves the way the real animal does. The distilled model still chooses every
creature's goal; the species decides how that goal is carried out.

## Where they come from

- **The shop:** each species is a sand-dollar item that brings a **pair of
  juveniles** (`SD_ITEM_SP_*`, nine items, `progression_buy` → `tank_add_species_pair`).
  It needs two free places in the tank. A species can be bought again once
  none of its kind are left.
- **A birth:** an arrival is the species of its parents. With a small chance
  (`SP_MUTATE_P`, 4 %) a fry of the classic fish hatches as a random new
  species - the rare surprise.
- **Breeding is within a species.** `pick_parents` picks a courting pair of
  the same species, so a tank of two species has two families.

## The cap

`N_FISH_MAX` is 10 (it was 6), on every board (`POP_CAP` 10; the device
shipped 5 "until advisor latency is measured"). Every creature adds a turn
to the advisor's queue: at ~3.7 s a decision on the device, ten creatures
get a fresh decision every ~37 s, against ~22 s at six. The reflex layer
keeps everyone moving in between; the decisions are just older.

## Species table (`SPECIES[]`, tank.c)

| species | token | size | bold | social | curiosity | lazy | turn | moves as |
|---|---|---|---|---|---|---|---|---|
| fish | `fish` | roster | 0.10-0.90 | 0.10-0.90 | roster | roster | roster | `LOCO_FIN` |
| seahorse | `seahorse` | 0.80-1.00 | 0.08-0.35 | 0.50-0.85 | 4.0 | 0.70 | 1.6 | `LOCO_UPRIGHT` |
| octopus | `octopus` | 1.00-1.25 | 0.40-0.80 | 0.05-0.25 | 8.5 | 0.40 | 3.0 | `LOCO_JET` (crawls the floor) |
| pufferfish | `puffer` | 0.85-1.05 | 0.25-0.60 | 0.20-0.50 | 7.0 | 0.50 | 4.6 | `LOCO_HOVER` |
| anglerfish | `angler` | 0.95-1.20 | 0.50-0.80 | 0.05-0.20 | 2.5 | 0.90 | 1.4 | `LOCO_AMBUSH` |
| electric eel | `eel` | 1.40-1.75 | 0.60-0.90 | 0.10-0.30 | 4.5 | 0.60 | 1.8 | `LOCO_UNDULATE` |
| hammerhead | `shark` | 1.60-1.95 | 0.80-0.95 | 0.50-0.80 | 5.0 | 0.10 | 1.5 | `LOCO_CRUISE` |
| squid | `squid` | 0.90-1.10 | 0.30-0.60 | 0.70-0.95 | 6.0 | 0.30 | 3.5 | `LOCO_JET` (hovers in open water) |
| crab | `crab` | 0.80-1.00 | 0.40-0.80 | 0.20-0.50 | 6.5 | 0.50 | 3.5 | `LOCO_SIDEWALK` |
| lobster | `lobster` | 1.10-1.40 | 0.50-0.85 | 0.05-0.25 | 5.5 | 0.60 | 2.0 | `LOCO_WALK` |

Sizes are tank-scaled (a "pup" hammerhead, a dwarf seahorse): 1.0 is the
classic fish's ~42 px. A newborn's size, bold and social are rolled inside
its species' range (inherited: the parents' mean ± 0.15, then clamped to
the range widened by 0.1).

## Designs (`SPECIES[].var[SP_VARIANTS]`)

Four designs a species, each a body / fin / accent and a pattern the
renderer draws for it (`fish_t.variant` 0..3). An arrival takes one
parent's design (80 %) or a random one (20 %), and its colours come from
the parents as the classic fish's do (body from one, markings from the
other).

- **Seahorse:** 0 golden (bands), 1 crimson (speckles), 2 black (white spots), 3 lavender (spines)
- **Octopus:** 0 common red-brown (mottled), 1 blue-ringed (rings), 2 mimic (stripes), 3 violet (plain, spots)
- **Pufferfish:** 0 spotted, 1 dogface grey (dark mask), 2 saddled white (black saddles), 3 golden (plain)
- **Anglerfish:** 0 abyss black (cyan lure), 1 frogfish orange (warts), 2 mottled brown (blotches), 3 pink warty (yellow lure)
- **Electric eel:** 0 olive (orange belly), 1 charcoal (yellow belly), 2 bronze, 3 spotted green
- **Hammerhead:** 0 grey, 1 bronze, 2 slate blue, 3 pale scalloped
- **Squid:** 0 pink (chromatophore dots), 1 firefly blue (glowing dots), 2 bigfin white, 3 reef amber
- **Crab:** 0 red rock, 1 blue (orange-tipped claws), 2 Sally Lightfoot (orange, blue flecks), 3 green shore crab
- **Lobster:** 0 common (dark olive, orange antennae), 1 rare blue, 2 spiny (teal, gold spots), 3 calico (red, cream patches)

## How they move (the reflex layer, tank.c)

The model picks one of the 8 goals; `target_for_goal` and the locomotion
step turn it into the animal's own motion.

- **Seahorse (`LOCO_UPRIGHT`)** - swims upright, slowly (dorsal fin flutter,
  the slowest swimmer: speed ×0.35), climbs and sinks more than it travels.
  REST: wraps its tail around the nearest grass frond and sways with it.
  Feeding: drifts close, then a quick head snick. Startle: clings and
  freezes instead of bolting.
- **Octopus (`LOCO_JET`, crawler)** - EXPLORE / REST / INSPECT on the floor
  and the reef: crawls on its arms. DART_PLAY and a startle: jets
  mantle-first, arms trailing, in pulses, and a startle leaves an ink cloud.
  REST: a den at the reef cluster, castle or grass foot, and it **takes on
  the colour of what it sits on** (`fish_t.camo`).
- **Pufferfish (`LOCO_HOVER`)** - slow, boxy, fins sculling: it can stop,
  turn on the spot and back up (no committed U-turn needed). Startle:
  **inflates** into a spiny ball for a few seconds, then deflates.
- **Anglerfish (`LOCO_AMBUSH`)** - an ambush hunter: it barely moves, stays
  low, waits with its lure bobbing, and lunges short and fast at food that
  comes near. The lure **glows**, brightest at night. Frogfish-style it can
  "walk" the floor on its pectoral fins when it explores.
- **Electric eel (`LOCO_UNDULATE`)** - a long body that ripples from head to
  tail, swims backward as easily as forward, and lies along the floor to
  rest. It **breathes air**: every 1-2 minutes it rises to the surface for a
  gulp. Startle: a harmless spark.
- **Hammerhead (`LOCO_CRUISE`)** - never stops (it breathes by swimming): a
  floor of speed, wide smooth turns, the head sweeping side to side. REST
  is a slow patrol lap, not a stop.
- **Squid (`LOCO_JET`, hoverer)** - hovers in open water with its fins
  rippling, moves forward or backward, and jets in pulses to dart or flee
  (ink on a startle). Social: squid hold station near each other.

- **Crab (`LOCO_SIDEWALK`)** - a floor walker that moves **sideways**, legs
  stepping in a ripple; it climbs the reef cluster, castle and rocks, and
  picks at food on the floor with its claws (two-handed). It never swims up:
  a goal in open water becomes the nearest point on the floor or a rock below
  it. REST: tucked under the reef / castle edge. Startle: claws up, then a
  fast sideways scuttle away.
- **Lobster (`LOCO_WALK`)** - walks the floor head first on its legs, long
  antennae sweeping; REST in a den under rock. Startle: the **tail-flip** -
  a few fast backward strokes of its tail that shoot it backward (burst ×2.2),
  then it walks again. Like the crab it stays on the floor and rocks.

## The gentle tank

No creature harms another. The small ones (fish, seahorse, pufferfish,
squid, crab, lobster) keep their distance from the big ones (hammerhead, eel): a stronger
separation push inside `SP_AVOID_R`. Every creature eats pellets.

## Persistence

- `fish_save_t.pad` (2 bytes a fish, written 0 by every older build) holds
  **species** and **variant**: an older save reads every creature as the
  classic fish, design 0.
- Fish 7..10 live in a new save tail (`fish_ext`, from offset 1688). The
  core count `n_fish` stays ≤ 6 and the tail holds the true count, so an
  older build (an OTA rollback) still loads the tank: its first six, as fish.
- The species' live state (puff, ink, camo, the eel's air, the seahorse's
  frond) is not saved.

## The model (schema 5)

The v4 model has no word for a species: an unknown word becomes `<unk>`,
which it never trained on. So the advisor sends `species <token>` only to a
**schema-5** model (detected by ` species` in its tokenizer), and the v4
model keeps deciding as before, from each creature's species-shaped traits.
The retrain runbook is `docs/retrain-v5.md`.

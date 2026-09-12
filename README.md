# ReQoning Wave Arena

An homage to **Quake III Arena**, **Team Arena**, **Rocket Arena**, **OSP**, and **Threewave**.

Arena FPS built on [Quake3e](https://github.com/ec-/Quake3e) with Team Arena–derived gamecode.

RWA takes the modes people actually play — item Duel, RA3-style Arena, Clan Arena, FFA, Freeze Tag, instagib, and CTF — and runs them as one game on the classic map pool if you already own Quake III.

## Play style

This is not a campaign and not a hero shooter.

- Movement and aim decide the fight.
- **Gametype rules are fixed**, taken from the most popular Quake III and Quake Live setups. Same weapons, health, armor, and win conditions on every server. Admins do not get a rules kitchen.
- **Duel** is item control (the old tournament 1v1).
- **ReQoning Arena** is RA3-style 1v1 / 2v2 / 3v3 (starts 1v1, max 3). **Clan Arena** and **CTF** use the default max team size (**15**). Admin can change it.
- A **queueing system** on every gametype. If the floor is full, extras spec in line. AFK is skipped.
- Default server cap is **64 players**. Default max team size is **15**. Admin can change both.
- **Fullbright / brightskins** are supported, including in team modes.
- **CTF:** the map specifies the loadout — HMG, nailgun, and chaingun only if the mapper placed them.
- Warmup uses the **same guns as the gametype and map**. No separate warmup arsenal.

If you have Quake III `pak0`–`pak8` (and optional Team Arena data), **any Quake III or converted Quake Live map that uses stock shader paths will load** — workshop maps, LvL World pk3s, the usual duel/CA pool. This game does not launch stock `baseq3` or Team Arena. Those folders are asset libraries only.

## Gametypes

| Mode | How it plays |
|---|---|
| **Duel (`1v1`)**| Popular Q3 / QL duel. **1v1.** 10 min, 125/0, G+MG, items on. Queue; winner stays. |
| **Free For All (`ffa`)**| Popular Q3 / QL FFA. No teams; player cap is the server cap (default 64). Items on. 50 frags / time. Map decides extra guns (HMG / nail / chaingun). |
| **Capture The Flag (`ctf`)**| Popular Q3 / QL CTF. Default max team size **15** (admin can change). **The map specifies the loadout** (HMG / nail / chaingun only if placed). **Weapon respawn 1 second.** |
| **Clan Arena (`ca`)**| Default QL CA. Default max team size **15** (admin can change). Full stack, no items, first to 10. Dead sit until the next round. |
| **Insta FFA (`iffa`)**| Instagib rail + instagib gauntlet. Same player cap as FFA. No ammo items. **Quad, regen, invis, and battlesuit still spawn.** |
| **ReQoning Arena (`ra`)**| RA3-style stacked arena. **Starts 1v1, max 3v3.** No items, no self-dmg. Die → spec. Queue; winner(s) stay. |
| **Freeze Tag (`ft`)**| Popular QL / Q3 FT. **Minimum start 2v2.** First to 8 (pub) / 10 (classic). Thaw ~3s. |
| **Insta Freeze (`ift`)**| Insta FFA rules on teams. **Minimum start 2v2.** |
| **ReQoning Freeze (`rft`)** | Team freeze + insta rail + insta gaunt + **offhand grapple**. **Minimum start 2v2.** No ammo/armor/health. Powerups: invis, regen, quad, battlesuit. |

### Voting

A simplified callvote system for gametype and team size / player cap. Details belong in the docs.

## Queueing

Every gametype uses the queue. Duel is 1v1. ReQoning Arena maxes at 3v3. Freeze modes need 2v2 to start. Default **64** players and max team size **15** leave room for spec. Tags: `[n]` in line, `[S]` watching, `[AFK]` skipped. Winner stays in Duel and ReQoning Arena.

## Data / maps

RWA ships its own game folder (`baserwa`) and gamecode. It does **not** ship id Software pak files.

Layout — this is the only supported path:

```
~/RWA/
  baserwa/                  # this game (qvms + our pk3)
  idassets/
    baseq3/                 # copy YOUR Quake III pak0.pk3 – pak8.pk3 here
    missionpack/            # copy YOUR Team Arena pak0.pk3 here
  maps/                     # maps autodownloaded from servers
```

- Id assets go in `idassets/` only. Do not put them in `baserwa/` or a root `baseq3/`.
- Server maps download into `maps/`.
- `vm/` inside `idassets` is never loaded.
- **No other mods.** OSP, CPMA, baseq3, missionpack, and anything else as `fs_game` will not load.

Quake III and Team Arena are sold separately. You provide those paks; they are not in this repo.

## Single player

An intro map teaches movement and a few rounds of shooting. After that, single player walks through **tiers of bots and gametypes** so you get a feel for each type before you go online. Skip the intro if you already know how to strafe.

## Skill and balance

Skill is recorded and logged. A balance system is included when teams are filled.

## Status

Pre-alpha. Engine and gamecode compile first; gametypes, queue, and our own maps follow.

## License

Engine and gamecode: GNU GPL v2, same as Quake III source and Quake3e.

Our original assets (when added) will be listed here with their licenses. Community maps keep whatever license their authors set — we do not claim them.

## Credits

- id Software — Quake III Arena / Team Arena *source*
- Eugene **ec-** — [Quake3e](https://github.com/ec-/Quake3e)
- **Kr3m** — [missionpackplus](https://github.com/Kr3m/missionpackplus)
- Map authors who released pk3s for Q3 / QL

## Disclaimer

This project is not affiliated with, endorsed by, or sponsored by id Software, ZeniMax, Bethesda, Microsoft, or Quake Live.

Quake®, Quake III Arena, and Team Arena are trademarks of id Software LLC. Rocket Arena, OSP, and Threewave are the work of their original authors. RWA is an independent homage built from GPL source.

Quake® is a trademark of id Software LLC.

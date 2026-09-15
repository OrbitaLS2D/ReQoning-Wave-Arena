# Changelog

## 0.2.2 — 2026-09-14

- Game pak is `baserwa/rwapak0.pk3` only. `pak3a.pk3` and `rwapak1.pk3` are not built.

## 0.2.1 — 2026-09-14

- `/cv capturelimit` works (missing comma had glued it to warmup).
- `ovld`, `harv`, and `oneflag` are accepted gametype names.
- `1fctf` is not parsed as duel. 1FCTF needs a white flag (TA mpteam*/mpterra*/mpq3ctf*, or arena oneflag).
- Removed unused home-gametype helpers. Map-only votes keep the current mode.


## 0.2.0 — 2026-09-14

First tracked release. Engine and gamecode both report **RWA 0.2.0**.

### What it is
- Standalone arena shooter on Quake3e + Missionpack+ gamecode.
- Does not launch baseq3 or Team Arena as a mod.
- Needs your own `idassets/baseq3/pak0–8`. Team Arena pak is optional.
- Extra maps go in `~/RWA/maps/`.

### Build
- One `make` builds the engine, VMs, `rwapak0.pk3`, and installs to `~/RWA`.
- Binaries: `reqwa.x64` / `reqwa.ded.x64`.
- Folder layout: `baserwa/`, `idassets/baseq3/`, `idassets/missionpack/`, `maps/`.

### Net / hits
- Unlagged the original way (server rewind + client delag).
- Bounding-box draw is cheat-protected.

### Maps / gametypes
- FFA, duel, TDM, and single-player can run on any map (including CTF maps).
- CTF, 1FCTF, overload, and harvester need the map’s arena type.
- Names work as well as numbers: `ffa`, `duel`/`1v1`, `tdm`, `ctf`, `1fctf`, `ovld`, `harv`.

### Votes (`/cv` and `callvote`)
- `/cv` is the short form of `callvote`.
- Empty `/cv` lists commands.
- Empty `/cv map` lists maps in groups (FFA / CTF / 1FCTF / OVLD / HARV).
- Empty `/cv gametype` (also `gt`, `g_gametype`) lists modes.
- `warmup` and `g_warmup` both work.
- `/cv map q3dm6` or `/cv map q3ctf1 ctf` — refuses a bad combo in red.
- `texturegrab` and `test_bigbox` stay off the vote list. Admin `/map` can still load them.

### Console
- Colored cvar prints.
- `g_gametype` shows the number and the name.

### Menu
- Team Arena style UI, RWA main menu (no Mods button).
- Branding: OrbitaL Software, based on Quake III Arena / Team Arena.

# Ultimate Warcraft 3 (UWC3)

UWC3 is an AMX Mod X plugin for Counter-Strike 1.6 and Condition Zero, derived from Warcraft 3
Frozen Throne (War3FT). It is raceless: instead of picking a Warcraft 3 race you train skills from
all of them, with an optional enhancement system of attributes and resistances on top.

Created by K2 (Andrew Cowan, "K2mia") in 2004, with Jim Rhoades (website) and steve french (graphic
effects). Continued on wc3mods.net in 2005-2006 as the 1.6.x unofficial line by Geesu, with
optimizations by JGHG, then developed further by Marticus (with teame06) as 1.6.3-beta.

## Versions in this repository

Each release is one commit, authored by the person who made it and dated to its release, and tagged
(`v0.1.7c`, `v1.0.0-RC6j`, `v1.6.0-unofficial`, `v1.6.1-unofficial`, `v1.6.2-unofficial`,
`v1.6.3-beta-133`). The tip of `main` is 1.6.3-beta-133.

| Commit | Version | Notes |
|---|---|---|
| UWC3 0.1.7c | 0.1.7c, AMX Mod X 0.16 (K2, 2004-10-02) | Binary release only; no source was published |
| UWC3 1.0.0-RC6j | 1.0.0-RC6j, AMX Mod X 0.20 (K2, source released 2005-02-16) | K2's final version and his published source |
| UWC3 1.6.0 (unofficial) | 1.6.0-unofficial, AMX Mod X 1.60 (Geesu, 2005-09-22) | First AMX Mod X 1.60 build, with JGHG's optimizations |
| UWC3 1.6.1 (unofficial) | 1.6.1-unofficial (Geesu, 2005-11-29) | DBI fixes, fakemeta headshot immunity |
| UWC3 1.6.2 (unofficial) | 1.6.2-unofficial (Geesu, 2006-01-30) | The most widely run UWC3: STEAM_ID_PENDING fix, /dropskill |
| UWC3 1.6.3-beta-133 | 1.6.3-beta-133 (Marticus, 2006-07-29) | Last nightly snapshot of the 1.6.3 development branch, recovered from the Internet Archive; see `changelog.txt` |

[HISTORY.md](HISTORY.md) has the full timeline, including forks that did not survive.

## Marticus' Subversion history

In 2006 Marticus kept UWC3 in Subversion (svn.marticus.org/svn/uwc3ng). The server is long gone,
but the Internet Archive captured its ViewVC browser in early 2007, including tarballs of the whole
repository at most revisions. That history is rebuilt here, one commit per SVN revision with the
original author, date and log message (`SVN-Revision:` trailer):

| Ref | What |
|---|---|
| `svn/trunk`, tag `svn/1.0.0-RC6j` | K2's 1.0.0-RC6j as imported |
| `svn/unofficial`, tags `svn/1.6.0`, `svn/1.6.1`, `svn/1.6.2` | Geesu's 1.6.x releases (the source of 1.6.0-1.6.2 on `main`) |
| `svn/dumb` | The DumB clan's 1.6.1-RC1 fork (obe), with its changelogs |
| `svn/idlenexus`, tags `svn/1.61`, `svn/1.6.1+idlenexus`, `svn/1.6.2+idlenexus` | Marticus' development branch, leading to 1.6.3-beta |
| `svn/teame06` | teame06's branch: a deathmatch build (`dm_uwc3`) and structure changes |

102 of the 104 revisions are rebuilt exactly from archived tarballs; the other two differ only in
files outside these branches.

## Layout

| Path | What |
|---|---|
| `uwc3.sma`, `uwc3/*.inl` | Plugin source (1.6.0-1.6.2 named the includes `uwc3/uwc3_*.inc`) |
| `changelog.txt` | Marticus' 1.6.3-beta changelog |
| `config/UWC3/UWC3.cfg`, `config/UWC3/UWC3_sql.cfg` | Server config; install into `addons/amxmodx/configs/UWC3/` |
| `sound/`, `sprites/`, `models/` | Assets; install into the game directory (`cstrike/` or `czero/`) |
| `INSTALL` | K2's original install guide (written for AMX Mod X 0.20) |
| `vault_to_dbi.pl` | K2's script to convert vault XP files to SQL |
| `README` | K2's source terms |

## Building

1.6.0 through 1.6.3-beta-133 compile with AMX Mod X 1.10's compiler (0 errors, deprecation warnings only):

```
amxxpc uwc3.sma -ouwc3.amxx
```

It needs the fun, engine, cstrike, fakemeta and mysql modules (enable `mysql` in `modules.ini`).
UWC3 saves XP through AMX Mod X's legacy DBI natives. On AMX Mod X 1.10 the stock mysql module
crashes in `dbi_connect` once a login succeeds (`DatabaseInfo::charset` is left uninitialized), so
SQL saving needs a module built with that fix, or set `mp_sql 0` for vault saving.

## License

K2's terms (see `README`): follow AMX Mod X's terms, keep all existing credits intact in the source
and in game, and add credit for your own changes alongside them.

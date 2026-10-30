# UWC3 history

Reconstructed in October 2026 from Internet Archive captures of uwc3.com, wc3mods.net (site and
forum "Announcements and Releases"), uwc3mod.com and AlliedModders, plus the wc3mods.net forum
database.

## Timeline

| When | What |
|---|---|
| ~2003 | SpaceDude's original Warcraft 3 plugin for CS; PimpDaddy's Warcraft3 Frozen Throne (War3FT) derives from it. |
| 2004 | K2 (Andrew Cowan, "K2mia") creates Ultimate Warcraft3: a raceless War3FT derivative where players train skills from every race. uwc3.com is first archived 2004-09-18. |
| 2004-09 to 10-02 | AMXX 0.16 line, ending at v0.1.7c ("final amxx0.16 cleanup", 10-02-04). Vault saving only. |
| 2004-09-16 to 11-03 | AMXX 0.20 line: 1.0.0 RC1 (09-20-04), RC6e (10-07-04) ... RC6j. The release zip's uwc3.amxx is dated 10-27-04; the changelog's last entry is 11-03-04 (players.cgi). |
| 2004-12-25 | Last edits in K2's source tree (still labelled 1.0.0-RC6j). |
| 2005-02-16 | K2 publishes uwc3_src.zip with its terms: keep all credits, add your own alongside. |
| 2005 | JGHG ("Johnny got his gun") publishes optimizations for UWC3 (jghg.zip, actually a RAR). |
| 2005-09-22 | Geesu posts "[AMXX] Update - v1.6.0 - unofficial" on wc3mods.net: a rebuild for AMXX 1.60 using JGHG's optimizations (t=512). |
| 2005-10-10 | "obe" posts the DumB clan fork, "UWC3 1.6.1-RC1 - SQL only": max level 40, new XP saving, Psychostats output, skill rebalancing (t=605). |
| 2005-11-14 | Geesu mirrors the old uwc3.com forums at wc3mods.net/uwc3/oldforums (t=720). |
| 2005-11-29 | Geesu releases v1.6.1: DBI fixes (dbi_free_result errors), fakemeta instead of set_user_hitzones, fixed SQL CREATE TABLE statements. 2,497 downloads (t=816). |
| 2006-01-30 | Geesu releases v1.6.2-unofficial: STEAM_ID_PENDING fix and /dropskill (both from Marticus). **85,365 downloads**, almost certainly the most-run UWC3 (t=1048). |
| 2006-02-01 | Geesu: "Official uwc3 forums are now here!!! The official uwc3.com site now links to these forums, so expect official releases to come from this forum." UWC3 effectively moves to wc3mods (t=1053). |
| 2006-04-06 | Marticus freezes UWC3 until the forks are merged into one source tree (t=1387). |
| 2006-07 | Marticus returns from a hiatus and opens Subversion at svn.marticus.org/svn/uwc3ng/ with branches idlenexus (official development), trunk, dumb, teame06 and unofficial (Geesu's 1.6.x); posts the official development branch as "UWC3 1.6.3 Beta"; nightly builds versioned 1.6.3-beta-&lt;revision&gt; (t=1704, 1712, 1717, 1767). |
| 2007-04 | Marticus marks the SVN and nightly builds "extremely outdated due to lack of interest". uwc3mod.com, a separate community, launches a new site (2007-04-01) hosting UWC3 1.6.3, UWC3 1.6.4, DumB's 1.6.3 and a "UWC3 Base Pack". |
| 2007-08-09 | K2 restores uwc3.com's download page: "It looks like the project to continue UWC3 development over at wc3mods has died", relinking his final versions (0.1.7c, 1.0.0 RC6j, source, JGHG). |
| 2007-09-13 | UWC3 Steam group founded (per its group page). |
| 2008-07 | DA (Michel Vocks) releases "Ultimate Warcraft 3 Next Generation" (UWC3NG) on AlliedModders: a new raceless mod built on War3FT (r11) for AMXX 1.8, not K2's code. Later versions register `amx_uwc3ng` (2.35.6 is still running in 2026). Inactive after 2011. |
| 2008 to 2024 | wc3mods.net/uwc3/ mirrors the uwc3.com pages and K2's downloads, plus an "Unofficial Updates (AMXX 1.60)" link to forum #12. Still served in June 2024. |
| 2010 to 2013 | uwc3.com news is about K2's next project, WowMOD. |
| 2025-05 | wc3mods/uwc3 created on GitHub (empty). The wc3mods.net Rails site replaces the PHP site; its UWC3 docs page is a placeholder. |
| 2026-10-29 | UWC3 recovered from the Internet Archive: K2's releases and source from uwc3.com, and Marticus' whole Subversion history (Geesu's 1.6.0-1.6.2, the DumB and teame06 forks, 1.6.3-beta) from svn.marticus.org, published in this repository. The official wc3mods UWC3 server runs 1.6.2 again. |

## Version lines

- **K2 (official, 2004):** 0.1.7c (AMXX 0.16), 1.0.0 RC1 through RC6j (AMXX 0.20). Source = RC6j, Dec 2004.
- **wc3mods unofficial (2005-2006):** Geesu 1.6.0, 1.6.1, 1.6.2-unofficial (with Marticus fixes).
- **DumB clan fork:** 1.6.1-RC1 (2005), later a 1.6.3 on uwc3mod.com.
- **Marticus / wc3mods development (2006-2007):** 1.6.3 beta + nightlies in svn.marticus.org/svn/uwc3ng; 1.6.4 seen on uwc3mod.com (author not confirmed).
- **Separate mods with UWC3 in the name:** UWC3NG by DA (AlliedModders, 2008+, `amx_uwc3ng`), UWC3X forks such as WarStrike (`amx_uwc3x`).
- **Unknown:** a server reporting `amx_uwc3 1.0.0b` in 2026; no source for that version string was found.


## What was lost, and what came back

The 1.6.x zips attached to the wc3mods.net forum (1.6.0, 1.6.1, 1.6.2, DumB's 1.6.1-RC1, teame06's
version) lost their files in a later site move; only the posts and download counts survived
(1.6.2: 97,515 downloads; DumB 1.6.1-RC1: 93,214). The code first came back through Marticus'
Subversion repository, whose ViewVC browser the Internet Archive captured in early 2007 (the `svn/*`
branches and tags here). The original zips then turned up on an old server backup and are
downloadable from the forum again; every source file in them is byte-identical to the
corresponding SVN tag or branch.

Still lost: the uwc3mod.com builds (1.6.3, 1.6.4, DumB 1.6.3). Per the wc3mods.net forums in 2007
that site was unofficial and shipped them without source. If you have any of these files, please
open an issue.

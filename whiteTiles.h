
//{{BLOCK(whiteTiles)

//======================================================================
//
//	whiteTiles, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 121 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 7744 + 2048 = 10304
//
//	Time-stamp: 2014-04-24, 19:31:33
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_WHITETILES_H
#define GRIT_WHITETILES_H

#define whiteTilesTilesLen 7744
extern const unsigned short whiteTilesTiles[3872];

#define whiteTilesMapLen 2048
extern const unsigned short whiteTilesMap[1024];

#define whiteTilesPalLen 512
extern const unsigned short whiteTilesPal[256];

#endif // GRIT_WHITETILES_H

//}}BLOCK(whiteTiles)
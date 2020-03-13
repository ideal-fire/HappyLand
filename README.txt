How to compile v1.0 for PS Vita:
1. Revert to commit 380ec3912edd8b2f4e6e009034a1343459cebf41. An easy way is to just download the ZIP (https://github.com/MyLegGuy/HappyLand/archive/380ec3912edd8b2f4e6e009034a1343459cebf41.zip)
2. make clean
3. make
4. You need to put the "Stuff", "SpanishReplace", "sce_sys", and "OtherStuff" folders into the VPK. This can be done, for example, with 7za using the command "7z a HappyLand.vpk ./Stuff/ ./SpanishReplace/ ./sce_sys/ ./OtherStuff/"

The working code for v1.1 Vita version exists only between two commits, so it's gone without doing extra work.
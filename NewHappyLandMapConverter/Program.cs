using System;
using NLua;
using System.IO;
using System.Diagnostics;

namespace MoronLandMapConverter
{
	class MainClass
	{
		/// <summary>
		/// This is the lua state we'll do everything in
		/// </summary>
		static Lua currentLua;

        static byte totalTilesets;
        static byte[] tilesetFirstGid;

		public static void Main (string[] args)
		{

			// Changes file path to work with lua.
			string currentDirectoryForLua = Directory.GetCurrentDirectory().Replace("\\","/");
			// The map's width
			short mapWidth;
			// The map's height
			short mapHeight;
			// THe map's map stuff
			byte[][,] map;
			// The collision map
			byte [][,] collisionMap;

			byte [] [,] blockEventMap;

            // Create the new lua.
			currentLua = new Lua ();
			
			// Load map lua.
			currentLua.DoString("loadedMap = dofile('"+currentDirectoryForLua+"/map.lua');");

			Console.WriteLine ("Reading all Lua data...");

			// Get width and height from loaded map.
			mapWidth = Convert.ToInt16((currentLua.DoString ("return loadedMap.layers[1].width;") [0]));
			mapHeight = Convert.ToInt16((currentLua.DoString ("return loadedMap.layers[1].height;") [0]));

            totalTilesets = Convert.ToByte((currentLua.DoString("return #loadedMap.tilesets")[0]));
            tilesetFirstGid = new byte[totalTilesets + 1];

            // Set firstgid on each tileset.
            for (int i = 1; i != totalTilesets + 1; i++) {
                tilesetFirstGid[i] = Convert.ToByte((currentLua.DoString("return loadedMap.tilesets[" + i.ToString() + "].firstgid")[0]));
                Debug.Print(i.ToString() + " is " + tilesetFirstGid[i].ToString());
            }

			// -2 b/c solid layer and block event layer
			byte numLayers = Convert.ToByte(Convert.ToByte ((currentLua.DoString ("return #loadedMap.layers")[0]))-2);

			Console.WriteLine ("Layers:" + numLayers.ToString () + "\nWidth:" + mapWidth.ToString () + "\nHeight:" + mapHeight.ToString ()+"\nTotal Tilesets:"+totalTilesets.ToString());
			Console.WriteLine ("Reading layer data...");
			//map = new byte[numLayers] [mapWidth, mapHeight];
			map=new byte[numLayers][,];


			collisionMap = new byte [1][,];
			collisionMap [0] = new byte [mapWidth, mapHeight];
			blockEventMap = new byte [1] [,];
			blockEventMap [0] = new byte [mapWidth, mapHeight];
			for (int i = 0; i < map.Length; i++) {
				map[i] = new byte[mapWidth,mapHeight];
			}
			System.Diagnostics.Debug.Print (mapWidth.ToString ());
			System.Diagnostics.Debug.Print (mapHeight.ToString ());
			Debug.Print (numLayers.ToString ());
			for (int i=0;i!=numLayers;i++){
				System.Diagnostics.Debug.Print ("I layer:" + i.ToString ());
				for (int y = 0; y < mapHeight; y++) {
					for (int x = 0; x < mapWidth; x++) {
						//Debug.Print("x:"+x.ToString()+";y:"+y.ToString()+";i:"+i.ToString());
						map [i] [x, y] = Convert.ToByte (currentLua.DoString ("return loadedMap.layers[" + (i + 1).ToString () + "].data[" + (x + (y*mapWidth) + 1).ToString () + "];")[0]);
						//Console.WriteLine (map [i] [x, y].ToString ());

						//Console.WriteLine("X:"+x.ToString()+";Y:"+y.ToString()+";val:"+map[i][x,y].ToString());
						//Console.ReadKey ();
					}
					//Console.WriteLine ("New Y value.");
				}
				Console.WriteLine ("New layer.");
			}

			Console.WriteLine ("Going to assign collision layer and block event layer...");
			for (int y = 0; y < mapHeight; y++) {
				for (int x = 0; x < mapWidth; x++) {
					//Debug.Print("x:"+x.ToString()+";y:"+y.ToString()+";i:"+i.ToString());
					collisionMap [0][x, y] = fixNumber(Convert.ToByte (currentLua.DoString ("return loadedMap.layers["+(numLayers+2).ToString()+"].data[" + (x + (y * mapWidth) + 1).ToString () + "];") [0]));
					//Debug.Print (fixNumber (Convert.ToByte (currentLua.DoString ("return loadedMap.layers[" + (numLayers + 2).ToString () + "].data[" + (x + (y * mapWidth) + 1).ToString () + "];") [0])).ToString ());
					blockEventMap [0] [x, y] = fixNumber(Convert.ToByte (currentLua.DoString ("return loadedMap.layers[" + (numLayers + 1).ToString () + "].data[" + (x + (y * mapWidth) + 1).ToString () + "];") [0]));

					//Console.WriteLine (map [i] [x, y].ToString ());

					//Console.WriteLine("X:"+x.ToString()+";Y:"+y.ToString()+";val:"+map[i][x,y].ToString());
					//Console.ReadKey ();
				}
				//Console.WriteLine ("New Y value.");
			}

			Console.WriteLine ("Going to write to file...");

			FileStream fs = new FileStream (Directory.GetCurrentDirectory () + "/exported.MoronLand",FileMode.Create);
			BinaryWriter bw = new BinaryWriter(fs);

			
			bw.Write (mapWidth);
			bw.Write (mapHeight);
			for (int y=0;y<mapHeight;y++){
				for (int x=0;x<mapWidth;x++){
					// issolid
					bw.Write(collisionMap[0][x,y]);
					// Event
					bw.Write(blockEventMap[0][x,y]);
				}
			}
			bw.Write((byte)numLayers);
			for (int i=0;i<numLayers;i++){
				for (int y=0;y<mapHeight;y++){
					for (int x=0;x<mapWidth;x++){
						// tile
						bw.Write(fixNumber( map[i][x,y]));
						// tileset TODO
						if (GetTilesetNumberFromId(map[i][x,y])!=0){
							Console.WriteLine(GetTilesetNumberFromId(map[i][x,y])+" "+x+" "+y);
						    }
						bw.Write(GetTilesetNumberFromId(map[i][x,y]));
					}
				}
			}
			
			bw.Dispose();
			bw.Close();
			
			Console.WriteLine ("Done.");
			Console.ReadKey ();
	

		}

		public static byte GetTilesetNumberFromId(int tile){
			if (tile==0){
				return 0;
			}
			for (int i = 1; i <= totalTilesets; i++) {
				// Oh no, we passed it.
				if (tile<tilesetFirstGid[i]){
					return (byte)(i-2);
				}
            }
			return (byte)(totalTilesets-1);
		}
		
		public static byte fixNumber (byte passedNumero) {
            if (totalTilesets == 1) {
				return passedNumero;
			}

           // byte totalTilesets = Convert.ToByte((currentLua.DoString("return #loadedMap.tilesets")[0]));


            for (int i = 2; i != totalTilesets + 1; i++) {
                if (passedNumero < tilesetFirstGid[i]) {
                    //  Debug.Print(Convert.ToByte(passedNumero - (tilesetFirstGid[i - 1] - 1)).ToString());
                    //Debug.Print("Patched " + passedNumero.ToString() + " to " + Convert.ToByte(passedNumero - (tilesetFirstGid[i - 1] - 1)).ToString());
                    return Convert.ToByte(passedNumero - (tilesetFirstGid[i-1]-1));
                }
            }
            //Debug.Print("Patched " + passedNumero.ToString() + " to " + Convert.ToByte(passedNumero - (tilesetFirstGid[totalTilesets] - 1)).ToString());
            return Convert.ToByte(passedNumero - (tilesetFirstGid[totalTilesets]-1));
          //  return 0;
        }


	}
}

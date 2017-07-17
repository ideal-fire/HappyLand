function Event01()
	-- x,y,layer,tileset,tile
	SetMapImageData(4,1,0,0,3);
end
function Event02()
	-- x,y,layer,tileset,tile
	SetMapImageData(3,1,0,0,3);
	chose = ShowMessage("This is a noob question that you should answer.",true);
	ShowMessage(tostring(chose),false);
	chose=nil;
end
function Event03()
	SetMapImageData(1,1,0,0,3);
end
function Event04()
	SetMapImageData(2,1,0,0,3);
end
function Event05(passedObject)
	SetMapImageData(2,2,0,0,2);
end
function MapDispose()
	UnloadTexture(tileset0);
	Event01=nil;
	Event02=nil;
	Event03=nil;
	Event04=nil;
	Event05=nil;
end

tileset0=LoadPNG("app0:Stuff/a.png");
SetTileset(tileset0,0);
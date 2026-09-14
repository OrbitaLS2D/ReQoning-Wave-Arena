bbox
{
	nopicmip
	nomipmaps
	{
		map gfx/misc/bbox.tga
		blendFunc GL_SRC_ALPHA GL_ONE
		rgbGen vertex
		alphaGen const 0.4
	}
}

bbox_nocull
{
	nopicmip
	nomipmaps
	cull none
	{
		map gfx/misc/bbox.tga
		blendFunc GL_SRC_ALPHA GL_ONE
		rgbGen vertex
		alphaGen const 0.4
	}
}


/* * * * * * * * * * * * * Author's note * * * * * * * * * * * *\
*   _       _   _       _   _       _   _       _     _ _ _ _   *
*  |_|     |_| |_|     |_| |_|_   _|_| |_|     |_|  _|_|_|_|_|  *
*  |_|_ _ _|_| |_|     |_| |_|_|_|_|_| |_|     |_| |_|_ _ _     *
*  |_|_|_|_|_| |_|     |_| |_| |_| |_| |_|     |_|   |_|_|_|_   *
*  |_|     |_| |_|_ _ _|_| |_|     |_| |_|_ _ _|_|  _ _ _ _|_|  *
*  |_|     |_|   |_|_|_|   |_|     |_|   |_|_|_|   |_|_|_|_|    *
*                                                               *
*                     http://www.humus.name                     *
*                                                                *
* This file is a part of the work done by Humus. You are free to   *
* use the code in any way you like, modified, unmodified or copied   *
* into your own work. However, I expect you to respect these points:  *
*  - If you use this file and its contents unmodified, or use a major *
*    part of this file, please credit the author and leave this note. *
*  - For use in anything commercial, please request my approval.     *
*  - Share your work and ideas too as much as you can.             *
*                                                                *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <Framework3/Direct3D10/D3D10App.h>
#include "GDModel.h"
#include "GDAnim.h"

class App : public D3D10App
{
public:
	char *getTitle() const { return "Wind Waker BMD Loading"; }

	bool init();
	void exit();

	bool initAPI();
	void exitAPI();

	bool load(int argc, char** argv);
	void unload();

	void resetCamera();

	void drawFrame();

	bool onKey(const uint key, const bool pressed);

protected:	
	ubyte* m_AnimBlob;
	GDModel::GDModel m_GDModel;

	bool animLoaded;
	GDAnim::GDAnim m_restAnim;
};

//
//  Settings.h
//  Sensel Morph
//
//  Created by Shuvashis Das on 10/24/18
//
//

#pragma once
#include "cinder/app/App.h"
#include "cinder/CinderMath.h"

#define WIDTH 1280
#define HEIGHT 720
#define APP_SIZE vec2(WIDTH, HEIGHT)
#define APP_BOUNDS Rectf(0, 0, WIDTH, HEIGHT)

using namespace ci;
using namespace std;

namespace sensel{

class Settings {
public:
	static Settings* getInstance();
    
    
	// drawing booleans
	bool				bDrawSensel				= true;
    bool                bShowParams             = false;
	bool				bRecordData				= false;
	bool				bUseRecordedData		= false;
	bool				bSendOscData			= false;
	float				mOutputWidth			= 24.0;
	float				mOutputHeight			= 14.0;
	float				mMaxForce				= 25.0;
	float				mFps					= 0; 

private:
	// Singleton
	Settings() {};					
	Settings(Settings const&){};	
	Settings& operator=(Settings const&) { return *m_pInstance; } 
	static Settings* m_pInstance;

};
}

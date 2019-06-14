//
//  SenselData.h
//
//  Created by Shuvashis Das on 10/16/18.
//
//

/* -- info from api - primer

Sensor is made of a grid of 185 columns x 105 rows of sensels
Accelerometer: Value range: 0-1; Units: Gs
Force array: Value range: 0-8192; Units: grams
Labels Array: Value range: 0-16
ID: Value range: 0 - 15
Coordinates: Value range: 0 - 240, 0 - 139
Area: Value range: 0 - 33360; Units: square mm
Peak Force:	Value range: 0 - 1000; Units: grams, mm

*/

#pragma once

#include <sstream>
#include <iostream>

#include "cinder/app/App.h"
#include "cinder/Log.h"
#include "cinder/gl/gl.h"
#include "cinder/app/RendererGl.h"
#include "cinder/Camera.h"
#include "cinder/CameraUi.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/VboMesh.h"
#include "cinder/CinderMath.h"
#include "cinder/ImageIo.h"
#include "cinder/Json.h"

#include "sensel.h"

#include "Settings.h"



using namespace ci;
using namespace ci::app;
using namespace std;

namespace rph {
	static const char* CONTACT_STATE_STRING[] = { "CONTACT_INVALID","CONTACT_START", "CONTACT_MOVE", "CONTACT_END" };
	static const int SENSEL_ROWS = 105;
	static const int SENSEL_COLS = 185;

	struct SenselDevice{
		// id to keep track of sensel device
		int					mId;
		string 				mSerialNum;
		
		// boolean to keep track of if sensel connection exists
		bool				mSenselConnected;

		// surface variable to draw final fbo texture
		Surface32fRef		mSenselForceSurface;
		vector<string>		mRecordedData;

	};

	class SenselData{
		
	  public:
		
		SenselData() { };
		~SenselData() { };
		
		void						setup( );
		void 						update();
		void 						draw();
		
		string 						outputToString();
		
	  private:

		// Handle that references a Sensel device
		SENSEL_HANDLE				mHandles[SENSEL_MAX_DEVICES] = { NULL };
		// List of all available Sensel devices
		SenselDeviceList			mList;
		// Sensor info from the Sensel device
		SenselSensorInfo			mSensorInfos[SENSEL_MAX_DEVICES] = { NULL };
		// SenselFrame data that will hold the forces
		SenselFrameData				*mFrames[SENSEL_MAX_DEVICES] = { NULL };

		vector< SenselDevice >		mSenselDeviceVector;

		// Functions and variables for FBO drawing
		void						updateSenselForceSurface( int senselIndex );
		Color 						remapCol( Color col, float force);

		gl::FboRef					mCombinedFbo;
		void 						renderCombinedFbo();
		
		// Singleton for Settings class
		sensel::Settings  			*mSettings = nullptr;
	};
}

//
//  SenselMorphDevice.h
//
//  Created by Daniel Scheibel on 6/23/19.
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

//#include <sstream>
//#include <iostream>

#include "cinder/app/App.h"
#include "cinder/Log.h"
#include "cinder/gl/gl.h"
//#include "cinder/app/RendererGl.h"
//#include "cinder/gl/Texture.h"
//#include "cinder/CinderMath.h"
#include "cinder/Json.h"

#include "sensel.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace rph {

	//	static const char* CONTACT_STATE_STRING[] = { "CONTACT_INVALID","CONTACT_START", "CONTACT_MOVE", "CONTACT_END" };

	class SenselMorphDevice{
		
	  public:
		static const int ROWS = 105;
		static const int COLS = 185;
		
		SenselMorphDevice() {};
		~SenselMorphDevice() {};
		
		virtual void setup(SenselDeviceID deviceId);
		virtual void update();
		virtual void draw();
		virtual void updateForceChannel( float maxForce = 1.0f );
		virtual void reconnect();
		
		string				mSerialNum = "";
		
		// boolean to keep track of if sensel connection exists
		bool				mConnected;
		
		// surface variable to draw final fbo texture
		Channel32fRef 		mForcesChannel;
		
		// Handle that references a Sensel device
		SENSEL_HANDLE		mHandle = NULL;
		
		// SenselFrame data that will hold the forces
		SenselFrameData		*mFrame = NULL;
		
	  protected:
		
		// Sensor info from the Sensel device
		SenselSensorInfo	mInfo;
		
	};
}

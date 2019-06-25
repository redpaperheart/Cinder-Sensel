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
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Texture.h"
#include "cinder/Json.h"

#include "sensel.h"
#include "SenselMorphDevice.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace rph {

	//	static const char* CONTACT_STATE_STRING[] = { "CONTACT_INVALID","CONTACT_START", "CONTACT_MOVE", "CONTACT_END" };

	class SenselMorphManager{
		
	  public:
		
		SenselMorphManager() { };
		~SenselMorphManager() { };
		
		void						setup( );
		void						update();
		void						draw();
		void						setMaxForce( float force ){ mMaxForce = force ; }

	  private:
		// List of all available Sensel devices
		SenselDeviceList			mDeviceList;

		vector< SenselMorphDevice* > mDevices;

//		gl::FboRef					mCombinedFbo;
		float						mMaxForce				= 25.0;
	};
}

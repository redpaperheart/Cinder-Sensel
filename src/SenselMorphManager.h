//
//  SenselMorphManager.h
//
//  Created by Daniel Scheibel on 6/23/19.
//
//

#pragma once

#include "cinder/app/App.h"
#include "cinder/Log.h"
#include "cinder/gl/gl.h"

#include "sensel.h"
#include "SenselMorphDevice.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace rph {

	class SenselMorphManager{
		
	  public:
		SenselMorphManager() {};
		~SenselMorphManager() {};
		
		virtual void setup( );
		virtual void update();
		virtual void draw();
		
		float mMaxForce = 25.0;
		
	  protected:
		vector<SenselMorphDevice*>	mDevices;
		
	};
}

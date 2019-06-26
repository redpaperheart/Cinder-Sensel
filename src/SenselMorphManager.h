//
//  SenselMorphManager.h
//
//  Created by Daniel Scheibel on 6/23/19.
//
//

#pragma once

//#include <sstream>
//#include <iostream>

#include "cinder/app/App.h"
#include "cinder/Log.h"
#include "cinder/gl/gl.h"
//#include "cinder/app/RendererGl.h"
//#include "cinder/gl/Fbo.h"
//#include "cinder/gl/Texture.h"
//#include "cinder/Json.h"

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
		
		void						setup( );
		void						update();
		void						draw();
		
//		void						setMaxForce( float force ){ mMaxForce = force ; }
		float						mMaxForce = 25.0;
		
	  private:
		
		vector<SenselMorphDevice*>	mDevices;
		
	};
}

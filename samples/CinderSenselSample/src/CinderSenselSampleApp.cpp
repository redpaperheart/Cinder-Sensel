#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"
#include "cinder/params/Params.h"

#include "sensel.h"
#include "sensel_device.h"
#include "SenselData.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CinderSenselSampleApp : public App {
  public:
	  //CinderSenselSampleApp();
	  void							setup() override;
	  void							update() override;
	  void							draw() override;
	  	  
	  // Indivisual Sensel Sensor instances containing force data
	  rph::SenselData				mSensel;

	  // Functions to save & load frame data to/from JSON
	  string						mFrame;
	  
	  // OSC string
	  string						mForceInfo = "";

	  // Numbers conntected Sensel sensors
	  int							mNumberOfSensel = 0;

	  // For printing information on app screen
	  Font							mFont;
	  gl::TextureFontRef			mTextureFont;

	  // Functions and variables for UI
	  void							setupParams();
	  params::InterfaceGlRef		mParams = nullptr;
	  float							mFps = 0;
	  float							mMaxForce = 50.0;	// by default the senselData class sets this to 25 
														// we can set other values here
};

void CinderSenselSampleApp::setup(){
	// Setting up font
	mFont = Font("Arial", 19);
	mTextureFont = gl::TextureFont::create(mFont);

	// setup sensel
	mSensel.setup();
	mSensel.setMaxForce(mMaxForce);

}

void CinderSenselSampleApp::setupParams(){

	if (mParams == nullptr) {
		mParams = params::InterfaceGl::create("General Settings", ivec2(250, 150));
		mParams->setPosition(ivec2(350, 20));
	}
	else {
		mParams->clear();
	}
	
	mParams->addSeparator();
	mParams->addParam("FPS: ", &mFps);

	mParams->addSeparator();
	mParams->addParam("Sensel Max Force", &mMaxForce);
}

void CinderSenselSampleApp::update(){
	mFps = getAverageFps();
	mSensel.update();
}

void CinderSenselSampleApp::draw(){
	gl::clear(Color(0.150, 0.150, 0.150));
	gl::color(Color::white());
	gl::ScopedMatrices m;
	mSensel.draw();
	mTextureFont->drawString(to_string(mFps), vec2(10, getWindowHeight() - mTextureFont->getDescent() * 2));
}

CINDER_APP( CinderSenselSampleApp, RendererGl )

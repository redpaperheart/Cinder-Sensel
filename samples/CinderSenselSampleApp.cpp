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
	  void						setup() override;
	  void						update() override;
	  void						draw() override;

	  void						mouseDown(MouseEvent event) override;
	  void						keyDown(KeyEvent event) override;
	  
	  // Indivisual Sensel Sensor instances containing force data
	  rph::SenselData				mSensel;

	  // Functions to save & load frame data to/from JSON
	  string						mFrame;

	  // Singleton for Settings class
	  sensel::Settings* mSettings = nullptr;

	  // OSC string
	  string						mForceInfo = "";

	  // Numbers conntected Sensel sensors
	  int							mNumberOfSensel = 0;

	  // For printing information on app screen
	  //	void						drawSenselInfo();
	  Font							mFont;
	  gl::TextureFontRef			mTextureFont;

	  // Functions and variables for UI
	  void							setupParams();
	  params::InterfaceGlRef		mParams = nullptr;
};

void prepareSettings(CinderSenselSampleApp::Settings* settings){
	settings->setTitle("Sensel Morph Visualizer");
	settings->setWindowSize(400, 300);
	settings->setHighDensityDisplayEnabled(false);
}

void CinderSenselSampleApp::setup(){
	// Setting up font
	mFont = Font("Arial", 19);
	mTextureFont = gl::TextureFont::create(mFont);

	// setting up singleton object
	mSettings = sensel::Settings::getInstance();

	// setup sensel
	mSensel.setup();

	// setup params
	setupParams();
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
	mParams->addParam("FPS: ", &mSettings->mFps);

	mParams->addSeparator();

	mParams->addParam("Draw Sensel", &mSettings->bDrawSensel);
	mParams->addParam("Sensel Max Force", &mSettings->mMaxForce);
}

void CinderSenselSampleApp::keyDown(KeyEvent event){
	if (event.getChar() == 'p') {
		mSettings->bShowParams = !mSettings->bShowParams;
		if (mSettings->bShowParams) {
			mParams->show();
		}
		else {
			mParams->hide();
		}
	}
}

void CinderSenselSampleApp::mouseDown( MouseEvent event )
{
}

void CinderSenselSampleApp::update(){
	mSettings->mFps = getAverageFps();
	mSensel.update();
}

void CinderSenselSampleApp::draw(){
	gl::clear(Color(0.150, 0.150, 0.150));
	gl::color(Color::white());
	gl::ScopedMatrices m;

	if (mSettings->bDrawSensel) {
		mSensel.draw();
	}
	mTextureFont->drawString(to_string(mSettings->mFps), vec2(10, getWindowHeight() - mTextureFont->getDescent() * 2));

	// Draw parameters
	if (mSettings->bShowParams) {
		mParams->draw();
	}
}

CINDER_APP( CinderSenselSampleApp, RendererGl )

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"
#include "cinder/params/Params.h"

#include "SenselMorphManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CinderSenselSampleApp : public App {
  public:
	
	void						setup() override;
	void						update() override;
	void						draw() override;
	  	  
	  // Indivisual Sensel Sensor instances containing force data
	rph::SenselMorphManager		mSenselMorphManager;

	
	  // Params & Debug
	void						setupParams();
	params::InterfaceGlRef		mParams = nullptr;
	gl::TextureFontRef			mTextureFont;
	float						mFps = 0;
	float						mMaxForce = 50.0;	// by default the senselData class sets this to 25
														// we can set other values here
};

void prepareSettings( CinderSenselSampleApp::Settings* settings )
{
	settings->setTitle( "Sensel Morph Sample" );
//	settings->setWindowSize(400, 300);
	settings->setHighDensityDisplayEnabled( true );
}

void CinderSenselSampleApp::setup(){
	mTextureFont = gl::TextureFont::create(Font("Arial", 19));

	// setup sensel
	mSenselMorphManager.setup();
	mSenselMorphManager.setMaxForce(mMaxForce);

}

void CinderSenselSampleApp::setupParams(){
	if (mParams == nullptr) {
		mParams = params::InterfaceGl::create("General Settings", ivec2(250, 150));
		mParams->setPosition(ivec2(350, 20));
	} else {
		mParams->clear();
	}
	mParams->addSeparator();
	mParams->addParam("FPS: ", &mFps);
	mParams->addParam("Sensel Max Force", &mMaxForce);
}

void CinderSenselSampleApp::update(){
	mFps = getAverageFps();
	mSenselMorphManager.update();
}

void CinderSenselSampleApp::draw(){
	gl::clear(Color(0.150, 0.150, 0.150));
	gl::color(Color::white());
	gl::ScopedMatrices m;
	
	mSenselMorphManager.draw();
	
	mTextureFont->drawString(to_string(mFps), vec2(10, getWindowHeight() - mTextureFont->getDescent() * 2));
}

CINDER_APP( CinderSenselSampleApp, RendererGl, prepareSettings )

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
	
};

void prepareSettings( CinderSenselSampleApp::Settings* settings )
{
	settings->setTitle( "Sensel Morph Sample" );
	settings->setHighDensityDisplayEnabled( true );
}

void CinderSenselSampleApp::setup(){
	setupParams();

	// setup sensel
	mSenselMorphManager.setup();
}

void CinderSenselSampleApp::setupParams(){
	mTextureFont = gl::TextureFont::create(Font("Arial", 19));
	if (mParams == nullptr) {
		mParams = params::InterfaceGl::create("Settings", ivec2(250, 150));
		mParams->setPosition(ivec2(350, 20));
	} else {
		mParams->clear();
	}
	mParams->addParam("Sensel Max Force", &mSenselMorphManager.mMaxForce).min(1.0f).step(0.1f);
}

void CinderSenselSampleApp::update(){
	mSenselMorphManager.update();
}

void CinderSenselSampleApp::draw(){
	gl::clear(Color(0.150, 0.150, 0.150));
	gl::color(Color::white());
	gl::ScopedMatrices m;
	
	mSenselMorphManager.draw();
	
	if (mParams) mParams->draw();
	if (mTextureFont) mTextureFont->drawString(to_string(getAverageFps()), vec2(10, getWindowHeight() - mTextureFont->getDescent() * 2));
	
}

CINDER_APP( CinderSenselSampleApp, RendererGl, prepareSettings )

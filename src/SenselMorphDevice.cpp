#include "SenselMorphDevice.h"
//#include <boost/algorithm/string.hpp>

namespace rph {
	
	void SenselMorphDevice::setup( SenselDeviceID deviceId ){
		
//		d->mId = i;
		mSerialNum = ci::toString(deviceId.serial_num);
		
		// Open a Sensel device by the id in the SenselDeviceList, handle initialized
		senselOpenDeviceByID(&mHandle, deviceId.idx);
		
		// Get the sensor info
		senselGetSensorInfo(mHandle, &mInfo);
		
		// Setting up sensel surface
//		mForcesSurfRef = ci::Surface32f::create(mInfo.num_cols, mInfo.num_rows, false);
		mForcesChannel = ci::Channel32f::create(mInfo.num_cols, mInfo.num_rows);
		
		// Set the frame content to scan force data
		senselSetFrameContent(mHandle, FRAME_CONTENT_PRESSURE_MASK);
		
		// Allocate a frame of data, must be done before reading frame data
		senselAllocateFrameData(mHandle, &mFrame);
		
		// Start scanning the Sensel device
		senselStartScanning(mHandle);
		
		mSenselConnected = true; // Sensel device is connected
	}

	void SenselMorphDevice::update(){
		// Read all available data from the Sensel device
		SenselStatus status = senselReadSensor(mHandle);
		if (status == SENSEL_ERROR ) {
			mSenselConnected = false;
		} else {
			// Get number of frames available in the data read from the sensor
			unsigned int num_frames = 0;
			senselGetNumAvailableFrames(mHandle, &num_frames);
//			CI_LOG_I("num_frames " << num_frames);
			for (int f = 0; f < num_frames; f++){
				// Read one frame of data
				if ( senselGetFrame(mHandle, mFrame) != SENSEL_OK) {
					CI_LOG_E("Sensen error.");
				}
			}
			updateSenselForceSurface();
		}
	}

	Color SenselMorphDevice::remapCol (Color col, float force){
		float c = ( force / mMaxForce ) ;
		col.r = c ;
		col.g = c ;
		col.b = c ;
		return col;
	}

	void SenselMorphDevice::updateSenselForceSurface(){
		if ( mFrame == NULL ) return;
		for (int i = 0; i < COLS * ROWS; i++){
			mFrame->force_array[i] /= mMaxForce ;
		}
		memcpy(mForcesChannel->getData(), mFrame->force_array, COLS * ROWS * 4);
		
//		gl::draw( gl::Texture2d::create( myChannel ) );
		//mForcesSurfRef->getChannelRed().getData()
//		float force = 0.0f;
//		ci::Color color;
////		 Set pixel values of surface variable with force data from sensel
//		float max = 0;
//		for (int i = 0; i < COLS; i++){
//			for( int j = 0 ; j < ROWS; j++ ){
//				max = math<float>::max( max, mFrame->force_array[i + j * COLS]);
//				force =  mFrame->force_array[i + j * COLS];
//				color = remapCol( ci::Color( 0, 0, 0 ), force );
//				mForcesSurfRef->setPixel( ivec2(i,j), color );
////				mForcesSurfRef->setPixel( ivec2(i,j), ci::Color::gray(force/mMaxForce) );
//			}
//		}
//		CI_LOG_I("max: " << max);
//		auto iter = mForcesSurfRef->getIter( mForcesSurfRef->getBounds() );
////		bool first = true;
//		while( iter.line() ) {
//			while( iter.pixel() ) {
////				force = mFrame->force_array[i + j * COLS]
//				iter.r() = iter.g() = iter.b() = force / mMaxForce;
//			}
//		}
	}
	
	void SenselMorphDevice::draw () {
//		gl::draw( gl::Texture2d::create( *mForcesSurfRef ) );
		gl::draw( gl::Texture2d::create( *mForcesChannel ) );
	}
}

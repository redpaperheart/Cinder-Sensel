#include "SenselData.h"
#include <boost/algorithm/string.hpp>

namespace rph {
	void SenselData::setup( ){

		// Get a list of avaialable Sensel devices
		senselGetDeviceList(&mList);

		CI_LOG_I( "Found total of " << to_string(mList.num_devices) << " Sensel devices." );
		
		if (mList.num_devices == 0) {
			CI_LOG_I( "No Sensel device found" );
		} else {
			// Main app telling how manmy Sensel devices need to be set up
			mSenselDeviceVector.resize( mList.num_devices );

			for (int i = 0; i < mList.num_devices; i++) {
				
				mSenselDeviceVector[i].mId = i;
				mSenselDeviceVector[i].mSerialNum = ci::toString(mList.devices[i].serial_num);
				
				CI_LOG_I( "Setting up Sensel device index: " << i << ", idx: " << to_string(mList.devices[i].idx) << ", serial: " << ci::toString(mList.devices[i].serial_num) );
				
				// Setting up sensel surface
				mSenselDeviceVector[i].mSenselForceSurface = ci::Surface32f::create(SENSEL_COLS, SENSEL_ROWS, false);
				
				mSenselDeviceVector[i].mSenselConnected = true; // Sensel device is connected

				// Open a Sensel device by the id in the SenselDeviceList, handle initialized
				senselOpenDeviceByID(&mHandles[i], mList.devices[i].idx);

				// Get the sensor info
				senselGetSensorInfo(mHandles[i], &mSensorInfos[i]);

				// Set the frame content to scan force data
				senselSetFrameContent(mHandles[i], FRAME_CONTENT_PRESSURE_MASK);

				// Allocate a frame of data, must be done before reading frame data
				senselAllocateFrameData(mHandles[i], &mFrames[i]);

				// Start scanning the Sensel device
				senselStartScanning(mHandles[i]);

			}
		}
	}

	void SenselData::update(){

		for ( int i = 0; i < mSenselDeviceVector.size(); i++ ){
			if( mSenselDeviceVector[i].mSenselConnected ){
				// Read all available data from the Sensel device
				SenselStatus status = senselReadSensor(mHandles[i]);
				if(status == SENSEL_ERROR ) {
					CI_LOG_I("SENSEL_ERROR at index: " << i);
					mSenselDeviceVector[i].mSenselConnected = false;
					continue;
				}
				
				// Get number of frames available in the data read from the sensor
				unsigned int num_frames = 0;
				senselGetNumAvailableFrames(mHandles[i], &num_frames);
				for (int f = 0; f < num_frames; f++){
					// Read one frame of data
					if ( senselGetFrame(mHandles[i], mFrames[i]) == SENSEL_OK) {
					} else {
						CI_LOG_E("Sensen error.");
					}
				}
				updateSenselForceSurface( i );
			} else {
				// try to reconnect
				senselGetDeviceList(&mList);
				for (int di = 0; di < mList.num_devices; di++) {
					CI_LOG_I(ci::toString(mList.devices[di].serial_num) <<" == " << mSenselDeviceVector[i].mSerialNum);
					if( ci::toString(mList.devices[di].serial_num) == mSenselDeviceVector[i].mSerialNum ) {
						senselOpenDeviceByID(&mHandles[i], mList.devices[di].idx);
						senselGetSensorInfo(mHandles[i], &mSensorInfos[i]);
						senselSetFrameContent(mHandles[i], FRAME_CONTENT_PRESSURE_MASK);
						senselAllocateFrameData(mHandles[i], &mFrames[i]);
						senselStartScanning(mHandles[i]);
						mSenselDeviceVector[i].mSenselConnected = true; // Sensel device is reconnected
					}
				}
				continue;
			}
		}
	}

	Color SenselData::remapCol (Color col, float force){
		float c = ( force / mMaxForce ) ;
		col.r = c ;
		col.g = c ;
		col.b = c ;
		return col;
	}

	void SenselData::updateSenselForceSurface( int senselIndex ){

		float force = 0.0f;
		ci::Color color;
		
		if ( mFrames[senselIndex] == NULL ) return;
		
		// Set pixel values of surface variable with force data from sensel
		for (int i = 0; i < SENSEL_COLS; i++){
			for( int j = 0 ; j < SENSEL_ROWS; j++ ){
				force =  mFrames[senselIndex]->force_array[i + j * SENSEL_COLS];
				color = remapCol( ci::Color( 0, 0, 0 ), force );
				mSenselDeviceVector[senselIndex].mSenselForceSurface->setPixel( ivec2(i,j), color );
			}
		}
	}
	
	void SenselData::draw () {
		gl::ScopedMatrices m;
		for (int i = 0; i < mSenselDeviceVector.size(); i++){
			gl::draw( gl::Texture2d::create( *mSenselDeviceVector[i].mSenselForceSurface) );
			gl::translate( 0, SENSEL_ROWS+20 );
		}
	}
}

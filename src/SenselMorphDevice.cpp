#include "SenselMorphDevice.h"

namespace rph {
	
	void SenselMorphDevice::setup( SenselDeviceID deviceId ){
		
		mSerialNum = ci::toString(deviceId.serial_num);
		
		// Open a Sensel device by the id in the SenselDeviceList, handle initialized
		senselOpenDeviceByID(&mHandle, deviceId.idx);
		
		// Get the sensor info
		senselGetSensorInfo(mHandle, &mInfo);
		
		// Setting up sensel surface
		mForcesChannel = ci::Channel32f::create(mInfo.num_cols, mInfo.num_rows);
		
		// Set the frame content to scan force data
		senselSetFrameContent(mHandle, FRAME_CONTENT_PRESSURE_MASK);
		
		// Allocate a frame of data, must be done before reading frame data
		senselAllocateFrameData(mHandle, &mFrame);
		
		// Start scanning the Sensel device
		senselStartScanning(mHandle);
		
		mConnected = true; // Sensel device is connected
	}

	void SenselMorphDevice::update(){
		if (!mConnected) {
			// try to reconnect
			if (mSerialNum != "") reconnect();
			return;
		}
		// Read all available data from the Sensel device
		SenselStatus status = senselReadSensor(mHandle);
		if (status == SENSEL_ERROR ) {
			CI_LOG_I("SENSEL_ERROR");
			mConnected = false;
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
		}
	}
	
	void SenselMorphDevice::reconnect(){
		SenselDeviceList deviceList;
		senselGetDeviceList(&deviceList);
		CI_LOG_I("trying to reconnect: " << mSerialNum);
		for (int di = 0; di < deviceList.num_devices; di++) {
			CI_LOG_I(ci::toString(deviceList.devices[di].serial_num) << " == " << mSerialNum);
			if( ci::toString(deviceList.devices[di].serial_num) == mSerialNum ) {
				CI_LOG_I("device found again: " << mSerialNum);
				senselOpenDeviceByID(&mHandle, deviceList.devices[di].idx);
				senselSetFrameContent(mHandle, FRAME_CONTENT_PRESSURE_MASK);
//				senselGetSensorInfo(mHandle, &mSensorInfo);
//				senselAllocateFrameData(mHandle, &mFrame);
				senselStartScanning(mHandle);
				mConnected = true; // Sensel device is reconnected
			}
		}
	}

	void SenselMorphDevice::updateForceChannel( float maxForce ){
		if ( mFrame == NULL ) return;
		if (maxForce != 1.0) {
			for (int i = 0; i < COLS * ROWS; i++){
				mFrame->force_array[i] /= maxForce ;
			}
		}
		memcpy(mForcesChannel->getData(), mFrame->force_array, COLS * ROWS * 4);
	}
	
	void SenselMorphDevice::draw () {
		gl::draw( gl::Texture2d::create( *mForcesChannel ) );
	}
}

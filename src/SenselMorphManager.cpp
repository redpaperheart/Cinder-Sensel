#include "SenselMorphManager.h"
//#include <boost/algorithm/string.hpp>

namespace rph {
	void SenselMorphManager::setup( ){

		// Get a list of avaialable Sensel devices
		senselGetDeviceList(&mDeviceList);

		CI_LOG_I( "Found total of " << to_string(mDeviceList.num_devices) << " Sensel devices." );
		
		if (mDeviceList.num_devices == 0) {
			CI_LOG_I( "No Sensel device found" );
		} else {
			// Main app telling how manmy Sensel devices need to be set up
//			mDevices.resize( mList.num_devices );

			for (int i = 0; i < mDeviceList.num_devices; i++) {
				CI_LOG_I( "Setting up Sensel device index: " << i << ", idx: " << to_string(mDeviceList.devices[i].idx) << ", serial: " << ci::toString(mDeviceList.devices[i].serial_num) );
				
				SenselMorphDevice *d = new SenselMorphDevice();
				d->setup(mDeviceList.devices[i]);
				mDevices.push_back(d);
			}
		}
	}

	void SenselMorphManager::update(){

		for ( int i = 0; i < mDevices.size(); i++ ){
			if( mDevices[i]->mConnected ){
				mDevices[i]->update();
				mDevices[i]->updateForceChannel();
			} else {
				// try to reconnect
//				senselGetDeviceList(&mDeviceList);
//				for (int di = 0; di < mDeviceList.num_devices; di++) {
//					CI_LOG_I(ci::toString(mDeviceList.devices[di].serial_num) << " == " << mDevices[i]->mSerialNum);
//					if( ci::toString(mDeviceList.devices[di].serial_num) == mDevices[i]->mSerialNum ) {
						// reconnect
//						senselOpenDeviceByID(&mHandles[i], mList.devices[di].idx);
//						senselGetSensorInfo(mHandles[i], &mSensorInfos[i]);
//						senselSetFrameContent(mHandles[i], FRAME_CONTENT_PRESSURE_MASK);
//						senselAllocateFrameData(mHandles[i], &mFrames[i]);
//						senselStartScanning(mHandles[i]);
//						mSenselDeviceVector[i].mSenselConnected = true; // Sensel device is reconnected
//					}
//				}
//				continue;
			}
		}
	}
	
	void SenselMorphManager::draw () {
		gl::ScopedMatrices m;
		for (int i = 0; i < mDevices.size(); i++){
			mDevices[i]->draw();
			gl::translate( 0, SenselMorphDevice::ROWS+20 );
		}
	}
}

#include "SenselMorphManager.h"

namespace rph {
	void SenselMorphManager::setup( ){
		
		SenselDeviceList deviceList;
		
		// Get a list of avaialable Sensel devices
		senselGetDeviceList(&deviceList);

		CI_LOG_I( "Found total of " << to_string(deviceList.num_devices) << " Sensel devices." );
		
		if (deviceList.num_devices == 0) {
			CI_LOG_I( "No Sensel device found" );
		} else {
			for (int i = 0; i < deviceList.num_devices; i++) {
				CI_LOG_I( "Setting up Sensel device index: " << i << ", idx: " << to_string(deviceList.devices[i].idx) << ", serial: " << ci::toString(deviceList.devices[i].serial_num) );
				SenselMorphDevice *d = new SenselMorphDevice();
				d->setup(deviceList.devices[i]);
				mDevices.push_back(d);
			}
		}
	}

	void SenselMorphManager::update(){
		for ( int i = 0; i < mDevices.size(); i++ ){
			mDevices[i]->update();
			mDevices[i]->updateForceChannel(mMaxForce);
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

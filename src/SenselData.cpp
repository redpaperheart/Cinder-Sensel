#include "SenselData.h"
#include <boost/algorithm/string.hpp>

namespace rph {
	void SenselData::setup( ){

		// Setting up singleton object
		mSettings = sensel::Settings::getInstance();

		// Get a list of avaialable Sensel devices
		senselGetDeviceList(&mList);

		// Clearing all vectors
//		clearForces();

		CI_LOG_I( "Found total of " << to_string(mList.num_devices) << " Sensel devices." );
		
		if (mList.num_devices == 0) {
			// Since no device is connected we will use the recorded data
			// the row and col number is the size of force array of the JSON data
			
			// Setting up fake sensel devices that would be read fromt the JSON file
//			mSenselDeviceVector.resize( checkDeviceNumInJSON() );
//			for ( int i = 0; i < checkDeviceNumInJSON(); i++ ){
				CI_LOG_I( "No Sensel device found" );
//				mSenselDeviceVector[i].mSenselConnected = false; // Sensel device is not connected
//				mSenselDeviceVector[i].mId = i;
//				mSenselDeviceVector[i].mSenselForceSurface = ci::Surface32f::create(SENSEL_COLS, SENSEL_ROWS, false);
//			}
			// Reading from JSON file
//			fromJSON();
		} else {
			// Main app telling how manmy Sensel devices need to be set up
			mSenselDeviceVector.resize( mList.num_devices );

			for (int i = 0; i < mList.num_devices; i++) {
				
				mSenselDeviceVector[i].mId = i;
//				mSenselDeviceVector[i].mFlipX = false;
//				mSenselDeviceVector[i].mFlipY = false;
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

//		mSettings->mNumberOfSensels = mSenselDeviceVector.size();
		
		gl::Fbo::Format format;
		format.setSamples( 4 );		// enable 4x antialiasing
		mCombinedFbo = gl::Fbo::create(int(mSettings->mOutputWidth), int(mSettings->mOutputHeight), format.depthTexture() );

	//	for( int i = 0 ; i < mSenselDeviceVector.size(); i++ ){
	//		mSettings->bSenselFlipX[i] = false;
	//		mSettings->bSenselFlipY[i] = false;
	//	}

	}

//	int SenselData::checkDeviceNumInJSON(){
//		string fileName = "sensel_force.json";
//		fs::path filePath( "../assets/sensel_force.json" );
//		int numDevicesRecorded = 0;
//		JsonTree device;
//		if( fs::exists( filePath ) ){
//			JsonTree senselForce( loadAsset( fileName ) );
//			for( JsonTree::ConstIter item = senselForce.begin(); item != senselForce.end(); ++item ) {
//				numDevicesRecorded = item->getNumChildren();
//			}
//			//CI_LOG_I("Number of devices recorded " << numDevicesRecorded);
//		}
//		else{
//			CI_LOG_I( fileName<<" does not exist" );
//		}
//		return numDevicesRecorded;
//	}

//	void SenselData::saveJSON( ){
//
//		string fileName = "sensel_force.json";
//		fs::path localFile = getAssetPath("") / fileName;
//		toJSON().write( localFile, ci::JsonTree::WriteOptions().createDocument(true) );
//
//	}

//	JsonTree SenselData::toJSON() {
//
//		JsonTree sensel_data = JsonTree::makeArray("sensel_data");
//		for (int i = 0; i < mSenselDeviceVector.size(); i++ )
//		{
//			JsonTree device = JsonTree::makeArray("sensor_" + to_string(i));
//			JsonTree frames  = JsonTree::makeObject("frames");
//			int frame_count = 0;
//			int stringVectorSize = mSenselDeviceVector[i].mRawForceStringVector.size();
//	//		float force;
//
//			if(stringVectorSize > 0){
//				for (int j = 0 ; j < stringVectorSize; j++){
//					//CI_LOG_I( "populating device " << senselIndex << " frame " << i);
//					frames.pushBack( JsonTree( "frames_" + to_string( frame_count ), mSenselDeviceVector[i].mRawForceStringVector[j]) );
//					device.pushBack(JsonTree( "", frames.getValueAtIndex(frame_count) ) );
//					frame_count++;
//				}
//				sensel_data.pushBack( device );
//			}
//		}
//		return sensel_data;
//	}

//	void SenselData::fromJSON() {
//
//		string fileName = "sensel_force.json";
//		fs::path filePath( "../assets/sensel_force.json" );
//		int numDevicesRecorded;
//		JsonTree device;
//		if( fs::exists( filePath ) ){
//			JsonTree senselForce( loadAsset( fileName ) );
//			for( JsonTree::ConstIter item = senselForce.begin(); item != senselForce.end(); ++item ) {
//				numDevicesRecorded = item->getNumChildren();
//				for (int i = 0; i < numDevicesRecorded; i++){
//					mSenselDeviceVector[ i ].mFileExists = true;
//					device = item->getChild(i);
//					mTotalRecerdedFrames = device.getNumChildren();
//					//CI_LOG_I( "recorded frame for device " << i << " " << mTotalRecerdedFrames );
//					//mSenselDeviceVector[i].mStringForceVectorFromJson.clear();
//					for( int j = 0; j < mTotalRecerdedFrames; j++ ){
//						mSenselDeviceVector[i].mStringForceVectorFromJson.push_back( device.getValueAtIndex( j ) );
//						//CI_LOG_I( "frame from device " << i << " " << device.getValueAtIndex( j ) );
//					}
//
//					//processRecordedStringFromJson( i );
//				}
//			}
//			CI_LOG_I("Number of devices recorded " << numDevicesRecorded);
//		}
//		else{
//			for( int i = 0; i < mSettings->mNumberOfSensels; i++)
//				mSenselDeviceVector[i].mFileExists = false;
//			CI_LOG_I( fileName << " does not exist" );
//		}
//	}


	//void SenselData::updateSenselFlip( int senselIndex ){
	//		mSenselDeviceVector[senselIndex].mFlipX = mSettings->bSenselFlipX[senselIndex];
	//		mSenselDeviceVector[senselIndex].mFlipY = mSettings->bSenselFlipY[senselIndex];
	//}

//	string SenselData::senselRawForceToString( int senselIndex ){
////		float forceFloat;
//
////		for (int i = 0; i < SENSEL_COLS; i++){
////			for( int j = 0 ; j < SENSEL_ROWS; j++ ){
////				forceFloat = mSenselDeviceVector[senselIndex].mForceArray[i + j * SENSEL_COLS];
////				mSenselDeviceVector[senselIndex].mRawForce[i + j * SENSEL_COLS] =  forceFloat;
////			}
////		}
//		string forceString = "";
//		int sizeOfVector = SENSEL_COLS * SENSEL_ROWS;
//		for (int i = 0; i < sizeOfVector; i++){
////			forceString += to_string( mSenselDeviceVector[senselIndex].mRawForce[i] );
//			forceString += to_string( mFrames[senselIndex]->force_array[i] );
//			if(i < (sizeOfVector - 1) ){
//				forceString += ",";
//			}
//		}
//		return forceString;
//	}

	void SenselData::update(){

		for ( int i = 0; i < mSenselDeviceVector.size(); i++ ){
//		for ( int i = 0; i < mList.num_devices; i++ ){
			// If the sensel device(s) needs to be flipped
	//		updateSenselFlip( i );
			if( mSenselDeviceVector[i].mSenselConnected ){
				// Read all available data from the Sensel device
				SenselStatus status = senselReadSensor(mHandles[i]);
				if(status == SENSEL_ERROR ) {
					CI_LOG_I("SENSEL_ERROR at index: " << i);
					mSenselDeviceVector[i].mSenselConnected = false;
//					senselGetDeviceList(&mList);
					continue;
				}
				
				// Get number of frames available in the data read from the sensor
				unsigned int num_frames = 0;
				senselGetNumAvailableFrames(mHandles[i], &num_frames);
				for (int f = 0; f < num_frames; f++){
					// Read one frame of data
					if ( senselGetFrame(mHandles[i], mFrames[i]) == SENSEL_OK) {
//						float force = 0.0f;
//						for (int fi = 0; fi < SENSEL_COLS; fi++){
//							for( int fj = 0 ; fj < SENSEL_ROWS; fj++ ){
//								force =  mFrames[i]->force_array[fi + fj * SENSEL_COLS];
////								if (force > 0.0f) CI_LOG_I( "frame force: " << force );
////                                CI_LOG_E("force: " << force);
//							}
//						}
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

			

			// Following vector is used for saving data to JSON file
//			if( mSettings->bRecordData ){
//				mSenselDeviceVector[i].mRawForceStringVector.push_back( senselRawForceToString( i ) );
//			}

			// Updating frame number if reading recorded data
//			if( mSettings->bUseRecordedData ){
//				if( mCurrentFameNumber < mTotalRecordedFrames){
//					mTempFameNumber++; //mSettings->mRecPlaySpeed;
//					mCurrentFameNumber = int( math<float>::ceil(mTempFameNumber));
//					if(mCurrentFameNumber == mTotalRecerdedFrames){
//						mCurrentFameNumber = 0;
//						mTempFameNumber = 0.0;
//					}
//				}
//			}
		}
		
		renderCombinedFbo();
	}

	Color SenselData::remapCol (Color col, float force)
	{
		float c = (force/mSettings->mMaxForce) ;
		col.r = c ;
		col.g = c ;
		col.b = c ;
		return col;
	}


	void SenselData::updateSenselForceSurface( int senselIndex ){

//		if(mSettings->bUseRecordedData){
//			processRecordedStringFromJson( senselIndex );
//		}

		float force = 0.0f;
		ci::Color color;
		
		if ( mFrames[senselIndex] == NULL ) return;
		
		// Set pixel values of surface variable with force data from sensel
		for (int i = 0; i < SENSEL_COLS; i++){
			for( int j = 0 ; j < SENSEL_ROWS; j++ ){
//				if(mSettings->bUseRecordedData && mSenselDeviceVector[senselIndex].mFileExists){
//					force =  stof(mSenselDeviceVector[senselIndex].mRecordedData[i + j*mSenselDeviceVector[senselIndex].mSenselCols]);
//				}
//				else if( mSenselDeviceVector[senselIndex].mSenselConnected ){
//					force =  mSenselDeviceVector[senselIndex].mForceArray[i + j * mSenselDeviceVector[senselIndex].mSenselCols];
				force =  mFrames[senselIndex]->force_array[i + j * SENSEL_COLS];
	//				CI_LOG_I( "force: " << force );
//				}

				//maxForceFloat = math<float>::max(force, maxForceFloat);
				
				color = remapCol( ci::Color( 0, 0, 0 ), force );
//				if( mSenselDeviceVector[senselIndex].mFlipX && !mSenselDeviceVector[senselIndex].mFlipY ){
//					mSenselDeviceVector[senselIndex].mSenselForceSurface->setPixel( ivec2( SENSEL_COLS - i,j ), color );
//				}
//				else if( mSenselDeviceVector[senselIndex].mFlipY && !mSenselDeviceVector[senselIndex].mFlipX ){
//					mSenselDeviceVector[senselIndex].mSenselForceSurface->setPixel( ivec2( i, SENSEL_ROWS - j ), color );
//				}
//				else if( mSenselDeviceVector[senselIndex].mFlipX && mSenselDeviceVector[senselIndex].mFlipY ){
//					mSenselDeviceVector[senselIndex].mSenselForceSurface->setPixel( ivec2( SENSEL_COLS -  i, SENSEL_ROWS - j ), color );
//				}
//				else{
					mSenselDeviceVector[senselIndex].mSenselForceSurface->setPixel( ivec2(i,j), color );
//				}
			}
		}
	}
	
	string SenselData::outputToString(){
		
		Surface32f surf = mCombinedFbo->readPixels32f(mCombinedFbo->getBounds());
		string forceString = "";
//		float forceFloat = 0.0f;
//		int sizeOfVector = surf.getWidth() * surf.getHeight();
//		for (int i = 0; i < surf.getWidth(); i++){
//			for( int j = 0 ; j < surf.getHeight(); j++ ){
//				if (i == 0 && j == 0 ) {
//					forceString = "";
//				} else {
//					forceString += ",";
//				}
//				forceFloat = surf.getPixel( vec2(i,j) ).r;
//				forceString += to_string(forceFloat);
//			}
//		}
		auto iter = surf.getIter( surf.getBounds() );
		bool first = true;
		while( iter.line() ) {
			while( iter.pixel() ) {
				if ( !first ) {
					forceString += ",";
				} else {
					first = false;
				}
				forceString += to_string(iter.r());
			}
		}
		return forceString;
	}
	
	void SenselData::renderCombinedFbo(){
		
		gl::ScopedFramebuffer fbScpFinal( mCombinedFbo );
		gl::clear( Color( 0.0, 0.0f, 0.0f ) );
		gl::ScopedViewport scpVpFinal( ivec2(0.0), mCombinedFbo->getSize() );
		gl::ScopedMatrices matFinal;
		gl::setMatricesWindow( mCombinedFbo->getSize() );
		
		if ( mSenselDeviceVector.size() == 1 ){ //only one sensel connected
			gl::pushMatrices();
//			gl::draw( mSenselTextureFinalFboVector[0]->getColorTexture() );
			gl::popMatrices();
		}
		else if ( mSenselDeviceVector.size() == 2 )
		{
//			{
//				gl::ScopedColor c (1,0,0);
//				gl::drawSolidRect(Rectf(0,0,mSettings->mOutputWidth*0.5f, mSettings->mOutputHeight));
//			}
//			{
//				gl::ScopedColor c (1,1,0);
//				gl::drawSolidRect(Rectf(mSettings->mOutputWidth*0.5f,0,mSettings->mOutputWidth, mSettings->mOutputHeight));
//			}
			{
				gl::ScopedMatrices m;
				gl::translate(0, mSettings->mOutputHeight);
				gl::rotate(-M_PI * 2.0f);
				gl::scale( mSettings->mOutputHeight/SENSEL_COLS, mSettings->mOutputWidth*0.5f/SENSEL_ROWS);
				gl::draw( gl::Texture2d::create( *mSenselDeviceVector[0].mSenselForceSurface) );
			}
			{
				gl::ScopedMatrices m;
				gl::translate(mSettings->mOutputWidth, 0);
				gl::rotate(M_PI * 2.0f);
				gl::scale( mSettings->mOutputHeight/SENSEL_COLS, mSettings->mOutputWidth*0.5f/SENSEL_ROWS);
				gl::draw( gl::Texture2d::create( *mSenselDeviceVector[1].mSenselForceSurface) );
			}
		}
	}

	void SenselData::draw ()
	{
		gl::ScopedMatrices m;
		for (int i = 0; i < mSenselDeviceVector.size(); i++){
			gl::draw( gl::Texture2d::create( *mSenselDeviceVector[i].mSenselForceSurface) );
			gl::translate( 0, SENSEL_ROWS+20 );
		}
		gl::draw( mCombinedFbo->getColorTexture() );
	}
}

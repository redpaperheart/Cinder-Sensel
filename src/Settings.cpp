//
//  Settings.cpp
//  Sensel Morph
//
//  Created by Shuvashis Das on 10/24/18
//
//

#include "Settings.h"

namespace sensel{

	// Global static pointer used to ensure a single instance of the class.
	Settings* Settings::m_pInstance = NULL;
	Settings* Settings::getInstance(){

		if (!m_pInstance){ 
			m_pInstance = new Settings;
		}
		return m_pInstance;
	}

}

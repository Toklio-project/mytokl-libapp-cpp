//
//  AppServiceLocator.hpp
//  MyMonero
//
//  Copyright (c) 2014-2019, MyMonero.com
//
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without modification, are
//  permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, this list of
//  conditions and the following disclaimer.
//
//  2. Redistributions in binary form must reproduce the above copyright notice, this list
//  of conditions and the following disclaimer in the documentation and/or other
//  materials provided with the distribution.
//
//  3. Neither the name of the copyright holder nor the names of its contributors may be
//  used to endorse or promote products derived from this software without specific
//  prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
//  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
//  THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
//  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
//  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//
//
#ifndef AppServiceLocator_HPP_
#define AppServiceLocator_HPP_
//
#include <iostream> // TODO: this is to obtain stdlib.. what should be imported instead of this?
//
#include "../Dispatch/Dispatch_Interface.hpp"
#include "../Passwords/PasswordController.hpp"
#include "../Settings/SettingsController.hpp"
#include "../UserIdle/UserIdle.hpp"
#include "../Currencies/Currencies.hpp"
#include "../Wallets/WalletsListController.Full.hpp"
#include "../APIClient/HTTPRequests_Interface.hpp"
#include "../APIClient/HostedMonero.hpp"
#include "cryptonote_config.h"
//
namespace App
{
	using namespace std;
	using namespace cryptonote;
	//
	class ServiceLocator_SpecificImpl;
	//
	class ServiceLocator
	{
		private:
			ServiceLocator() {}
			ServiceLocator(const ServiceLocator&) = delete;
			ServiceLocator& operator=(const ServiceLocator&) = delete;
			//
			ServiceLocator_SpecificImpl *_pImpl;
			//
			ServiceLocator &_shared_build( // use this for platform-specific implementations of ServiceLocator::build
				std::shared_ptr<string> this_documentsPath,
				network_type this_nettype,
				std::shared_ptr<HTTPRequests::RequestFactory> this_httpRequestFactory,
				std::shared_ptr<Dispatch::Dispatch> this_dispatch_ptr
			) {
				//
				// TODO: assert non-nullptr deps here?
				//
				documentsPath = this_documentsPath;
				nettype = this_nettype;
				//
				dispatch_ptr = this_dispatch_ptr; // store - it got std::moved
				httpRequestFactory = this_httpRequestFactory;
				//
				settingsController = std::make_shared<Settings::Controller>();
				userIdleController = std::make_shared<UserIdle::Controller>();
				passwordController = std::make_shared<Passwords::Controller>();
				ccyConversionRatesController = std::make_shared<Currencies::ConversionRatesController>();
				walletsListController = std::make_shared<Wallets::ListController>(nettype);
				apiClient = std::make_shared<HostedMonero::APIClient>();
				//
				passwordController->documentsPath = documentsPath;
				passwordController->dispatch_ptr = dispatch_ptr;
				passwordController->userIdleController = userIdleController;
				passwordController->setup();
				//
				userIdleController->documentsPath = documentsPath;
				userIdleController->dispatch_ptr = dispatch_ptr;
				userIdleController->idleTimeoutAfterS_SettingsProvider = settingsController;
				userIdleController->setup();
				//
				settingsController->documentsPath = documentsPath;
				settingsController->dispatch_ptr = dispatch_ptr;
				settingsController->passwordController = passwordController;
				settingsController->setup();
				//
				apiClient->requestFactory = httpRequestFactory;
				apiClient->settingsController = settingsController;
				apiClient->setup();
				//
				ccyConversionRatesController->setup();
				//
				walletsListController->documentsPath = documentsPath;
				walletsListController->dispatch_ptr = dispatch_ptr;
				walletsListController->passwordController = ServiceLocator::instance().passwordController;
				walletsListController->apiClient = apiClient;
				walletsListController->userIdleController = userIdleController;
				walletsListController->ccyConversionRatesController = ccyConversionRatesController;
				walletsListController->setup();
				//
				built = true;
				//
				return *this;
			}
		//
		public:
			~ServiceLocator();
			void teardown()
			{
				documentsPath = nullptr;
				httpRequestFactory = nullptr;
				//
				dispatch_ptr = nullptr;
				apiClient = nullptr;
				settingsController = nullptr;
				passwordController = nullptr;
				userIdleController = nullptr;
				ccyConversionRatesController = nullptr;
				walletsListController = nullptr;
			}
			//
			static ServiceLocator& instance()
			{
				static ServiceLocator pInstance;
				return pInstance;
			}
			bool uniqueFlag = false;
			//
			// Properties - Initial: Status
			bool built = false;
			// Properties - Initial: Required for build()
			std::shared_ptr<string> documentsPath;
			network_type nettype;
			std::shared_ptr<HTTPRequests::RequestFactory> httpRequestFactory;
			// Properties - Services: Built and retained dependencies
			std::shared_ptr<Dispatch::Dispatch> dispatch_ptr;
			std::shared_ptr<HostedMonero::APIClient> apiClient;
			std::shared_ptr<Passwords::Controller> passwordController;
			std::shared_ptr<Settings::Controller> settingsController;
			std::shared_ptr<UserIdle::Controller> userIdleController;
			std::shared_ptr<Currencies::ConversionRatesController> ccyConversionRatesController;
			std::shared_ptr<Wallets::ListController> walletsListController;
			//
			// Lifecycle - Init
			ServiceLocator &build(
				std::shared_ptr<string> documentsPath,
				network_type nettype,
				std::shared_ptr<HTTPRequests::RequestFactory> httpRequestFactory
			); // simply returns the singleton for convenience
   };
}

#endif /* AppServiceLocator_HPP_ */

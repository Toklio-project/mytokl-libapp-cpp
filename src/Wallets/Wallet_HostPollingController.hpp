//
//  Wallet_HostPollingController.hpp
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
//	conditions and the following disclaimer.
//
//  2. Redistributions in binary form must reproduce the above copyright notice, this list
//	of conditions and the following disclaimer in the documentation and/or other
//	materials provided with the distribution.
//
//  3. Neither the name of the copyright holder nor the names of its contributors may be
//	used to endorse or promote products derived from this software without specific
//	prior written permission.
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

#ifndef HostPollingController_hpp
#define HostPollingController_hpp
//
#include <functional>
#include "../APIClient/HTTPRequests_Interface.hpp"
//
namespace Wallets
{
	using namespace std;
//	using namespace boost;
	//
	// Forward-declarations
	class Object;
	//
	// Controllers
	class HostPollingController
	{
	public:
		//
		// Lifecycle - Init
		HostPollingController(
			const Object &wallet,
			std::function<void()>&& didUpdate_factorOf_isFetchingAnyUpdates_fn
		):
		_wallet(wallet),
		_didUpdate_factorOf_isFetchingAnyUpdates_fn(std::move(didUpdate_factorOf_isFetchingAnyUpdates_fn))
		{
		}
		~HostPollingController()
		{
		}
		//
		// Dependencies


		//
		// Properties

		//
		// Signals

		//
		// Imperatives
		void requestFromUI_manualRefresh();
	private:
		//
		// Properties
		const Object &_wallet;
		std::function<void()> _didUpdate_factorOf_isFetchingAnyUpdates_fn;
		//
		// Imperatives

	};
}

#endif /* HostPollingController_hpp */

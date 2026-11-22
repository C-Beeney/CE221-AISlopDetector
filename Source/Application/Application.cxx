/*---------------------------------------------------------------------------|
|                                                                            |
|   Copyright 2026 Connor R Beeney                                           |
|                                                                            |
|   Licensed under the Apache License, Version 2.0 (the "License");          |
|   you may not use this file except in compliance with the License.         |
|   You may obtain a copy of the License at                                  |
|                                                                            |
|       http://www.apache.org/licenses/LICENSE-2.0                           |
|                                                                            |
|   Unless required by applicable law or agreed to in writing, software      |
|   distributed under the License is distributed on an "AS IS" BASIS,        |
|   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. |
|   See the License for the specific language governing permissions and      |
|   limitations under the License.                                           |
|                                                                            |
|---------------------------------------------------------------------------*/

#include <Application/Application.hxx>

#include <cassert>
#include <numeric>
#include <algorithm>

#include <iostream>

#include <vector>
#include <string_view>

void
::Application::Application::run( void
) {

	assert(not this->running);
	this->running = true;

	this->start();
	while(this->running) {
		this->update();
	}
}

void
::Application::Application::stop( void
) {
	assert(this->running);
	this->running = false;
}

void
::Application::Application::submitEvent(
	::Application::Event &event
) {
	this->events.emplace(event);
}


::std::vector<::std::string_view>
Application::Application::getViewNames(
) const {
	return ::std::accumulate(viewByName.cbegin(), viewByName.cend(),
			decltype(getViewNames())(),
			[](	auto viewNames,
				auto entry
			) {	auto [a, _] {entry};
				viewNames.emplace_back(a);
				return viewNames;
			});
}

void
::Application::Application::setCurrentViewByName(
	::std::string_view name
) {
	auto viewNames { this->getViewNames() };
	if (::std::find(viewNames.cbegin(), viewNames.cend(), name)
			!= viewNames.cend()) {
		this->currentView = this->viewByName.at(name);
	}
}
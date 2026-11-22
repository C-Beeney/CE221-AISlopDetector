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

#include <Application/AISlopDetector.hxx>

#include <iostream>
#include <chrono>

::Application::AISlopDetector::AISlopDetector( void
) :	landingView (::std::ref(*this)),
	addView     (::std::ref(*this)),
	editView    (::std::ref(*this)),
	removeView  (::std::ref(*this)),
	showView    (::std::ref(*this))
{
	this->currentView = landingView;

	viewByName = {
		{"landing", std::ref(landingView)},
		{"add"    , std::ref(addView    )},
		{"edit"   , std::ref(editView   )},
		{"remove" , std::ref(removeView )},
		{"show"   , std::ref(showView   )}
	};

	prop = {
		{"currentView", std::ref(this->currentView)},
	};
}

void
::Application::AISlopDetector::start( void
) {
}

void
::Application::AISlopDetector::update( void
) {	
	::View::View::SubmitEvent callable = [this](Event &e)
	{	submitEvent(e);
	};

	this->currentView->get().display(callable);
	while(events.size()) {
		auto const &ev { ::std::move(events.front()) };
		ev(*this);
		events.pop();
	}
}

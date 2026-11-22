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

#pragma once

#include <View/CLI/Landing.hxx>
#include <Model/SlopDictionary.hxx>

#include <Application/AISlopDetector.hxx>

#include <iostream>
#include <string>

void ::View::Landing::display(
	::View::View::SubmitEvent &submitEvent
) const {
	unsigned c {};
	auto views = std::vector<std::string> ({"add", "edit", "remove", "show"});

	std::cout
		<< "AI Slop Detector:\n"
		<< "\t" << ++c << " - Edit\n"
		<< "\t" << ++c << " - Add\n"
		<< "\t" << ++c << " - Show\n"
		<< "\t" << ++c << " - Remove\n"
		<< "\t" << ++c << " - Quit\n"
		<< "Your choice: " << std::flush;

	std::string ret;
	std::getline(std::cin, ret);

	::Application::Event event;
	auto reti = std::stoi(ret);
	if (reti >= 1 && reti <= 4)
		event = [&reti](::Application::Application &a)
		{	auto viewNames { a.getViewNames()   };
			auto viewName  { viewNames.at(reti-1) };
			std::cout << "vn -> " << viewName << std::endl;
			a.setCurrentViewByName(viewName);
		};
	else
	if (ret == "5")
		event = [](::Application::Application &a){ a.stop(); };
	
	submitEvent(event);
    
}
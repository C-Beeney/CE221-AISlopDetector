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

#include <View/CLI/Dictionary/Add.hxx>
#include <Application/Application.hxx>

#include <iostream>

void
::View::Dictionary::Add::display(
	::View::View::SubmitEvent &submitEvent
) const {

	using CurrentView = ::std::optional<::std::reference_wrapper<::View::View const>>;
	std::cout << "ADD DISPLAY" << std::endl;
	::Application::Event event =
		[](::Application::Application &a)
		{
			std::cout << "?" << std::endl;
			std::cout << a.getPropertyByName<CurrentView>("currentView")
					->get().getViewName()
				<< std::endl;
			std::cout << "?" << std::endl;
		
		};

	submitEvent(event);
}
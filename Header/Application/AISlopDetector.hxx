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

#include <Application/Application.hxx>

#include <View/CLI/Landing.hxx>
#include <View/CLI/Dictionary/common.hxx>

#include <Model/SlopDictionary.hxx>

namespace Application {
	class AISlopDetector final :
			public ::Application::Application
	{
	public:
		AISlopDetector();

	private:
		void start () override;
		void update() override;

	private:
		using SlopDict = ::Model::SlopDictionary;

		SlopDict slop;

		::View::Landing            landingView;
		::View::Dictionary::Add    addView;
		::View::Dictionary::Edit   editView;
		::View::Dictionary::Remove removeView;
		::View::Dictionary::Show   showView;

	};
}

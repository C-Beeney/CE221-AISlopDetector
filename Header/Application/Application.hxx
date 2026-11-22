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

#include <queue>
#include <functional>
#include <vector>
#include <string_view>
#include <optional>

#include <View/View.hxx>
#include <Model/Model.hxx>

#include <Application/Event.hxx>

#ifdef COMPLY_WITH_ASSIGNMENT
	#include <unordered_map>
#else
	#include <ankerl/unordered_dense.h>
#endif

namespace Application {
	class Application : public Model::Model
	{
	public:
		Application() = default;
		void submitEvent(Event &);

		void run();
		void stop();

		void setCurrentViewByName(::std::string_view const);
		::std::vector<::std::string_view> getViewNames() const;


	protected:
		virtual void update() = 0;
		virtual void start() = 0;

	protected:
		bool running;

		::std::optional<::std::reference_wrapper<::View::View const>> currentView;

		#ifdef COMPLY_WITH_ASSIGNMENT
		using map_t = ::std::unordered_map<
			::std::string_view,
			::std::reference_wrapper<::View::View>>;
		#else
		using map_t = ::ankerl::unordered_dense::map<
			::std::string_view,
			::std::reference_wrapper<::View::View>>;
		#endif
		
		map_t viewByName;

		::std::queue<Event> events;
	};
}

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

#include <any>
#include <string_view>
#include <vector>

#include <functional>

#ifdef COMPLY_WITH_ASSIGNMENT
	#include <unordered_map>
#else
	#include <ankerl/unordered_dense.h>
#endif

namespace Model 
{
	class Model
	{
		using any = ::std::any;
		using string_view = ::std::string_view;
	public:
		template<typename T>
		T &operator[](string_view const);

		template<typename T>
		T const &getPropertyByName(string_view const) const;

		::std::vector<::std::string_view>
				getPropertyNames(void) const;
	protected:
		#ifdef COMPLY_WITH_ASSIGNMENT
		using map_t = ::std::unordered_map<
			string_view, any>;
		#else
		using map_t = ::ankerl::unordered_dense::map<
			string_view, any>;
		#endif

		map_t prop;
		
    	};
}

#include <Model/Model.txx>
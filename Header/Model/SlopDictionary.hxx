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

#include <optional>
#include <string_view>

#include <Model/Model.hxx>

namespace Model
{
	class SlopDictionary final : public Model
	{
	public:
		enum class Mode {
			SYNC, CACHE, MEM
		};

		enum class Error {
			OK, FULL, EMPTY, NOMEM, RANGE, IO, ILLEGAL
		};

		static constexpr const char *const ErrorString[] = {
			"OK", "FULL", "EMPTY", "NOMEM", "RANGE"
		};
		
	public:
		static constexpr auto ENTRY_MAX { 3 };

		explicit SlopDictionary(
			const Mode mode = Mode::MEM,
			const std::optional<std::string_view>
				filePath = std::nullopt
		);

		SlopDictionary(const SlopDictionary&)
			= delete;
		SlopDictionary& operator=(const SlopDictionary&)
			= delete;

		~SlopDictionary();

		Error add(
			std::string_view string
		);

		Error replace(
			std::string_view string,
			const unsigned short index
		);

		Error remove(
			const unsigned short index
		);

		void setMode(
			const Mode mode
		);

		Mode getMode(
		) const;

		const char *const (&getEntries() const)[ENTRY_MAX];
		const char *const (&operator[](unsigned long i) const);

	private:
		const char *filePath;
		const char *entries[ENTRY_MAX];
		Mode mode;
	};
}
